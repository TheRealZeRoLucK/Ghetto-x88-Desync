#include "../features.hpp"

int random2(int min, int max) {
	static bool first = true;
	if (first) {
		srand(time(NULL));
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}

void antiaim::fakelag::do_fakelag(bool& send_packet, c_usercmd* cmd)
{
	if (!variables::antiaim_enable)
		return;

	if (!variables::fake_lag_enable)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	weapon_t* active_weapon = csgo::local_player->active_weapon();
	if (!active_weapon) return;

	if (variables::disable_fl_onshot_c4) {
		// if shooting or holding c4 disable fakelag
		if (cmd->buttons & in_attack || active_weapon->is_knife() && cmd->buttons & in_attack2 || active_weapon->is_c4())
			return;
	}

	if (variables::fake_lag_enable)
	{
		switch (variables::fake_lag_type)
		{
		case 1: send_packet = interfaces::clientstate->net_channel->choked_packets >= variables::fake_lag_amount;
			break;
		case 2:  
#ifndef _DEBUG
			send_packet = interfaces::clientstate->net_channel->choked_packets >= random2(0, 13);
#endif
#ifdef _DEBUG
			send_packet = interfaces::clientstate->net_channel->choked_packets >= random2(0, 18);
#endif
			break;
		default: break;
		}
	}
	else
		send_packet = interfaces::clientstate->net_channel->choked_packets >= 1;
}