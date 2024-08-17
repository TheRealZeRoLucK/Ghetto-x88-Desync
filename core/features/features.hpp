#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../menu/variables.hpp"
#include "exploits/backtrack.h"

namespace sequence {
	struct sequence_object_t {
		//sequence_object_t(int iInReliableState, int iOutReliableState, int iSequenceNr, float flCurrentTime)
		//	: iInReliableState(iInReliableState), iOutReliableState(iOutReliableState), iSequenceNr(iSequenceNr), flCurrentTime(flCurrentTime) { }

		int iInReliableState;
		int iOutReliableState;
		int iSequenceNr;
		float flCurrentTime;
	};

	inline std::deque<sequence_object_t> sequences_vector = { };
	inline int last_incoming_sequence = 0;

	float get_estimated_server_time(float latency);
	void update_incoming_sequences(i_net_channel* net_channel);
	void clear_incoming_sequences();
	void add_latency_to_net_channel(i_net_channel* net_channel, float latency);
}

class player_log : public singleton<player_log>
{
public:
	player_log_t& get_log(const int index);
private:
	player_log_t logs[65];

};

struct autowall_data_t {
	bool lethal;
	float damage;
};

namespace aimbot {
	// bunch of checks that will be used in the future
	inline bool autowalling = false;
	namespace autowall {
		float get_damage_multiplier(int hit_group, float hs_multiplier);
		bool is_armored(int hit_group, bool helmet);
		static float handlebulletpenetration(surface_data* enterSurfaceData, trace_t& enterTrace, const vec3_t& direction, vec3_t& result, float penetration, float damage) noexcept;
		//autowall_data_t handle_walls(player_t* local_player, entity_t* entity, const vec3_t& destination, const weapon_info_t* weapon_data, bool enabled_hitbox);
	}
	namespace resolver {
		void resolver(player_t* player);
	}
	namespace legit {
		inline bool attack = false;
		inline int bestindex = -1;

		int get_nearest_bone(player_t* entity, c_usercmd* user_cmd);
		void weapon_settings(weapon_t* weapon);
		int find_target(c_usercmd* user_cmd);

		static inline int aimbone;
		static inline float aim_smooth;
		static inline float aim_fov;
		static inline float rcs_x;
		static inline float rcs_y;
		static inline int hitbox_id;
		static inline vec3_t angle;
		static inline int kill_delay;

		void event_player_death(i_game_event* event);
		void run(c_usercmd* user_cmd);
		void auto_shoot(c_usercmd* cmd);
	}
	namespace rage {
	}
}

namespace antiaim {
	namespace rageaa {
		void ideal_pitch(c_usercmd* cmd);
		void remake_real(c_usercmd* cmd, bool& send_packet);
		void remake_desync(c_usercmd* cmd, bool& send_packet);
		int remake_freestand(float yaw);
		void update_lowerbody_breaker();
		void legit_antiaim_run(c_usercmd* cmd, bool& send_packet);

		inline int freestand = antiaim::rageaa::remake_freestand(csgo::fake_angle.y);
	}

	namespace animation {
		void snyc_local_animations(c_usercmd* cmd, bool& _send_packet);
		void update_fake_animations();

		inline matrix_t fake_matrix[256];
	}

	namespace fakelag {
		void do_fakelag(bool& send_packet, c_usercmd* cmd);
	}
#ifdef _DEBUG
	void show_hitbox();
#endif
}

namespace misc {
	namespace movement {
		void bunny_hop(c_usercmd* cmd);
		void skate(c_usercmd* cmd);
		void force_crosshair();
	};
	void thirdperson();

	namespace client {
		void player_list();
		void untrusted(c_usercmd* cmd);
		void smac();
	}
}

namespace visuals {
	namespace client {
		void indicators(c_usercmd* cmd);
		void fov_changer();
	}

	namespace player {
		struct box {
			int x, y, w, h;
			box() = default;
			box(int x, int y, int w, int h) {
				this->x = x;
				this->y = y;
				this->w = w;
				this->h = h;
			}
		};

		static bool is_checked = false;
		bool get_playerbox(player_t* ent, box& in);
		void draw_box(player_t* entity, visuals::player::box bbox);
		void radar_spot(player_t* entity);
		void draw_info(player_t* entity, visuals::player::box bbox);
		void draw_name(player_t* entity, visuals::player::box bbox);
		void draw_hp(player_t* entity, visuals::player::box bbox);
		void draw_ammo(player_t* entity, visuals::player::box bbox);
		void draw_weapon(player_t* entity, visuals::player::box bbox);
		void grenades_draw(entity_t* entity);
		void player_esp_render(player_t* entity);
		void player_loop();
		void world_loop();
	}

	namespace world {
		void ambient_light();
	}

	namespace chams {
		void dme_chams_run(hooks::dme::fn original, void* thisptr, void* context, void* state, const model_render_info_t& info, matrix_t* custom_bone_to_world);
		void dme_misc(hooks::dme::fn original, void* thisptr, void* context, void* state, const model_render_info_t& info, matrix_t* custom_bone_to_world);
		void custom_materials();
	}
}

namespace skins {
	void run();
	void animfix_hook();
	void animfix_unhook();
}

namespace fakeping {

}