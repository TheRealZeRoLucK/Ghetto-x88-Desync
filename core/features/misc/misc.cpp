#include "../features.hpp"

void misc::movement::bunny_hop(c_usercmd* cmd) {
	if (!variables::bhop)
		return;

	const int move_type = csgo::local_player->move_type();

	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;
  
	if (!(csgo::local_player->flags() & fl_onground))
		cmd->buttons &= ~in_jump;
};

void misc::movement::skate(c_usercmd* cmd) {
	if (!variables::skate)
		return;

	if (csgo::local_player && csgo::local_player->move_type() != movetype_ladder)
		cmd->buttons ^= in_forward | in_back | in_moveleft | in_moveright;
}

void misc::thirdperson() {
	if (GetAsyncKeyState(VK_MBUTTON) & 1)
		variables::thirdperson = !variables::thirdperson;

	if (interfaces::input->b_camera_in_third_person = variables::thirdperson)
		interfaces::input->vec_camera_offset.z = variables::thirdperson_dist;
}

void misc::movement::force_crosshair() {
	static auto weapon_debug_spread_show = interfaces::console->get_convar("weapon_debug_spread_show");
	weapon_debug_spread_show->set_value(variables::force_crosshair && !csgo::local_player->is_scoped() ? 3 : 0);
}

void misc::client::player_list() {

}

static float normalizeyaw(float offset) {
    if (offset > 180.f) {
        offset = (offset - 180.f) + -180.f;
    }
    if (offset < -180) {
        offset = (offset + 180.f) + 180.f;
    }
    return offset;
}
