#include "menu.hpp"

std::vector<std::string> data_center_list = { "", "syd", "vie", "gru", "scl", "dxb", "par", "fra", "hkg",
"maa", "bom", "tyo", "lux", "ams", "limc", "man", "waw", "sgp", "jnb",
"mad", "sto", "lhr", "atl", "eat", "ord", "lax", "mwh", "okc", "sea", "iad" };

std::vector<std::string> region = { "[OFF]", "[Australia]", "[Austria]", "[Brazil]", "[Chile]", "[Dubai]", "[France]", "[Germany]", "[Hong Kong]", "[India (Chennai)]", "[India (Mumbai)]", "[Japan]", "[Luxembourg]",
   "[Netherlands]", "[Peru]", "[Philipines]", "[Poland]", "[Singapore]", "[South Africa]", "[Spain]", "[Sweden]", "[UK]", "[USA (Atlanta)]", "[USA (Seattle)]", "[USA (Chicago)]", "[USA (Los Angeles)]", "[USA (Moses Lake)]",
   "[USA (Oklahoma)]", "[USA (Seattle)]", "[USA (DC)]" };

static std::string* force_relay_cluster_value = *(std::string**)(utilities::pattern_scan("steamnetworkingsockets.dll", "B8 ? ? ? ? B9 ? ? ? ? 0F 43") + 1);

static int item_count = 1;
static int item_countx3 = 12;
void text(int x, int* y, std::string text, text_type type, int alpha = 255) {
	if (menu::menu_locked)
		alpha *= 0.80f;
	color text_color;
	switch (type) {
	case info: text_color = color::yellow(alpha); break;
	case regular: text_color = color::white(alpha); break;
	case enabled: text_color = color::blue(alpha); break;
	case enabled_green: text_color = color::green(alpha); break;
	case warning: text_color = color::red(alpha); break;
	case extra:	text_color = color::turquoise(alpha); break;
	default: text_color = color::white(alpha); break;
	}
	render::text(x, *y, render::fonts::gui_font, text, false, text_color);
	*y += 15;
}

void checkbox(int x, int* y, std::string text, bool* option, int key, bool special = false, int alpha = 255, int item_counts = 1) {
	if (menu::menu_locked)
		alpha *= 0.f;
	render::text(x, *y, render::fonts::gui_font, text, false, color::white(alpha));
	
	if (item_counts == item_count && !menu::menu_locked && GetAsyncKeyState(key) & 1)
		*option = !(*option);
	render::text(x + 100, *y, render::fonts::gui_font, *option ? "[X]" : "[ ]", false, *option ? (special ? color::red(alpha) : color::blue(alpha)) : color::white(alpha));
	*y += 15;
}

// no idea what kind of design i want here, so for now imma go with if the id is on the button the button will change to a more grey or light yellow color and when pressed its blue
void button(int x, int* y, std::string text, std::string buttonlabel, bool option, int key, int alpha = 255, int item_counts = 1) {
	if (menu::menu_locked)
		alpha *= 0.f;

	bool special = true;

	if (item_counts == item_count && !menu::menu_locked && GetAsyncKeyState(key) & 1)
		option = true;

	if (item_count == item_counts) {
		render::text(x, *y, render::fonts::gui_font, text, false, color::turquoise(alpha));
		render::text(x + 100, *y, render::fonts::gui_font, *"[" + buttonlabel + "]", false, option ? (color::blue(alpha)) : color::turquoise(alpha));
	}
	else {
		render::text(x, *y, render::fonts::gui_font, text, false, color::white(alpha));
		render::text(x + 100, *y, render::fonts::gui_font, *"[" + buttonlabel + "]", false, option ? (color::blue(alpha)) : color::white(alpha));
	}	
	*y += 15;
}

bool ex_button(int x, int* y, std::string text, std::string buttonlabel, int key, int alpha = 255, int item_counts = 1) {
	bool option = false;

	button(x, y, text, buttonlabel, option, key, alpha = 255, item_counts);
	return true;
}

void combo(int x, int* y, std::string text, int* option, std::vector<std::string> aliases, int key, bool special = false, int alpha = 255, int item_counts = 1) {
	if (menu::menu_locked)
		alpha *= 0.f;

	render::text(x, *y, render::fonts::gui_font, text, false, color::white(alpha));
	if (item_counts == item_count && GetAsyncKeyState(key) & 1 && !menu::menu_locked) {
		if (*option < aliases.size())
			*option += 1;
		if (*option >= aliases.size())
			*option = 0;
	}
	render::text(x + 100, *y, render::fonts::gui_font, aliases.at(*option), false, *option != 0 ? (special ? color::turquoise(alpha) : color::blue(alpha)) : color::white(alpha));
	*y += 15;
}

void int_slider(int x, int* y, std::string text, int& option, int min_value = 0, int max_value = 0, int item_counts = 1) {
	// logika: jeigu focus = true, enablins mum naudoti left/right arrow keys, kai pvz arrowkey left pressed addins +1 arba +5 prie int...
	int alpha = 255;
	if (menu::menu_locked)
		alpha *= 0.f;

	static bool focus = false;
	render::text(x, *y, render::fonts::gui_font, text, false, color::white(alpha));
	if (item_counts == item_count && !menu::menu_locked) {
		if (GetAsyncKeyState(VK_LEFT))
			option -= 1;
		else if (GetAsyncKeyState(VK_RIGHT))
			option += 1;
	}

	option = std::clamp(option, min_value, max_value);

	render::text(x + 100, *y, render::fonts::gui_font, (std::stringstream{ } << std::setprecision(3) << option).str(), false, color::white(alpha));
	*y += 15;
}

void float_slider(int x, int* y, std::string text, float& option, float min_value = 0.f, float max_value = 0.f, int item_counts = 1) {
	// logika: jeigu focus = true, enablins mum naudoti left/right arrow keys, kai pvz arrowkey left pressed addins +1 arba +5 prie int...
	int alpha = 255;
	if (menu::menu_locked)
		alpha *= 0.f;

	static bool focus = false;
	render::text(x, *y, render::fonts::gui_font, text, false, color::white(alpha));
	if (item_counts == item_count && !menu::menu_locked) {
		if (GetAsyncKeyState(VK_LEFT))
			option -= 0.1f;
		else if (GetAsyncKeyState(VK_RIGHT))
			option += 0.1f;
	}

	option = std::clamp(option, min_value, max_value);

	render::text(x + 100, *y, render::fonts::gui_font, (std::stringstream{ } << std::setprecision(3) << option).str(), false, color::white(alpha));
	*y += 15;
}

void menu::render() {
	int screen[2];
	interfaces::engine->get_screen_size(screen[0], screen[1]);
	// two x's and y's, why not?
	// could do:
	// (imma reinvent this shit)
	// int x[3] = 290, 460, 630; 
	//
	int x = 290, x2 = 460, x3 = 630, x4 = 800, x5 = 970, x6 = 1140;
	int y = 40, y2 = 40, y3 = 40, y4 = 40, y5 = 40, y6 = 40;

	static std::string name = "";
	if (csgo::local_player) {
		player_info_t player_info;
		interfaces::engine->get_player_info(interfaces::engine->get_local_player(), &player_info);
		name = player_info.name;
	}

	std::string fps_display = " fps:" + std::to_string(utilities::get_fps());
	text(x, &y, "Hello [ " + name + " ]", text_type::info);
#ifndef _DEBUG
	text(x, &y, "User build v0.01" + fps_display, text_type::extra);
	y += 10;
#endif
#ifdef _DEBUG
	text(x, &y, "Developer build v0.84" + fps_display, text_type::warning);
	y += 10;
#endif

	if (GetAsyncKeyState(VK_INSERT) & 1)
		menu_locked = !menu_locked;

	//
	// NOTE BEFORE RELEASE 2023 09 29:
	// IF YOU WANT TO ADD A FEATURE TO THIS MENU, YOU HAVE TO ADD AN ID + 1 FOR EXAMPLE THERES
	// 73 FEATURES NOW, WHEN YOU ADD A FEATURE DO 73 + 1 = 74, YOU GOTTA CHANGE UP THE ID STUFF FOR THE ARROW TO WORK (SIMPLE MATH STUFF)

	int alpha = 255;
	if (menu_locked)
		alpha *= 1.f;
	// respectfully scaring the hoes since day one lmao
	if (!menu_locked) {
		if (item_count < 16) {
			render::text(x - 25, y - 15 + (item_count) * 15, render::fonts::gui_font, "-->", false, color::green(alpha));
		}
		else if (item_count > 15 && item_count < 31) {
			render::text(x2 - 25, y2 - 150 + (item_count) * 15, render::fonts::gui_font, "-->", false, color::green(alpha));
		}
		else if (item_count > 30 && item_count < 44) {
			render::text(x3 - 25, y3 - 465 + (item_count) * 15, render::fonts::gui_font, "-->", false, color::green(alpha));
		}
		else if (item_count > 42 && item_count < 55) {
			render::text(x4 - 25, y4 - 660 + (item_count) * 15, render::fonts::gui_font, "-->", false, color::green(alpha));
		}
		else if (item_count > 53 && item_count < 74) {
			render::text(x5 - 25, y5 - 825 + (item_count) * 15, render::fonts::gui_font, "-->", false, color::green(alpha));
		}
		//else if (item_count > 66 && item_count < 71) {
		//	render::text(x6 - 25, y6 - 960 + (item_count) * 15, render::fonts::gui_font, "-->", false, color::green(alpha));
		//}

		if (GetAsyncKeyState(VK_DOWN) & 1)
			item_count += 1;

		if (GetAsyncKeyState(VK_UP) & 1)
			item_count -= 1;

		if (item_count > 73)
			item_count = 1;

		if (item_count < 1)
			item_count = 73;
	}

#ifdef _DEBUG
	//std::string itemcount = std::format("{}", item_count);
	//debug::quickprint(itemcount.c_str());
#endif

	// aimbot
	combo(x, &y, "Aimbot config", &variables::aimbot_config, std::vector<std::string>{ "[OFF]", "[Pistol]", "[Rifle]", "[Sniper]", "[HvH Sniper]" }, VK_RETURN, false, 255, 1);
	checkbox(x, &y, "Aimbot enable", &variables::aim_enabled, VK_RETURN, false, 255, 2);
	combo(x, &y, "Hitbox", &variables::aim_mode, std::vector<std::string>{ "[OFF]", "[Hitbox]", "[Nearest]" }, VK_RETURN, false, 255, 3);
	checkbox(x, &y, "Dynamic", &variables::aim_distance_based_fov, VK_RETURN, false, 255, 4);
	checkbox(x, &y, "Silent", &variables::aim_silent, VK_RETURN, false, 255, 5);
	checkbox(x, &y, "Scope", &variables::scope_aim, VK_RETURN, false, 255, 6);
	checkbox(x, &y, "Smoke", &variables::smoke_check, VK_RETURN, false, 255, 7);
	checkbox(x, &y, "Friendly", &variables::aim_team_check, VK_RETURN, false, 255, 8);
	combo(x, &y, "Bone hitbox", &variables::aim_bone, std::vector<std::string>{ "[OFF]", "[Head]", "[Neck]", "[Chest]", "[Stomach]", "[Pelvis]" }, VK_RETURN, false, 255, 9);
	float_slider(x, &y, "Fov", variables::aim_fov, 0.f, 180.f, 10);
	float_slider(x, &y, "Smooth", variables::aim_smooth, 1.f, 10.f, 11);
	float_slider(x, &y, "Rcs x", variables::rcs_x, 0.f, 1.f, 12);
	float_slider(x, &y, "Rcs y", variables::rcs_y, 0.f, 1.f, 13);
	int_slider(x, &y, "Delay", variables::aimbot_delay_after_kill, 0, 20, 14);
	checkbox(x, &y, "Auto shoot", &variables::auto_shoot, VK_RETURN, false, 255, 15);

	// antiaim
	//interfaces::engine->get_view_angles(csgo::real_angles);
	float angle_y = 0; // csgo::real_angles.y;
	text(x2, &y2, "FOV angle Y: " + std::to_string(angle_y), text_type::info);

	//interfaces::engine->get_view_angles(csgo::real_angles);
	float angle_x = 0; // csgo::real_angles.x;
	text(x2, &y2, "FOV angle X: " + std::to_string(angle_x), text_type::info);

	float fake_angle_y = csgo::real_angle.y;
	text(x2, &y2, "Fake angle Y: " + std::to_string(fake_angle_y), text_type::info);

	float fake_angle_x = csgo::real_angle.x;
	text(x2, &y2, "Fake angle X: " + std::to_string(fake_angle_x), text_type::info);

	float desync_angle_y = csgo::fake_angle.y;
	text(x2, &y2, "Desync angle Y: " + std::to_string(desync_angle_y), text_type::info);

	float desync_angle_x = csgo::fake_angle.x;
	text(x2, &y2, "Desync angle X: " + std::to_string(desync_angle_x), text_type::info);

	checkbox(x2, &y2, "Anti aim", &variables::antiaim_enable, VK_RETURN, false, 255, 16);
	combo(x2, &y2, "Pitch", &variables::pitch_aa, std::vector<std::string>{ "[OFF]", "[Up]", "[Down]", "[Minimal]", "[Random]" }, VK_RETURN, false, 255, 17);
	combo(x2, &y2, "Yaw", &variables::yaw_aa, std::vector<std::string>{ "[OFF]", "[Backwards]", "[Spin]", "[Static]" }, VK_RETURN, false, 255, 18);
	combo(x2, &y2, "Yaw jitter", &variables::yaw_jitter_aa, std::vector<std::string>{ "[OFF]", "[Offset]", "[Random]" }, VK_RETURN, false, 255, 19);
	combo(x2, &y2, "Body yaw", &variables::body_yaw_aa, std::vector<std::string>{ "[OFF]", "[Static]", "[Opposite]", "[Jitter]" }, VK_RETURN, false, 255, 20);
	float_slider(x2, &y2, "Backwards angles", variables::backwards_deg, -180.f, 180.f, 21);
	float_slider(x2, &y2, "Static angles", variables::static_deg, -180.f, 180.f, 22);
	float_slider(x2, &y2, "Jitter angles", variables::jitter_deg, -180.f, 180.f, 23);
	checkbox(x2, &y2, "Desync", &variables::desync_enable, VK_RETURN, false, 255, 24);
	combo(x2, &y2, "Desync", &variables::desync_aa, std::vector<std::string>{ "[OFF]", "[Switch]", "[Freestand]", "Side switch"}, VK_RETURN, false, 255, 25);
	checkbox(x2, &y2, "Fakelag enable", &variables::fake_lag_enable, VK_RETURN, false, 255, 26);
	checkbox(x2, &y2, "Disable onshot-c4", &variables::disable_fl_onshot_c4, VK_RETURN, false, 255, 27);
#ifndef _DEBUG
	int_slider(x2, &y2, "Fakelag amount", variables::fake_lag_amount, 0, 13, 28);
#endif
#ifdef _DEBUG // NOTE BEFORE RELEASE 2023 09 29: HAD THIS ON 18 TICKS AND AS FAR AS I KNOW THERE WAS A BYPASS OR SOMETHING SO IT WOULD WORK WITHOUT BUGGING, NEVER ADDED THAT
	int_slider(x2, &y2, "Fakelag amount", variables::fake_lag_amount, 0, 18, 28);
#endif
	combo(x2, &y2, "Fakelag type", &variables::fake_lag_type, std::vector<std::string>{ "[OFF]", "[Normal]", "[Random]" }, VK_RETURN, false, 255, 29);
	checkbox(x2, &y2, "Legit", &variables::legit_aa, VK_RETURN, false, 255, 30);

	// visuals - render
	checkbox(x3, &y3, "Visible check", &variables::visible_check, VK_RETURN, false, 255, 31);
	checkbox(x3, &y3, "Visible only", &variables::radar_spotted, VK_RETURN, false, 255, 32);
	checkbox(x3, &y3, "Box", &variables::draw_box, VK_RETURN, false, 255, 33);
	checkbox(x3, &y3, "Name", &variables::draw_name, VK_RETURN, false, 255, 34);
	checkbox(x3, &y3, "Weapon", &variables::draw_weapon, VK_RETURN, false, 255, 35);
	checkbox(x3, &y3, "Ammo", &variables::draw_ammo, VK_RETURN, false, 255, 36);
	checkbox(x3, &y3, "HP", &variables::draw_hp, VK_RETURN, false, 255, 37);
	checkbox(x3, &y3, "Armor", &variables::draw_hk, VK_RETURN, false, 255, 38);
	checkbox(x3, &y3, "Money", &variables::draw_money, VK_RETURN, false, 255, 39);
	checkbox(x3, &y3, "Bot check", &variables::draw_bot_check, VK_RETURN, false, 255, 40);
	checkbox(x3, &y3, "Zoom check", &variables::draw_zoom_check, VK_RETURN, false, 255, 41);
	checkbox(x3, &y3, "Flash check", &variables::draw_is_flashed, VK_RETURN, false, 255, 42);
	checkbox(x3, &y3, "Projectiles", &variables::draw_projectile, VK_RETURN, false, 255, 43);
	// chams
	checkbox(x4, &y4, "Chams enable", &variables::chams_enable, VK_RETURN, false, 255, 44);
	checkbox(x4, &y4, "Visible", &variables::ememy_chams, VK_RETURN, false, 255, 45);
	checkbox(x4, &y4, "Hidden", &variables::chams_xqz, VK_RETURN, false, 255, 46);
	checkbox(x4, &y4, "Wireframe", &variables::wireframe_chams, VK_RETURN, false, 255, 47);
	combo(x4, &y4, "Materials", &variables::chams_type, std::vector<std::string>{ "[Regular]", "[Flat]", "[Ghost]", "[OT overlay]", "[Pearlescent]", "[Animated]", "[Water test]", "[Test]" }, VK_RETURN, false, 255, 48);
	checkbox(x4, &y4, "Team enable", &variables::team_check_chams, VK_RETURN, false, 255, 49);
	checkbox(x4, &y4, "Local enable", &variables::local_chams, VK_RETURN, false, 255, 50);
	checkbox(x4, &y4, "Sleeves", &variables::sleeve_chams, VK_RETURN, false, 255, 51);
	checkbox(x4, &y4, "Arms", &variables::arms_chams, VK_RETURN, false, 255, 52);
	checkbox(x4, &y4, "Weapons", &variables::weapon_chams, VK_RETURN, false, 255, 53);
	checkbox(x4, &y4, "Desync", &variables::desync_chams, VK_RETURN, false, 255, 54);

	// misc
	checkbox(x5, &y5, "Bhop ", &variables::bhop, VK_RETURN, false, 255, 55);
	checkbox(x5, &y5, "FOV enable", &variables::fov_changer, VK_RETURN, false, 255, 56);
	int_slider(x5, &y5, "X fov", variables::x_fov, -50, 50, 57);
	int_slider(x5, &y5, "Y fov", variables::y_fov, -50, 50, 58);
	int_slider(x5, &y5, "Z fov", variables::z_fov, -50, 50, 59);

	if (variables::mmregionenable && !interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		*force_relay_cluster_value = data_center_list[variables::mmregion];
	checkbox(x5, &y5, "Enable region", &variables::mmregionenable, VK_RETURN, false, 255, 60);
	combo(x5, &y5, "Region", &variables::mmregion, region, VK_RETURN, false, 255, 61);
	checkbox(x5, &y5, "Knife enable", &variables::knife_enabled, VK_RETURN, false, 255, 62);
	combo(x5, &y5, "Model ", &variables::knife_model, std::vector<std::string>{ "[Default]", "[Bayonet]", "[Flip]", "[Gut]", "[Karambit]", "[M9 Bayonet]", "[Huntsman]", "[Falchion]", "[Bowie]", "[Butterfly]", "[Daggers]", "[Ursus]", "[Navaja]", "[Stiletto]", "[Talon]", "[Nomad]", "[Skeleton]", "[Survival]", "[Paracord]", "[Classic]" }, VK_RETURN, false, 255, 63);
	checkbox(x5, &y5, "Thirdperson [M]", &variables::thirdperson, VK_RETURN, false, 255, 64);
	int_slider(x5, &y5, "Distance", variables::thirdperson_dist, 0, 200, 65);
	checkbox(x5, &y5, "Force crosshair", &variables::force_crosshair, VK_RETURN, false, 255, 66);
	checkbox(x5, &y5, "Backtrack", &variables::backtrack_alt, VK_RETURN, false, 255, 67);
	int_slider(x5, &y5, "Backtrack time", variables::backtrack_time, 1.f, 400.f, 68);
	checkbox(x5, &y5, "Fake latency", &variables::fakelatency_enable, VK_RETURN, false, 255, 69);
	float_slider(x5, &y5, "Latency range", variables::fakelatency, 0.f, 1000.f, 70);
	checkbox(x5, &y5, "Backtrack chams", &variables::backtrack_chams, VK_RETURN, false, 255, 71);

	// NOTE BEFORE RELEASE 2023 09 29: NEVER FINISHED THE BUTTON SINCE I RUSHED WITH FIXING FEATURES BEFORE THE CS2 RELEASED
	// EVEN THO I RUSHED FIXING OTHER FEATURES, IVE NEVER FIXED CHAMS FOR ARMS/WEAPONS SO TAKE THAT IN MIND

	if (ex_button(x5, &y5, "Save config", "Default", VK_RETURN, 255, 72)) {
	//	config::save("default.cfg");
	}
	if (ex_button(x5, &y5, "Load config", "Default", VK_RETURN, 255, 73)) {
	//	config::load("default.cfg");
	}
	//button(x5, &y5, "Save config", "Default", variables::save_config, VK_RETURN, 255, 72);
	//button(x5, &y5, "Load config", "Default", variables::load_config, VK_RETURN, 255, 73);

	//if (!variables::save_config) {
	//	config::save("default.cfg");
	//}

	//if (!variables::load_config) {
	//	config::load("default.cfg");
	//}

	//
	// I THINK THAT EVERY CHEAT NEEDS A CRINGEY NAME
	// AND OFC CANT FORGET THE CRINGEY PRINT TO CONSOLE ASCII ART + "LOADED" STUFF
	// 
	//
	//
	//                      .-        -.
	//                     /            \
    //                    |              |
	//                    |,  .-.  .-.  ,|
	//                    | )(__/  \__)( |
	//                    |/     /\     \|
	//          (@_       (_     ^^     _)
	//     _     ) \_______\__|IIIIII|__/__________________________
	//    (_)@8@8{}<________|-\IIIIII/-|___________________________>
	//           )_/        \          /
	//          (@           `--------`
	//
	//                Welcome to The Fundraiser.
	//
	// Interfaces loaded.
	// Hooks loaded.
	// Renderer loaded.
	// Developer mode. <debug> - red till dot, build option - regular color
	// 
	// The Fundraiser v0.84 - red name + green version
	//

	static bool once = []() {
		interfaces::engine->execute_cmd("toggleconsole\n");
		interfaces::engine->execute_cmd("clear\n");
		interfaces::console->console_printf("\n");
		interfaces::console->console_printf("                      .-        -.\n                     /            \\\ \n                    |              |\n                    |,  .-.  .-.  ,|\n                    | )(__/  \\\__)( |\n                    |/     /\\\     \\\|\n          (@_       (_     ^^     _)\n     _     ) \\\_______\\\__|IIIIII|__/__________________________\n    (_)@8@8{}<________|-\\\IIIIII/-|___________________________>\n           )_/        \\\          /\n          (@           `--------`\n");
		interfaces::console->console_printf("\n");
		interfaces::console->console_printf("                Welcome to ");
		interfaces::console->console_printf("The");
		interfaces::console->console_printf(" Fund");
		interfaces::console->console_printf("raiser. \n");
		interfaces::console->console_printf("\n");
		if (variables::cheat_loaded::cheat_status == 3) {
			interfaces::console->console_printf("Interfaces loaded.\n");
			interfaces::console->console_printf("Hooks loaded.\n");
			interfaces::console->console_printf("Renderer loaded.\n");
		}
		else
			interfaces::console->console_printf("fuck...\n");
		interfaces::console->console_printf("\n");
#ifdef _DEBUG
		interfaces::console->console_printf("Developer mode. <debug>\n");
		interfaces::console->console_printf("dev - 18 tick fakelag\n");
		interfaces::console->console_printf("dev - hide information\n");
		interfaces::console->console_printf("\n");
		interfaces::console->console_printf("\n");
		interfaces::console->console_printf("\n");
		interfaces::console->console_printf(" The Fundraiser v0.84\n");
#endif
#ifndef _DEBUG
		interfaces::console->console_printf("User mode.\n");
		interfaces::console->console_printf("\n");
		interfaces::console->console_printf("\n");
		interfaces::console->console_printf("\n");
		interfaces::console->console_printf(" The Fundraiser v0.01\n");
#endif
		return true;
	} ();
}

// ALREADY EXISTING FEATURES 

//
// 
//  checkbox(x2, &y2, "Ambient enable", &variables::ambient_enabled, VK_RETURN, false, 255, 34);
//	float_slider(x2, &y2, "Ambient red", variables::ambientcolorr, 0.f, 100.f, 35);
//	float_slider(x2, &y2, "Ambient green", variables::ambientcolorg, 0.f, 100.f, 36);
//	float_slider(x2, &y2, "Ambient blue", variables::ambientcolorb, 0.f, 100.f, 37);
//
// SAFETY MODE: UNTRUSTED
// 
//  
//