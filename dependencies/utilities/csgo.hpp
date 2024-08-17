#pragma once
#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <intrin.h>
#include <filesystem>

#include "../utilities/singleton.hpp"
#include "../utilities/fnv.hpp"
#include "../utilities/utilities.hpp"
#include "../../dependencies/minhook/minhook.h"
#include "../interfaces/interfaces.hpp"
#include "../../source-sdk/sdk.hpp"
#include "../../core/hooks/hooks.hpp"
#include "../../dependencies/math/math.hpp"
#include "../../dependencies/utilities/renderer/renderer.hpp"
#include "../../dependencies/utilities/console/console.hpp"
#include "../utilities/csgo.hpp"
#include "../utilities/x86RetSpoof.h"
#include "../debug/debug.hpp"
#include "restrictions.hpp"

//interfaces
#define sig_client_state "A1 ? ? ? ? 8B 80 ? ? ? ? C3"
#define sig_directx "A1 ? ? ? ? 50 8B 08 FF 51 0C"
#define sig_input "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10"
#define sig_glow_manager "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00"
#define sig_player_move_helper "8B 0D ? ? ? ? 8B 46 08 68"
#define sig_weapon_data "8B 35 ? ? ? ? FF 10 0F B7 C0"

//misc
#define sig_set_angles "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"
#define sig_prediction_random_seed "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04"

// hooks
#define sig_should_skip_animation_frame "57 8B F9 8B 07 8B ? ? ? ? ? FF D0 84 C0 75 02"
#define sig_do_extra_bone_processing "55 8B EC 83 E4 F8 81 ? ? ? ? ? 53 56 8B F1 57 89 74 24 1C"
#define sig_check_for_sequence_check "55 8B EC 83 E4 F8 81 ? ? ? ? ? 53 56 8B F1 57 89 74 24 1C"
#define sig_do_procedural_foot_plant "55 8B EC 83 E4 F0 83 EC 78 56 8B F1 57 8B"
#define sig_standard_blending_rules "55 8B EC 83 E4 F0 B8 F8 10"
#define sig_update_client_animations "55 8B EC 51 56 8B F1 80 BE ? ? ? ? ? 74 36"
#define sig_update_animation_state "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3"
#define sig_modify_eye_position "55 8B EC 83 E4 F8 83 EC 70 56 57 8B F9 89 7C 24 14"
#define sig_calculate_view "55 8B EC 83 EC 14 53 56 57 FF 75 18"
#define sig_check_for_sequence_change "55 8B EC 51 53 8B 5D 08 56 8B F1 57 85"
#define sig_build_transformations "55 8B EC 83 E4 F0 81 EC ? ? ? ? 56 57 8B F9 8B 0D ? ? ? ? 89 7C 24 28 8B"
#define sig_invalidate_bone_cache "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81"

static auto time_to_ticks(float time) noexcept { return static_cast<int>(0.5f + time / interfaces::globals->interval_per_tick); }
static auto ticks_to_time(int ticks) noexcept { return static_cast<float>(ticks * interfaces::globals->interval_per_tick); }

namespace csgo {
	extern player_t* local_player;
	extern c_usercmd* cmd;
	inline vec3_t real_angle = {};
	inline vec3_t fake_angle = {};
	inline vec3_t saved_fake_angle = {};

	inline bool update_animations = true;
	extern bool send_packet;
	float server_time(c_usercmd* = nullptr);
}