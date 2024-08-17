#include "../features.hpp"

void antiaim::animation::snyc_local_animations(c_usercmd* cmd, bool& _send_packet) {
	static float spawn_time = 0.f;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	if (spawn_time != csgo::local_player->spawn_time())
	{
		spawn_time = csgo::local_player->spawn_time();
		for (int i = 0; i < 13; i++)
		{
			if (i == ANIMATION_LAYER_FLINCH ||
				i == ANIMATION_LAYER_FLASHED ||
				i == ANIMATION_LAYER_WHOLE_BODY ||
				i == ANIMATION_LAYER_WEAPON_ACTION ||
				i == ANIMATION_LAYER_WEAPON_ACTION_RECROUCH)
			{
				continue;
			}
			auto& l = *csgo::local_player->get_anim_layer(i);
			if (!&l)
				continue;
			l.reset();
		}
	}

	if (!csgo::local_player->get_anim_state())
		return;

	csgo::update_animations = false;
	csgo::local_player->update_client_side_animations();
	csgo::local_player->get_anim_state()->goal_feet_yaw = csgo::fake_angle.y;
	csgo::update_animations = true;
}

void antiaim::animation::update_fake_animations() {

	anim_layer backup_layers[14];
	memcpy(backup_layers, csgo::local_player->anim_overlays(), sizeof(anim_layer) * 14);

	if (csgo::local_player->get_anim_state()->last_client_side_animation_update_framecount == interfaces::globals->frame_count)
		csgo::local_player->get_anim_state()->last_client_side_animation_update_framecount -= 1;

	if (csgo::local_player->get_anim_state()->last_client_side_animation_update_time == interfaces::globals->cur_time)
		csgo::local_player->get_anim_state()->last_client_side_animation_update_time += ticks_to_time(1);

	csgo::local_player->set_angles({ 0, csgo::real_angle.y, 0 });
	csgo::local_player->get_anim_state()->goal_feet_yaw = csgo::real_angle.y;
	csgo::local_player->update_client_side_animations();
	csgo::local_player->invalidate_bone_cache();
	csgo::local_player->setup_bones(fake_matrix, MAXSTUDIOBONES, 0x7FF00, interfaces::globals->cur_time);

	memcpy(csgo::local_player->anim_overlays(), &backup_layers, sizeof(anim_layer) * 14);
}