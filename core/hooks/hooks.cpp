#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../menu/menu.hpp"

#define TICKS_TO_TIME(t) ( 	interfaces::globals->interval_per_tick* (t))

hooks::alloc_key_values::fn alloc_key_values_original = nullptr;
hooks::create_move_proxy::fn create_move_proxy_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;
hooks::frame_stage_notify::fn frame_stage_notify_original = nullptr;
hooks::override_view::fn override_view_original = nullptr;
hooks::dme::fn dme_original = nullptr;
hooks::override_mouse_input::fn override_mouse_input_original = nullptr;

bool hooks::initialize() {
	const auto alloc_key_values_target = reinterpret_cast<void*>(get_virtual(interfaces::key_values_system, 2));
	const auto create_move_proxy_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 22));
	const auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	const auto frame_stage_notify_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));
	const auto override_view_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 18));
	const auto dme_target = reinterpret_cast<void*>(get_virtual(interfaces::model_render, 21));
	const auto omi_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 23));

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize.");

	if (MH_CreateHook(alloc_key_values_target, &alloc_key_values::hook, reinterpret_cast<void**>(&alloc_key_values_original)) != MH_OK)
		throw std::runtime_error("failed to initialize alloc_key_values. (outdated index?)");

	if (MH_CreateHook(create_move_proxy_target, &create_move_proxy::hook, reinterpret_cast<void**>(&create_move_proxy_original)) != MH_OK)
		throw std::runtime_error("failed to initialize create_move_proxy. (outdated index?)");

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");

	if (MH_CreateHook(frame_stage_notify_target, &frame_stage_notify::hook, reinterpret_cast<void**>(&frame_stage_notify_original)) != MH_OK)
		throw std::runtime_error("failed to initialize frame_stage_notify. (outdated index?)");

	if (MH_CreateHook(override_view_target, &override_view::hook, reinterpret_cast<void**>(&override_view_original)) != MH_OK)
		throw std::runtime_error(("failed to initialize override_view."));

	if (MH_CreateHook(dme_target, &dme::hook, reinterpret_cast<void**>(&dme_original)) != MH_OK)
		throw std::runtime_error("failed to initialize dme. (outdated index?)");

	if (MH_CreateHook(omi_target, &override_mouse_input::hook, reinterpret_cast<void**>(&override_mouse_input_original)) != MH_OK)
		throw std::runtime_error("failed to initialize omi. (outdated index?)");

	// I LOOOOVE DETOURS MEN

	if (!hooks::should_skip_animation_frame::should_skip_animation_frame_detour.is_hooked())
		hooks::should_skip_animation_frame::should_skip_animation_frame_detour.create(utilities::pattern_scan("client.dll", sig_should_skip_animation_frame), hooks::should_skip_animation_frame::hook);

	if (!hooks::do_extra_bone_processing::do_extra_bone_processing_detour.is_hooked())
		hooks::do_extra_bone_processing::do_extra_bone_processing_detour.create(utilities::pattern_scan("client.dll", sig_do_extra_bone_processing), hooks::do_extra_bone_processing::hook);

	if (!hooks::standard_blending_rules::standard_blending_rules_detour.is_hooked())
		hooks::standard_blending_rules::standard_blending_rules_detour.create(utilities::pattern_scan("client.dll", sig_standard_blending_rules), hooks::standard_blending_rules::hook);

	if (!hooks::do_procedural_foot_plant::do_procedural_foot_plant_detour.is_hooked())
		hooks::do_procedural_foot_plant::do_procedural_foot_plant_detour.create(utilities::pattern_scan("client.dll", sig_do_procedural_foot_plant), hooks::do_procedural_foot_plant::hook);

	if (!hooks::update_client_side_animations::update_client_side_animations_detour.is_hooked())
		hooks::update_client_side_animations::update_client_side_animations_detour.create(utilities::pattern_scan("client.dll", sig_update_client_animations), hooks::update_client_side_animations::hook);

	if (!hooks::update_animation_state::update_animation_state_detour.is_hooked())
		hooks::update_animation_state::update_animation_state_detour.create(utilities::pattern_scan("client.dll", sig_update_animation_state), hooks::update_animation_state::hook);

	if (!hooks::modify_eye_position::modify_eye_position_detour.is_hooked())
		hooks::modify_eye_position::modify_eye_position_detour.create(utilities::pattern_scan("client.dll", sig_modify_eye_position), hooks::modify_eye_position::hook);

	if (!hooks::calculate_view_hook::calculate_view_hook_detour.is_hooked())
		hooks::calculate_view_hook::calculate_view_hook_detour.create(utilities::pattern_scan("client.dll", sig_calculate_view), hooks::calculate_view_hook::hook);

	if (!hooks::check_for_sequence_change::check_for_sequence_change_detour.is_hooked())
		hooks::check_for_sequence_change::check_for_sequence_change_detour.create(utilities::pattern_scan("client.dll", sig_check_for_sequence_change), hooks::check_for_sequence_change::hook);

	if (!hooks::build_transformations::check_for_sequence_change_detour.is_hooked())
		hooks::build_transformations::check_for_sequence_change_detour.create(utilities::pattern_scan("client.dll", sig_build_transformations), hooks::build_transformations::hook);
		
	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	skins::animfix_hook();

	console::log("[setup] hooks initialized!\n");
	variables::cheat_loaded::cheat_status += 1;
	return true;
}

void hooks::release() {
	skins::animfix_unhook();

	MH_Uninitialize();
	MH_DisableHook(MH_ALL_HOOKS);
}

std::uint8_t* key_values_engine = utilities::pattern_scan("engine.dll", "85 C0 74 ? 51 6A ? 56 8B C8 E8 ? ? ? ? 8B F0");
std::uint8_t* key_values_client = utilities::pattern_scan("client.dll", "85 C0 74 ? 6A ? 6A ? 56 8B C8 E8 ? ? ? ? 8B F0");

void* __stdcall hooks::alloc_key_values::hook(const std::int32_t size) {
	if (const std::uint32_t address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
		address == reinterpret_cast<std::uint32_t>(key_values_engine) || address == reinterpret_cast<std::uint32_t>(key_values_client))
		return nullptr;

	return alloc_key_values_original(interfaces::key_values_system, size);
}

static float nextLBY = -1;
bool should_breakLBY()
{
	if (csgo::local_player->velocity().length_2d() > 0.1f || !(csgo::local_player->flags() & fl_onground))
		nextLBY = interfaces::globals->cur_time + 0.22f;
	if (interfaces::globals->cur_time >= nextLBY)
	{
		nextLBY = interfaces::globals->cur_time + 1.1f;
		return true;
	}
	return false;
}

static bool __stdcall create_move(float input_sample_frametime, c_usercmd* cmd, bool& send_packet) {
	csgo::local_player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	csgo::send_packet = send_packet;

	auto net_channel = interfaces::clientstate->net_channel;
	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;

	should_breakLBY();
	sequence::update_incoming_sequences(net_channel);

	visuals::world::ambient_light();
	misc::movement::bunny_hop(cmd);
	misc::movement::force_crosshair();

	prediction::start(cmd); {
		backtrack.backtrack_run(cmd);
		aimbot::legit::run(cmd);
		antiaim::rageaa::remake_real(cmd, send_packet);
		antiaim::rageaa::remake_desync(cmd, send_packet);
		antiaim::rageaa::update_lowerbody_breaker();
		antiaim::rageaa::legit_antiaim_run(cmd, send_packet);
		antiaim::fakelag::do_fakelag(send_packet, cmd);
	} prediction::end();

	if (net_channel != nullptr) {
		if (!hooks::send_datagram::send_datagram_detour.is_hooked())
			hooks::send_datagram::send_datagram_detour.create(reinterpret_cast<void*>(hooks::get_virtual(net_channel, 46)), hooks::send_datagram::hook);
	}

	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);

	if (send_packet) {
		csgo::real_angle = cmd->viewangles;
		csgo::fake_angle = csgo::saved_fake_angle;
	}

	csgo::saved_fake_angle = variables::break_lby ? csgo::fake_angle : cmd->viewangles;

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

	cmd->viewangles.normalize();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	antiaim::animation::update_fake_animations();
	antiaim::animation::snyc_local_animations(cmd, send_packet);
	return false;
}

static void __stdcall chl_create_move(int sequence_number, float input_sample_time, bool active, bool& send_packet)
{
	create_move_proxy_original(sequence_number, input_sample_time, active);

	c_usercmd* cmd = interfaces::input->get_user_cmd(0, sequence_number);
	if (!cmd || !cmd->command_number)
		return;

	c_verified_user_cmd* verified_cmd = interfaces::input->get_verified_cmd(sequence_number);
	if (!verified_cmd)
		return;

	bool create_move_active = create_move(input_sample_time, cmd, send_packet);

	verified_cmd->user_cmd = *cmd;
	verified_cmd->u_hash_crc = cmd->get_checksum();
}

__declspec(naked) void __stdcall hooks::create_move_proxy::hook(int sequence_number, float input_sample_time, bool active)
{
	__asm
	{
		push ebp
		mov  ebp, esp
		push ebx;
		push esp
		push dword ptr[active]
		push dword ptr[input_sample_time]
		push dword ptr[sequence_number]
		call chl_create_move
		pop  ebx
		pop  ebp
		retn 0Ch
	}
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):
		menu::render();
		visuals::client::indicators(csgo::cmd);
		visuals::player::player_loop();
		visuals::player::world_loop();
		break;

	case fnv::hash("FocusOverlayPanel"):
		break;
	}
	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

int __fastcall hooks::send_datagram::hook(i_net_channel* net_channel, int edx, bf_write* datagram) {
	static auto original = hooks::send_datagram::send_datagram_detour.original<decltype(&send_datagram::hook)>();

	if (((variables::backtrack_time <= 200 || !variables::backtrack_alt) && variables::fakelatency <= 0) || datagram) {
		return original(net_channel, edx, datagram);
	}

	int state = net_channel->in_reliable_state;
	int sequence = net_channel->in_sequence_nr;

	auto lag_s = std::max(variables::fakelatency / 1000.f, (variables::backtrack_alt > ticks_to_time(200)) ? ticks_to_time(variables::backtrack_time - 200) : 0);
	auto lag_delta = lag_s - interfaces::engine->get_net_channel_info_alt()->get_latency(FLOW_OUTGOING);
	sequence::add_latency_to_net_channel(net_channel, lag_delta);

	int _return = original(net_channel, edx, datagram);

	net_channel->in_reliable_state = state;
	net_channel->in_sequence_nr = sequence;

	return _return;
}

void __fastcall hooks::do_extra_bone_processing::hook(void* ecx, void* edx, int a2, int a3, int a4, int a5, int a6, int a7) {
	return;
}

bool __fastcall hooks::should_skip_animation_frame::hook(void* this_pointer, void* edx) {
	return false;
}

void __fastcall hooks::do_procedural_foot_plant::hook(void* this_pointer, void* edx, void* bone_to_world, void* left_foot_chain, void* right_foot_chain, void* pos) {
	return;
}
// https://www.unknowncheats.me/forum/counterstrike-global-offensive/426383-paste-favourite-public-animation-fix-4.html
void __fastcall hooks::standard_blending_rules::hook(void* this_pointer, void* edx, void* hdr, void* pos, void* q, float current_time, int bone_mask) {
	static auto original = hooks::standard_blending_rules::standard_blending_rules_detour.original<decltype(&hooks::standard_blending_rules::hook)>();

	const auto entity = reinterpret_cast<entity_t*>(this_pointer);

	entity->effects() |= 8;

	original(this_pointer, edx, hdr, pos, q, current_time, bone_mask);

	entity->effects() &= ~8;
}

void __fastcall hooks::update_client_side_animations::hook(void* this_pointer, void* edx) {
	static auto original = hooks::update_client_side_animations::update_client_side_animations_detour.original<decltype(&hooks::update_client_side_animations::hook)>();

	const auto player = reinterpret_cast<player_t*>(this_pointer);

	if (player != csgo::local_player)
		return original(this_pointer, edx);

	if (csgo::update_animations)
		original(this_pointer, edx);
}

void __vectorcall hooks::update_animation_state::hook(void* this_pointer, void* unknown, float z, float y, float x, void* unknown1) {
	static auto original = hooks::update_animation_state::update_animation_state_detour.original<decltype(&hooks::update_animation_state::hook)>();

	const auto animation_state = reinterpret_cast<anim_state*>(this_pointer);

	if (animation_state->last_client_side_animation_update_framecount == interfaces::globals->frame_count)
		animation_state->last_client_side_animation_update_framecount -= 1;

	if (animation_state->last_client_side_animation_update_time == interfaces::globals->cur_time)
		animation_state->last_client_side_animation_update_time += ticks_to_time(1);

	const auto player = reinterpret_cast<entity_t*>(animation_state->base_entity);

	if (player != csgo::local_player)
		return original(this_pointer, unknown, z, y, x, unknown1);

	return original(this_pointer, unknown, csgo::real_angle.z, csgo::real_angle.y, csgo::real_angle.x, unknown1);
}

void __fastcall hooks::modify_eye_position::hook(void* this_pointer, void* edx, vec3_t& input_eye_position) {
	static auto original = hooks::modify_eye_position::modify_eye_position_detour.original<decltype(&hooks::modify_eye_position::hook)>();

	const auto animation_state = reinterpret_cast<anim_state*>(this_pointer);

	animation_state->set_smooth_height_valid(false);

	return original(this_pointer, edx, input_eye_position);
}

void __fastcall hooks::calculate_view_hook::hook(void* this_pointer, void* edx, vec3_t& eye_origin, vec3_t& eye_angles, float& z_near, float& z_far, float& fov) {
	static auto original = hooks::calculate_view_hook::calculate_view_hook_detour.original<decltype(&hooks::calculate_view_hook::hook)>();

	auto entity = reinterpret_cast<entity_t*>(this_pointer);

	if (!entity || !entity->is_player() || !csgo::local_player || entity != csgo::local_player)
		return original(this_pointer, edx, eye_origin, eye_angles, z_near, z_far, fov);

	const auto old_use_new_animation_state = entity->use_new_animation_state();

	entity->use_new_animation_state() = false;

	original(this_pointer, edx, eye_origin, eye_angles, z_near, z_far, fov);

	entity->use_new_animation_state() = old_use_new_animation_state;
}

static void __fastcall hooks::check_for_sequence_change::hook(void* this_pointer, void* edx, void* hdr, int current_sequence, bool force_new_sequence, bool interpolate)
{
	static auto original = hooks::check_for_sequence_change::check_for_sequence_change_detour.original<decltype(&hooks::check_for_sequence_change::hook)>();

	return original(this_pointer, edx, hdr, current_sequence, force_new_sequence, false);
}

void __fastcall hooks::build_transformations::hook(void* this_pointer, void* edx, c_studio_hdr* hdr, void* pos, void* q, matrix3x4_t* camera_transform, int bone_mask, void* bone_computed) {
	static auto original = hooks::build_transformations::check_for_sequence_change_detour.original<decltype(&hooks::build_transformations::hook)>();

	const utl_vector<int> backup_flags = hdr->bone_flags;

	for (int i = 0; i < hdr->bone_flags.size; i++)
	{
		hdr->bone_flags.elements[i] &= ~0x04;
	}

	original(this_pointer, edx, hdr, pos, q, camera_transform, bone_mask, bone_computed);

	hdr->bone_flags = backup_flags;
}

void __stdcall hooks::frame_stage_notify::hook(int frame_stage) {
	const auto local = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	static auto backtrack_init = (backtrack.initialize_backtrack(), false);
#ifdef _DEBUG
	antiaim::show_hitbox();
#endif
	aimbot::resolver::resolver(csgo::local_player);
	switch (frame_stage) {
	case FRAME_UNDEFINED:
		break;
	case FRAME_START:
		break;
	case FRAME_NET_UPDATE_START:
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
		if (variables::knife_enabled)
			skins::run();
		break;
	case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
		break;
	case FRAME_NET_UPDATE_END:
		backtrack.update();
		break;
	case FRAME_RENDER_START:
#ifdef _DEBUG
#endif
		if (local) {
			misc::thirdperson();
			if (!local->is_alive())
				interfaces::input->b_camera_in_third_person = false;
			if (local->is_alive() && interfaces::input->b_camera_in_third_person)
				*(vec3_t*)(((DWORD)local) + 0x31D4 + 0x4) = csgo::real_angle;
		}
		break;
	case FRAME_RENDER_END:
		break;
	default: break;
	}

	frame_stage_notify_original(interfaces::client, frame_stage);
}

void __fastcall hooks::override_view::hook(void* _this, void* _edx, view_setup_t* setup) {
	visuals::client::fov_changer();
	override_view_original(interfaces::clientmode, _this, setup);
}

void __fastcall hooks::dme::hook(void* thisptr, void* edx, void* ctx, void* state, const model_render_info_t& info, matrix_t* custom_bone_to_world) {
	visuals::chams::dme_chams_run(dme_original, thisptr, ctx, state, info, custom_bone_to_world);
	visuals::chams::dme_misc(dme_original, thisptr, ctx, state, info, custom_bone_to_world);
}

void __fastcall hooks::override_mouse_input::hook(void* ecx_, void* edx_, float* x, float* y) {
}