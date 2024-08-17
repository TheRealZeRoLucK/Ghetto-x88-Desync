#include "../features.hpp"

#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))

bool visuals::player::get_playerbox(player_t* ent, visuals::player::box& in) {

	vec3_t origin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
	int left, top, right, bottom;

	origin = ent->abs_origin();
	min = ent->collideable()->mins() + origin;
	max = ent->collideable()->maxs() + origin;

	vec3_t points[] = {
		vec3_t(min.x, min.y, min.z),
		vec3_t(min.x, max.y, min.z),
		vec3_t(max.x, max.y, min.z),
		vec3_t(max.x, min.y, min.z),
		vec3_t(max.x, max.y, max.z),
		vec3_t(min.x, max.y, max.z),
		vec3_t(min.x, min.y, max.z),
		vec3_t(max.x, min.y, max.z)
	};
	if (!interfaces::debug_overlay->world_to_screen(points[3], flb) || !interfaces::debug_overlay->world_to_screen(points[5], brt)
		|| !interfaces::debug_overlay->world_to_screen(points[0], blb) || !interfaces::debug_overlay->world_to_screen(points[4], frt)
		|| !interfaces::debug_overlay->world_to_screen(points[2], frb) || !interfaces::debug_overlay->world_to_screen(points[1], brb)
		|| !interfaces::debug_overlay->world_to_screen(points[6], blt) || !interfaces::debug_overlay->world_to_screen(points[7], flt))
		return false;

	vec3_t arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for (int i = 1; i < 8; i++) {
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}
	in.x = (int)left;
	in.y = (int)top;
	in.w = int(right - left);
	in.h = int(bottom - top);


	return true;
}

void visuals::player::draw_box(player_t* ent, visuals::player::box bbox)
{
	if (!(variables::draw_box)) return;

	render::draw_rect(bbox.x, bbox.y, bbox.w, bbox.h, color(255, 255, 255, 255)); // For Corner Boxes: render::draw_rect(bbox.x, bbox.y, bbox.w * 0.25, bbox.h * 0.25, color(255, 255, 255, 255));
	render::draw_rect(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, color(0, 0, 0, 170)); // For Corner Boxes: render::draw_rect(bbox.x - 1, bbox.y - 1, (bbox.w + 2) * 0.25, (bbox.h + 2) * 0.25, color(0, 0, 0, 170)); 
	render::draw_rect(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, color(0, 0, 0, 170)); // For Corner Boxes: render::draw_rect(bbox.x + 1, bbox.y + 1, (bbox.w - 2) * 0.25, (bbox.h - 2) * 0.25, color(0, 0, 0, 170));
}

void visuals::player::radar_spot(player_t* entity) {

	if (!entity
		|| !csgo::local_player
		|| !entity->is_alive()
		|| entity == csgo::local_player
		|| entity->dormant())
		return;

	entity->spotted() = variables::radar_spotted ? 1 : 0;
}

void visuals::player::draw_name(player_t* entity, visuals::player::box bbox) {

	if (!variables::draw_name)
		return;


	float alpha[65] = { 255 };

	static float lastDormantTime[65] = { 0 }, lastNonDormantTime[65] = { 0 }; // 64 maxplayers so 1-65 as entindex
	float fadespeed = 1.0; // adding this so you can modify it, fadeout speed in seconds 1.0 = 1s 0.5 = 0.5s etc
	int eid = entity->index(); // calling it once would be a better option
	if (entity->dormant())
	{
		lastDormantTime[eid] = interfaces::globals->realtime;
		alpha[eid] = clamp((lastNonDormantTime[eid] - interfaces::globals->realtime) * (255.f / fadespeed), 0, 255);
	}
	else // you don't need an if here since its either true or false
	{
		lastNonDormantTime[eid] = interfaces::globals->realtime + fadespeed;
		alpha[eid] = clamp((interfaces::globals->realtime - lastDormantTime[eid]) * (255.f / fadespeed), 0, 255);
	}
	player_info_t info;

	interfaces::engine->get_player_info(entity->index(), &info);

	render::text(bbox.x + (bbox.w / 2), bbox.y - 15, render::fonts::watermark_font, info.name, true, color(255, 255, 255, alpha[eid]));
}

void visuals::player::draw_hp(player_t* entity, visuals::player::box bbox) {

	if (!variables::draw_hp)
		return;

	float alpha[65] = { 255 };

	static float lastDormantTime[65] = { 0 }, lastNonDormantTime[65] = { 0 }; // 64 maxplayers so 1-65 as entindex
	float fadespeed = 1.0; // adding this so you can modify it, fadeout speed in seconds 1.0 = 1s 0.5 = 0.5s etc
	int eid = entity->index(); // calling it once would be a better option
	if (entity->dormant())
	{
		lastDormantTime[eid] = interfaces::globals->realtime;
		alpha[eid] = clamp((lastNonDormantTime[eid] - interfaces::globals->realtime) * (255.f / fadespeed), 0, 255);
	}
	else // you don't need an if here since its either true or false
	{
		lastNonDormantTime[eid] = interfaces::globals->realtime + fadespeed;
		alpha[eid] = clamp((interfaces::globals->realtime - lastDormantTime[eid]) * (255.f / fadespeed), 0, 255);
	}

	int entity_health = entity->health();

	int hp_bar = bbox.h * (entity_health / 100.f);


	color hp_color = color(0, 0, 0, alpha[eid]);

	if (entity_health >= 51)
		hp_color = color(0, 255, 0, alpha[eid]);
	else if (entity_health <= 50 && entity_health > 10)
		hp_color = color(255, 205, 1, alpha[eid]);
	else if (entity_health <= 10)
		hp_color = color(255, 0, 0, alpha[eid]);

	render::draw_filled_rect(bbox.x - 7, bbox.y - 1, 4, bbox.h + 2, color(35, 35, 35, alpha[eid]));
	render::draw_filled_rect(bbox.x - 6, bbox.y + bbox.h - hp_bar, 2, hp_bar, hp_color);

	if (entity_health < 100)
		render::text(bbox.x - 8, bbox.y + bbox.h - hp_bar - 12, render::fonts::watermark_font, std::to_string(entity->health()), false, color(255, 255, 255, alpha[eid]));
}

void visuals::player::draw_weapon(player_t* entity, visuals::player::box bbox) {

	if (!variables::draw_weapon)
		return;

	float alpha[65] = { 255 };

	static float lastDormantTime[65] = { 0 }, lastNonDormantTime[65] = { 0 }; // 64 maxplayers so 1-65 as entindex
	float fadespeed = 1.0; // adding this so you can modify it, fadeout speed in seconds 1.0 = 1s 0.5 = 0.5s etc
	int eid = entity->index(); // calling it once would be a better option
	if (entity->dormant())
	{
		lastDormantTime[eid] = interfaces::globals->realtime;
		alpha[eid] = clamp((lastNonDormantTime[eid] - interfaces::globals->realtime) * (255.f / fadespeed), 0, 255);
	}
	else // you don't need an if here since its either true or false
	{
		lastNonDormantTime[eid] = interfaces::globals->realtime + fadespeed;
		alpha[eid] = clamp((interfaces::globals->realtime - lastDormantTime[eid]) * (255.f / fadespeed), 0, 255);
	}


	weapon_t* weapon = reinterpret_cast<weapon_t*>(entity->active_weapon());

	if (!weapon)
		return;

	std::string weapon_name = weapon->get_weapon_data()->weapon_name;

	weapon_name.erase(0, 7);
	std::transform(weapon_name.begin(), weapon_name.end(), weapon_name.begin(), tolower);

	if (variables::draw_ammo) {

		if (weapon->clip1_count() >= 0)
			render::text(bbox.x + (bbox.w / 2), bbox.y + (bbox.h + 6), render::fonts::watermark_font, weapon_name, true, color(255, 255, 255, alpha[eid]));
		else
			render::text(bbox.x + (bbox.w / 2), bbox.y + (bbox.h), render::fonts::watermark_font, weapon_name, true, color(255, 255, 255, alpha[eid]));
	}
	else
		render::text(bbox.x + (bbox.w / 2), bbox.y + (bbox.h), render::fonts::watermark_font, weapon_name, true, color(255, 255, 255, alpha[eid]));

}

void visuals::player::draw_ammo(player_t* entity, visuals::player::box bbox) {

	if (!variables::draw_ammo)
		return;


	float alpha[65] = { 255 };

	static float lastDormantTime[65] = { 0 }, lastNonDormantTime[65] = { 0 }; // 64 maxplayers so 1-65 as entindex
	float fadespeed = 1.0; // adding this so you can modify it, fadeout speed in seconds 1.0 = 1s 0.5 = 0.5s etc
	int eid = entity->index(); // calling it once would be a better option
	if (entity->dormant())
	{
		lastDormantTime[eid] = interfaces::globals->realtime;
		alpha[eid] = clamp((lastNonDormantTime[eid] - interfaces::globals->realtime) * (255.f / fadespeed), 0, 255);
	}
	else // you don't need an if here since its either true or false
	{
		lastNonDormantTime[eid] = interfaces::globals->realtime + fadespeed;
		alpha[eid] = clamp((interfaces::globals->realtime - lastDormantTime[eid]) * (255.f / fadespeed), 0, 255);
	}

	weapon_t* weapon = reinterpret_cast<weapon_t*>(entity->active_weapon());

	if (!weapon)
		return;

	int weapon_ammo = weapon->clip1_count();

	if (weapon_ammo < 0)
		return;

	int weapon_ammo_bar = weapon_ammo * bbox.w / weapon->get_weapon_data()->weapon_max_clip;

	render::draw_filled_rect(bbox.x - 1, bbox.y + bbox.h + 3, bbox.w + 2, 4, color(0, 0, 0, alpha[eid]));
	render::draw_filled_rect(bbox.x, bbox.y + bbox.h + 4, weapon_ammo_bar, 2, color(255, 0, 0, alpha[eid]));

	if (!(weapon_ammo <= weapon->get_weapon_data()->weapon_max_clip / 4)) return;

	render::text((bbox.x + 10) + weapon_ammo_bar - 6, bbox.y + bbox.h - 2, render::fonts::watermark_font, std::to_string(weapon_ammo), true, color(255, 255, 255, alpha[eid]));

}

void visuals::player::draw_info(player_t* entity, visuals::player::box bbox) {

	static int offset;
	offset = 0;

	float alpha[65] = { 255 };

	static float lastDormantTime[65] = { 0 }, lastNonDormantTime[65] = { 0 }; // 64 maxplayers so 1-65 as entindex
	float fadespeed = 1.0; // adding this so you can modify it, fadeout speed in seconds 1.0 = 1s 0.5 = 0.5s etc
	int eid = entity->index(); // calling it once would be a better option
	if (entity->dormant())
	{
		lastDormantTime[eid] = interfaces::globals->realtime;
		alpha[eid] = clamp((lastNonDormantTime[eid] - interfaces::globals->realtime) * (255.f / fadespeed), 0, 255);
	}
	else // you don't need an if here since its either true or false
	{
		lastNonDormantTime[eid] = interfaces::globals->realtime + fadespeed;
		alpha[eid] = clamp((interfaces::globals->realtime - lastDormantTime[eid]) * (255.f / fadespeed), 0, 255);
	}


	player_info_t info;
	interfaces::engine->get_player_info(entity->index(), &info);

	if (variables::draw_bot_check && info.fakeplayer) {
		render::text(bbox.x + bbox.w + 5, bbox.y, render::fonts::watermark_font, "bot", false, color(255, 255, 255, alpha[eid]));
		offset += 11;
	}

	if (variables::draw_zoom_check) {
		if (entity->is_scoped()) {
			render::text(bbox.x + bbox.w + 5, bbox.y + offset, render::fonts::watermark_font, "zoom", false, color(255, 255, 255, alpha[eid]));
			offset += 11;
		}
	}

	if (variables::draw_money) {
		std::string money = std::to_string(entity->money()) + "$";
		render::text(bbox.x + bbox.w + 5, bbox.y + offset, render::fonts::watermark_font, money, false, color(255, 200, 0, alpha[eid]));
		offset += 11;
	}

	if (variables::draw_hk) {

		if (entity->armor() > 0 && !entity->has_helmet())
			render::text(bbox.x + bbox.w + 5, bbox.y + offset, render::fonts::watermark_font, "k", false, color(255, 255, 255, alpha[eid]));
		else if (entity->armor() > 0 && entity->has_helmet())
			render::text(bbox.x + bbox.w + 5, bbox.y + offset, render::fonts::watermark_font, "hk", false, color(255, 255, 255, alpha[eid]));
	}

	if (variables::draw_is_flashed && entity->not_flashed() > 75.0) {
		render::text(bbox.x + (bbox.w / 2), bbox.y - 25, render::fonts::watermark_font, "flashed", true, color(255, 255, 255, alpha[eid]));
	}
}

std::string clean_item_name(std::string name) {
	std::string Name = name;

	auto weapon_start = Name.find("CWeapon");
	if (weapon_start != std::string::npos)
		Name.erase(Name.begin() + weapon_start, Name.begin() + weapon_start + 7);

	if (Name[0] == '_')
		Name.erase(Name.begin());

	if (Name[0] == 'C') //optional for dropped weapons - designer
		Name.erase(Name.begin());

	return Name;
}

void visuals::player::grenades_draw(entity_t* entity) {

	if (!variables::draw_projectile)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player)
		return;

	if (!entity)
		return;

	vec3_t origin = entity->origin(), w2s;

	if (math::world_to_screen_vec3(origin, w2s)) {

		auto class_id = entity->client_class()->class_id;

		switch (class_id) {

		case cbasecsgrenadeprojectile: {

			const model_t* model = entity->model();

			if (!model)
				return;

			studio_hdr_t* hdr = interfaces::model_info->get_studio_model(model);

			if (!hdr)
				return;

			std::string name = hdr->name_char_array;

			if (name.find("incendiarygrenade") != std::string::npos || name.find("fraggrenade") != std::string::npos) {

				render::text(w2s.x, w2s.y, render::fonts::watermark_font, "FRAG", true, color(255, 255, 255));

				break;
			}

			render::text(w2s.x, w2s.y, render::fonts::watermark_font, "FLASH", true, color(255, 255, 255));

		}
									 break;
		case cmolotovprojectile:
		case cinferno: {

			render::text(w2s.x, w2s.y, render::fonts::watermark_font, "FIRE", true, color(255, 255, 255));

		}
					 break;
		case csmokegrenadeprojectile: {
			render::text(w2s.x, w2s.y, render::fonts::watermark_font, "SMOKE", true, color(255, 255, 255));
		}
									break;
		case cdecoyprojectile: {
			render::text(w2s.x, w2s.y, render::fonts::watermark_font, "DECOY", true, color(255, 255, 255));
		}
							 break;
		default: break;
		}
	}
}

void visuals::player::player_esp_render(player_t* entity) {

	if (!entity
		|| !csgo::local_player
		|| !entity->is_alive()
		|| entity == csgo::local_player
		|| entity->team() == csgo::local_player->team())
		return;

	box bbox;
	if (!visuals::player::get_playerbox(entity, bbox))
		return;

	visuals::player::radar_spot(entity);

	if (variables::visible_check && csgo::local_player->can_see_player_pos(entity, entity->get_hitbox_position(hitbox_chest))) {

		draw_name(entity, bbox);

		draw_info(entity, bbox);

		draw_box(entity, bbox);

		draw_ammo(entity, bbox);

		draw_weapon(entity, bbox);

		draw_hp(entity, bbox);

	}
	else if (!variables::visible_check) {

		draw_name(entity, bbox);

		draw_info(entity, bbox);

		draw_box(entity, bbox);

		draw_ammo(entity, bbox);

		draw_weapon(entity, bbox);

		draw_hp(entity, bbox);

	}
}

void visuals::player::player_loop()
{
	for (int i = 1; i <= interfaces::globals->max_clients; ++i) {

		player_t* entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		player_esp_render(entity);

		radar_spot(entity);
	}
}

void visuals::player::world_loop() {

	for (int i = 1; i <= interfaces::entity_list->get_highest_index(); ++i) {

		entity_t* entity = reinterpret_cast<entity_t*>(interfaces::entity_list->get_client_entity(i));

		grenades_draw(entity);
	}
}

void visuals::client::indicators(c_usercmd* cmd) {
	if (!variables::indicators)
		return;
	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;
	auto net_channel = interfaces::engine->get_net_channel_info();
	// https://imgur.com/qwdJduD
	// see how much we fakelagging - make it so its dark green going from 5 to 10 ticks - green from 10 to 15 - 0, 0, 0, 230 from 0 to 5 ticks
	if (variables::fake_lag_enable) {
		if (!net_channel)
			return;

		int packets = net_channel->choked_packets;
		std::string packet_count = std::format("{}", packets);

		if (packets <= 2)
			render::text(10, 650, render::fonts::watermark_font, packet_count, false, color::red());
		else
			render::text(10, 650, render::fonts::watermark_font, packet_count, false, color::green());

		render::draw_filled_rect(10, 665, 50, 3, color(0, 0, 0, 255));
		render::draw_filled_rect(10, 665, 2.7 * packets, 3, color(0, 255, 0, 255));
	}
	else
		render::text(10, 650, render::fonts::watermark_font, "0", false, color::red());

	if (!csgo::send_packet) {
		render::text(10, 640, render::fonts::watermark_font, "send packet", false, color::green());
	}
	else
		render::text(10, 640, render::fonts::watermark_font, "send packet", false, color::red());
}

void visuals::client::fov_changer() {
	if (!variables::fov_changer)
		return;

	// is connected to the game and is in game
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;
	// is alive and local player
	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;
	// viewmodel from local viemodel
	const auto view_model = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(csgo::local_player->view_model()));
	// if viewmodel is false, return
	if (!view_model)
		return;
	// vecs for xy n forward + bunch of math
	vec3_t forward{}, right{}, up{};
	math::angle_vectors(view_model->abs_angles(), &forward, &right, &up);
	// do the ting  G
	view_model->set_abs_origin(view_model->abs_origin() + forward * variables::z_fov + up * variables::y_fov + right * variables::x_fov);
}

void visuals::world::ambient_light() {
	if (variables::ambient_enabled)
	{
		interfaces::console->get_convar("mat_ambient_light_r")->set_value(variables::ambientcolorr);
		interfaces::console->get_convar("mat_ambient_light_g")->set_value(variables::ambientcolorg);
		interfaces::console->get_convar("mat_ambient_light_b")->set_value(variables::ambientcolorb);
	}
}