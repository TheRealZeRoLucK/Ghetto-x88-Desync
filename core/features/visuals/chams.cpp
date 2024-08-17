#include "../features.hpp"

void visuals::chams::dme_chams_run(hooks::dme::fn original, void* thisptr, void* context, void* state, const model_render_info_t& info, matrix_t* custom_bone_to_world)
{
	if (!variables::chams_enable)
		return original(thisptr, context, state, info, custom_bone_to_world);

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return original(thisptr, context, state, info, custom_bone_to_world);

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player)
		return original(thisptr, context, state, info, custom_bone_to_world);

	if (!info.model)
		return original(thisptr, context, state, info, custom_bone_to_world);

	// get our entity
	auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));
	if (!entity || !entity->is_alive() || entity->dormant())
		return original(thisptr, context, state, info, custom_bone_to_world);

	static bool create = true;
	if (create)
	{
		custom_materials();
		create = false;
	}

	if (strstr(interfaces::model_info->get_model_name(info.model), "models/player"))
	{
		static int index;
		static i_material* mat = nullptr;
		static i_material* mat_xqz = nullptr;
		//TODO: https://github.com/NullHooks/NullHooks/blob/3424a65ee978f9834f72d9f9c6012a49a3d30710/src/core/features/visuals/chams.cpp#L7 <- this is that shit i said about here \/
		if (!mat || !mat_xqz || index != variables::chams_type) // how do i do this better? nvm that link shit exists, im retardedso imma prolly do this in like 2 months or never do this
			switch (variables::chams_type)
			{
			case 0:
				mat = interfaces::material_system->find_material("regular", TEXTURE_GROUP_MODEL); index = variables::chams_type;
				mat_xqz = interfaces::material_system->find_material("/regular_xqz", TEXTURE_GROUP_MODEL); index = variables::chams_type; break;
			case 1:
				mat = interfaces::material_system->find_material("flat", TEXTURE_GROUP_MODEL); index = variables::chams_type;
				mat_xqz = interfaces::material_system->find_material("flat_xqz", TEXTURE_GROUP_MODEL); index = variables::chams_type; break;
			case 2:
				mat = interfaces::material_system->find_material("ghost", TEXTURE_GROUP_MODEL); index = variables::chams_type;
				mat_xqz = interfaces::material_system->find_material("ghost_xqz", TEXTURE_GROUP_MODEL); index = variables::chams_type; break;
			case 3:
				mat = interfaces::material_system->find_material("onetap_overlay", TEXTURE_GROUP_MODEL); index = variables::chams_type;
				mat_xqz = interfaces::material_system->find_material("onetap_overlay_xqz", TEXTURE_GROUP_MODEL); index = variables::chams_type; break;
			case 4:
				mat = interfaces::material_system->find_material("pearlescent", TEXTURE_GROUP_MODEL); index = variables::chams_type;
				mat_xqz = interfaces::material_system->find_material("pearlescent_xqz", TEXTURE_GROUP_MODEL); index = variables::chams_type; break;
			case 5:
				mat = interfaces::material_system->find_material("animated", TEXTURE_GROUP_MODEL); index = variables::chams_type;
				mat_xqz = interfaces::material_system->find_material("animated_xqz", TEXTURE_GROUP_MODEL); index = variables::chams_type; break;
			case 6:
				mat = interfaces::material_system->find_material("water", TEXTURE_GROUP_MODEL); index = variables::chams_type;
				mat_xqz = interfaces::material_system->find_material("water_xqz", TEXTURE_GROUP_MODEL); index = variables::chams_type; break;
			default:
				index = variables::chams_type; break;
			}

		mat->increment_reference_count();
		mat->set_material_var_flag(material_var_wireframe, variables::wireframe_chams);
		// backtrack

		// team
		if (variables::team_check_chams && entity != local_player && entity->team() == local_player->team())
		{
			if (variables::chams_xqz)
			{
				interfaces::render_view->set_blend(variables::f_xqz_clr[3]);
				interfaces::render_view->modulate_color(variables::f_xqz_clr);
				interfaces::model_render->override_material(mat_xqz);
				original(thisptr, context, state, info, custom_bone_to_world);
			}

			interfaces::render_view->set_blend(variables::f_chams_clr[3]);
			interfaces::render_view->modulate_color(variables::f_chams_clr);
			interfaces::model_render->override_material(mat);
		}

		// enemy
		if (variables::ememy_chams && entity->team() != local_player->team())
		{
			if (variables::chams_xqz)
			{
				interfaces::render_view->set_blend(variables::f_xqz_clr[3]);
				interfaces::render_view->modulate_color(variables::f_xqz_clr);
				interfaces::model_render->override_material(mat_xqz);
				original(thisptr, context, state, info, custom_bone_to_world);
			}

			interfaces::render_view->set_blend(variables::f_chams_clr[3]);
			interfaces::render_view->modulate_color(variables::f_chams_clr);
			interfaces::model_render->override_material(mat);
		}

		// local
		if (variables::local_chams && entity == local_player)
		{
			if (variables::chams_xqz)
			{
				interfaces::render_view->set_blend(variables::f_xqz_clr[3]);
				interfaces::render_view->modulate_color(variables::f_xqz_clr);
				interfaces::model_render->override_material(mat_xqz);
				original(thisptr, context, state, info, custom_bone_to_world);
			}
			interfaces::render_view->set_blend(variables::f_chams_local_clr[3]);
			interfaces::render_view->modulate_color(variables::f_chams_local_clr);
			interfaces::model_render->override_material(mat);
		}

		if (variables::desync_chams && entity == local_player) {
			interfaces::render_view->set_blend(variables::f_desync_chams[3]);
			interfaces::render_view->modulate_color(variables::f_desync_chams);
			interfaces::model_render->override_material(mat);
			original(interfaces::model_render, context, state, info, antiaim::animation::fake_matrix);

			interfaces::model_render->override_material(nullptr);
			original(thisptr, context, state, info, custom_bone_to_world);
		}
		
		if (entity->is_moving() && variables::backtrack_chams) {
			auto record = &records[info.entity_index];
			static i_material* backtrack_mat = nullptr;
			backtrack_mat = interfaces::material_system->find_material("ghost", TEXTURE_GROUP_MODEL); index = variables::chams_type;

			for (int i = 0; i < record->size(); ++i) {
				if (record && !record->empty() && backtrack.valid_tick(record->at(i).simulation_time))
				interfaces::render_view->set_blend(variables::f_backtrack_chams[3]);
				interfaces::render_view->modulate_color(variables::f_backtrack_chams);
				interfaces::model_render->override_material(backtrack_mat);
				original(interfaces::model_render, context, state, info, record->at(i).matrix);
			}
		}
	}
	original(thisptr, context, state, info, custom_bone_to_world);
	interfaces::model_render->override_material(nullptr);
}

void visuals::chams::dme_misc(hooks::dme::fn original, void* thisptr, void* context, void* state, const model_render_info_t& info, matrix_t* custom_bone_to_world)
{
	if (!variables::chams_enable)
		return;

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return;

	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
	if (!local_player || !local_player->is_alive())
		return;

	static i_material* mat = nullptr;
	if (!mat)
		mat = interfaces::material_system->find_material("regular", TEXTURE_GROUP_MODEL);

	const auto mdl = info.model;

	//bool is_arms = strstr(mdl->name, "arms") != nullptr;
	//bool is_sleeves = strstr(mdl->name, "sleeve") != nullptr;
	//bool is_weapon = strstr(mdl->name, "weapons/v_") != nullptr;
	//bool is_fists = std::strstr(mdl->name, "fists") != nullptr;
	//bool is_players = strstr(mdl->name, "models/player") != nullptr;

	const char* model_name = interfaces::model_info->get_model_name(info.model); player_t* entity;
	if (strstr(model_name, "models/player")) {
		entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));
	}
	else {
		entity = nullptr;
	}

	if (strstr(model_name, "models/weapons") && !strstr(model_name, "arms")) {
		if (variables::weapon_chams) {
			interfaces::render_view->set_blend(variables::f_weapon_chams[3]);
			interfaces::render_view->modulate_color(variables::f_weapon_chams);
			mat->set_material_var_flag(material_var_ignorez, false);
			interfaces::model_render->override_material(mat);
			original(thisptr, context, state, info, custom_bone_to_world);
		}
	}
	if (strstr(model_name, "models/weapons") && strstr(model_name, "arms") && strstr(model_name, "sleeve")) {
		if (variables::sleeve_chams) {
			interfaces::render_view->set_blend(variables::f_sleeve_chams[3]);
			interfaces::render_view->modulate_color(variables::f_sleeve_chams);
			mat->set_material_var_flag(material_var_ignorez, false);
			interfaces::model_render->override_material(mat);
			original(thisptr, context, state, info, custom_bone_to_world);
		}
	}
	if (strstr(model_name, "models/weapons") && strstr(model_name, "arms") && !strstr(model_name, "sleeve")) {
		if (variables::arms_chams) {
			interfaces::render_view->set_blend(variables::f_arms_chams[3]);
			interfaces::render_view->modulate_color(variables::f_arms_chams);
			mat->set_material_var_flag(material_var_ignorez, false);
			interfaces::model_render->override_material(mat);
			original(thisptr, context, state, info, custom_bone_to_world);
		}
	}
//	original(thisptr, context, state, info, custom_bone_to_world);
///	interfaces::model_render->override_material(nullptr);
}

