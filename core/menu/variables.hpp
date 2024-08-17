#pragma once

namespace variables {
	namespace cheat_loaded {
		inline int cheat_status = 0;
	}

	inline bool bhop = false;
	inline bool server_hitbox = true;

	inline bool skate = false;		

	inline bool fake_lag_enable = false;
	inline bool disable_fl_onshot_c4 = false;
	inline int fake_lag_amount = 5;
	inline int fake_lag_type = 0;

	inline bool indicators = true;

	inline bool thirdperson = false;
	inline int thirdperson_dist = 130;

	inline bool fov_changer = false;
	inline int x_fov = 0;
	inline int y_fov = -1;
	inline int z_fov = 5;

	inline bool chams_enable = false;
	inline bool chams_xqz = false;
	inline bool ememy_chams = false;
	inline bool team_check_chams = false;
	inline bool local_chams = false;
	inline bool wireframe_chams = false;
	inline bool desync_chams = false;

	inline int chams_type = 0;

	inline bool sleeve_chams = false;
	inline bool arms_chams = false;
	inline bool weapon_chams = false;
	inline bool glove_chams = false;

	inline float f_chams_clr[4] = { 0.5f, 1.f, 0.5f, 1.0f }; // done
	inline color chams_clr = color(255, 105, 180);
	inline float f_chams_local_clr[4] = { 1.f, 0.f, 0.f, 0.6f }; // done
	inline color chams_local_clr = color(255, 105, 180);
	inline float f_chams_local_fake_clr[4] = { 1.f, 0.f, 0.f, 0.6f }; // done
	inline color chams_local_fake_clr = color(255, 105, 180);
	inline float f_chams_team_clr[4] = { 0.37f, 0.41f, 0.75f, 1.0f }; // done
	inline color chams_team_clr = color(255, 105, 180);
	inline float f_sleeve_chams[4] = { 0.78f, 0.25f, 0.78f, 0.6f }; // done
	inline color sleeve_chams_clr = color(255, 105, 255);
	inline float f_arms_chams[4] = { 1.f, 1.f, 1.f, 1.f };
	inline color arms_chams_clr = color(255, 105, 255);
	inline float f_weapon_chams[4] = { 1.f, 1.f, 1.f, 1.f };
	inline color weapon_chams_clr = color(255, 105, 255);
	inline float f_glove_chams[4] = { 1.f, 1.f, 1.f, 1.f };
	inline color glove_chams_clr = color(255, 105, 255);
	inline float f_xqz_clr[4] = { 1.f, 1.f, 0.f, 1.0f }; // done
	inline color xqz_clr = color(255, 105, 255);

	inline float f_desync_chams[4] = { 0.78f, 0.25f, 0.78f, 0.6f }; // done
	inline color desync_chams_clr = color(255, 105, 255);

	inline float f_backtrack_chams[4] = { 0.78f, 0.25f, 0.78f, 0.6f }; // done
	inline color backtrack_chams_clr = color(255, 105, 255);

	inline bool knife_enabled = false;
	inline int knife_model = 0;

	inline bool ambient_enabled = false;
	inline float ambientcolorr = 0.f;
	inline float ambientcolorg = 0.f;
	inline float ambientcolorb = 0.f;

	inline bool aim_enabled = false;
	inline bool scope_aim = false;
	inline bool smoke_check = false;
	inline bool aim_silent = false;
	inline bool aim_distance_based_fov = false;
	inline float aim_fov = 180.0f;
	inline float rcs_x = 0.0f;
	inline float rcs_y = 0.0f;
	inline float aim_smooth = 5.f;
	inline int aim_bone = 0;
	inline int aim_mode = 0;
	inline bool aim_team_check = false;
	inline int aimbot_delay_after_kill = 0;
	inline bool auto_shoot = false;
	inline bool auto_scope = false;
	inline int aimbot_config = 0;

	inline bool remove_scope = false;
	inline bool engine_radar = false;

	inline bool legit_aa = false;
	inline bool break_lby = false;
	inline float next_update = 0.f;

	inline bool antiaim_enable = false;
	inline int pitch_aa = 0;
	inline int yaw_aa = 0;
	inline int yaw_jitter_aa = 0;
	inline int body_yaw_aa = 0;
	inline int lby_target = 0;

	inline float backwards_deg = 180.f;
	inline float static_deg = 0.f;
	inline float jitter_deg = 0.f;

	inline bool freestand_enable = false;

	inline bool desync_enable = false;
	inline int desync_aa = 0;

	inline bool draw_box = false;
	inline bool radar_spotted = false;
	inline bool draw_name = false;
	inline bool draw_hp = false;
	inline bool draw_weapon = false;
	inline bool draw_ammo = false;
	inline bool draw_bot_check = false;
	inline bool draw_money = false;
	inline bool draw_zoom_check = false;
	inline bool draw_hk = false;
	inline bool draw_is_flashed = false;
	inline bool draw_projectile = false;
	inline bool visible_check = false;

	inline bool mmregionenable = false;
	inline int mmregion = 0;

	inline bool force_crosshair = false;

	inline bool nightmode = false;
	inline float nightmode_brightness = 50.f;

	inline int safety = 0;
	inline const bool save_config = false;
	inline const bool load_config = false;

	inline bool backtrack_chams = false;
	inline bool backtrack_alt = false;
	inline int backtrack_time = 0;
	inline bool fakelatency_enable = false;
	inline float fakelatency = 200.f;

	inline bool rage_bhop = true;
}
