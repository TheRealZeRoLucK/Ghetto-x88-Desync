#include "../features.hpp"

int random(int min, int max) {
	static bool first = true;
	if (first) {
		srand(time(NULL));
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}
// yeah wtf is this 💀
using random_float = float(__cdecl*)(float, float);
random_float randomf;

void antiaim::rageaa::ideal_pitch(c_usercmd* cmd) {
	anim_state* state = csgo::local_player->get_anim_state();

	if (!state)
		return;

	cmd->viewangles.x = state->min_pitch;
}

void antiaim::rageaa::update_lowerbody_breaker() {
	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	float speed = csgo::local_player->velocity().length_2d_sqr();

	if (speed > 0.1)
		variables::next_update = csgo::server_time() + 0.22;

	variables::break_lby = false;

	if (variables::next_update <= csgo::server_time()) {
		variables::next_update = csgo::server_time() + 1.1;
		variables::break_lby = true;
	}

	if (!(csgo::local_player->flags() & fl_onground))
		variables::break_lby = false;
}

void antiaim::rageaa::legit_antiaim_run(c_usercmd* cmd, bool& send_packet) {
	if (!variables::legit_aa)
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	if (csgo::local_player->move_type() == movetype_ladder)
		return;

	if (cmd->buttons & in_attack)
		return;

	auto net_channel = interfaces::clientstate->net_channel;

	if (!net_channel)
		return;

	send_packet = net_channel->choked_packets >= 3;

	auto weapon = csgo::local_player->active_weapon();

	if (!weapon || weapon->is_nade())
		return;

	if (GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON) || (GetAsyncKeyState(0x45)))
		return;

	if (variables::break_lby) {
		send_packet = false;
		cmd->viewangles.y += 120.f;
	}
	else if (!send_packet)
		cmd->viewangles.y -= 120.f;

	cmd->viewangles.clamp();

	cmd->viewangles = cmd->viewangles;
}

void antiaim::rageaa::remake_real(c_usercmd* cmd, bool& send_packet) {
	if (!csgo::local_player)
		return;

	if (!variables::antiaim_enable)
		return;

	weapon_t* active_weapon = csgo::local_player->active_weapon();
	if (!active_weapon) return;

	if (cmd->buttons & in_attack ||
		cmd->buttons & in_use ||
		csgo::local_player->move_type() == movetype_ladder ||
		csgo::local_player->move_type() == movetype_noclip ||
		active_weapon->is_knife() && (cmd->buttons & in_attack || cmd->buttons & in_attack2) ||
		active_weapon->is_c4() && cmd->buttons & in_attack)
		return;

	if (active_weapon->is_nade() && !active_weapon->pin_pulled()) {
		float throw_time = active_weapon->throw_time();
		if (throw_time > 0) {
			send_packet = false;
			return;
		}
	}

	switch (variables::pitch_aa) {
	case 1: cmd->viewangles.x -= 89.f; break; // UP
	case 2: cmd->viewangles.x += 89.f; break; // DOWN
	case 3: ideal_pitch(cmd); break; // IDEAL/MINIMAL
	case 4: cmd->viewangles.x += random(-89, 89); break; // RANDOM
	default: break;
	}

	switch (variables::yaw_aa) {
	case 1: cmd->viewangles.y += variables::backwards_deg; break; // BACKWARDS + DEGREE SLIDER
	case 2: cmd->viewangles.y -= random(-360, 360); break; // SPIN
	case 3: cmd->viewangles.y += variables::static_deg; break; // STATIC + DEGREE SLIDER
	}

	switch (variables::yaw_jitter_aa) {
	case 1: cmd->viewangles.y += random(-variables::jitter_deg, variables::jitter_deg); break; // JITTER + DEGREE SLIDER
	case 2: cmd->viewangles.y += random(-360, 360); break; // JITTER RANDOM
	}


	static bool flip = false;
	static bool flip2 = false;
	float flip2angle = 0.f;

	switch (variables::body_yaw_aa) {
	case 1:

		break;
	case 2:
		
		break;
	case 3:
		
		break;
	}

	switch (variables::lby_target) {

	}

	cmd->viewangles.clamp();
}

int antiaim::rageaa::remake_freestand(float yaw) {
	if (!csgo::local_player || !csgo::local_player->is_alive())
		return false;

	float side_right, side_left;

	vec3_t src{}, dst{}, forward{}, right{}, up{};
	trace_t tr{};
	ray_t ray{}, ray1{};
	trace_filter filter{};

	vec3_t angles{};
	angles.x = 0;
	angles.y = yaw;

	math::angle_vectors(angles, &forward, &right, &up);

	filter.skip = csgo::local_player;
	src = csgo::local_player->get_eye_pos();
	dst = src + (forward * 384);

	ray.initialize(src + right * 35, dst + right * 35);

	interfaces::trace_ray->trace_ray(ray, MASK_SHOT, &filter, &tr);

	side_right = (tr.end - tr.start).length();

	ray1.initialize(src - right * 35, dst - right * 35);

	interfaces::trace_ray->trace_ray(ray1, MASK_SHOT, &filter, &tr);

	side_left = (tr.end - tr.start).length();

	static int result = 0;

	if (side_left > side_right)
		result = true;
	else if (side_right > side_left)
		result = false;

	return result;
}

void antiaim::rageaa::remake_desync(c_usercmd* cmd, bool& send_packet) {
	if (!csgo::local_player)
		return;

	if (!variables::desync_enable)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	weapon_t* active_weapon = csgo::local_player->active_weapon();
	if (!active_weapon) return;

	if (cmd->buttons & in_attack ||
		cmd->buttons & in_use ||
		csgo::local_player->move_type() == movetype_ladder ||
		csgo::local_player->move_type() == movetype_noclip ||
		active_weapon->is_knife() && (cmd->buttons & in_attack || cmd->buttons & in_attack2) ||
		active_weapon->is_c4() && cmd->buttons & in_attack)
		return;

	if (active_weapon->is_nade() && !active_weapon->pin_pulled()) {
		float throw_time = active_weapon->throw_time();
		if (throw_time > 0) {
			send_packet = false;
			return;
		}
	}

	auto net_channel = interfaces::clientstate->net_channel;

	if (!net_channel)
		return;

	switch (variables::desync_aa) {
	case 1:
		if (variables::break_lby) {
			if (GetKeyState(VK_MENU))
				cmd->viewangles.y += 120.f;
			else
				cmd->viewangles.y -= 120.f;
		}
		else
			cmd->viewangles;
		break;
	case 2:
		send_packet = net_channel->choked_packets >= 1;

		if (variables::break_lby) {
			cmd->viewangles.y += freestand ? -120.f : 120.f;
			send_packet = false;
		}
		cmd->viewangles.y += send_packet ? 0 : freestand ? -120.f : 120.f;
		break;
	case 3: 
		send_packet = net_channel->choked_packets >= 1;

		cmd->buttons &= ~(in_forward | in_back | in_moveleft | in_moveright);

		bool side = true;

		if (variables::break_lby) {
			cmd->viewangles.y += side ? -120.f : 120.f;
			send_packet = false;
		}

		cmd->viewangles.y += send_packet ? 0 : side ? -120.f : 120.f;
		break;
	}
	cmd->viewangles.clamp();
}

#ifdef _DEBUG
#define sig_player_by_index "85 C9 7E 32 A1"
#define sig_draw_server_hitboxes "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE"

void antiaim::show_hitbox() {
	if (!variables::server_hitbox)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	if (!interfaces::input->b_camera_in_third_person)
		return;

	auto get_player_by_index = [](int index) -> player_t* {
		typedef player_t* (__fastcall* player_by_index)(int);
		static auto player_index = reinterpret_cast<player_by_index>(utilities::pattern_scan("server.dll", sig_player_by_index));

		if (!player_index)
			return false;

		return player_index(index);
	};

	static auto fn = reinterpret_cast<uintptr_t>(utilities::pattern_scan("server.dll", sig_draw_server_hitboxes));
	auto duration = -1.f;
	PVOID entity = nullptr;

	entity = get_player_by_index(csgo::local_player->index());

	if (!entity)
		return;

	__asm {
		pushad
		movss xmm1, duration
		push 0 // 0 - colored, 1 - blue
		mov ecx, entity
		call fn
		popad
	}
}
#endif
