#include "../features.hpp"

float aimbot::autowall::get_damage_multiplier(int hit_group, float hs_multiplier) {
	switch (hit_group) {
	case hitgroup_head:			return hs_multiplier;		// Changes depending on weapon
	case hitgroup_stomach:		return 1.25f;
	case hitgroup_leftleg:
	case hitgroup_rightleg:		return 0.75f;
	default:					return 1.0f;
	}
}

bool aimbot::autowall::is_armored(int hit_group, bool helmet) {
	switch (hit_group) {
	case hitgroup_head:			return helmet;
	case hitgroup_chest:
	case hitgroup_stomach:
	case hitgroup_leftarm:
	case hitgroup_rightarm:		return true;
	default:					return false;
	}
}

void traceline(vec3_t& absStart, vec3_t& absEnd, unsigned int mask, entity_t* ignore, trace_t* ptr)
{

	ray_t ray;
	ray.initialize(absStart, absEnd);
	trace_filter filter;
	filter.skip = ignore;

	interfaces::trace_ray->trace_ray(ray, mask, &filter, ptr);
}

bool traceToExitalt3(trace_t* enter_trace, vec3_t start, vec3_t dir, trace_t* exit_trace)
{
	float flDistance = 0;
	vec3_t point = start;
	while (flDistance <= 125)
	{
		flDistance += 5;

		point += dir * flDistance;
		int point_contents = interfaces::trace_ray->get_point_contents_world(point, MASK_SHOT_HULL);
		if (!(point_contents & MASK_SHOT_HULL) || point_contents & CONTENTS_HITBOX)
		{
			traceline(point, enter_trace->end, MASK_SHOT | CONTENTS_GRATE, nullptr, exit_trace);
			return true;
		}
	}
}

static float aimbot::autowall::handlebulletpenetration(surface_data* enterSurfaceData, trace_t& enterTrace, const vec3_t& direction, vec3_t& result, float penetration, float damage) noexcept {
	vec3_t end;
	trace_t exitTrace;

	if (!traceToExitalt3(&enterTrace, enterTrace.end, direction, &exitTrace))
		return -1.0f;/*
	if (!traceToExit(enterTrace, enterTrace.endpos, direction, end, exitTrace))
		return -1.0f;*/
	surface_data* exitSurfaceData = interfaces::surface_props_physics->get_surface_data(exitTrace.surface.surfaceProps);

	float damageModifier = 0.16f;
	float penetrationModifier = (enterSurfaceData->penetrationmodifier + exitSurfaceData->penetrationmodifier) / 2.0f;

	if (enterSurfaceData->material == 71 || enterSurfaceData->material == 89) {
		damageModifier = 0.05f;
		penetrationModifier = 3.0f;
	}
	else if (enterTrace.contents >> 3 & 1 || enterTrace.surface.flags >> 7 & 1) {
		penetrationModifier = 1.0f;
	}

	if (enterSurfaceData->material == exitSurfaceData->material) {
		if (exitSurfaceData->material == 85 || exitSurfaceData->material == 87)
			penetrationModifier = 3.0f;
		else if (exitSurfaceData->material == 76)
			penetrationModifier = 2.0f;
	}
	//Utils::ConsolePrint(std::to_string(g_LocalPlayer->m_hActiveWeapon()->GetCSWeaponData()->flArmorRatio).c_str());
	damage -= 11.25f / penetration / penetrationModifier + damage * damageModifier + (exitTrace.end - enterTrace.end).length_sqr() / 24.0f / penetrationModifier;
	result = exitTrace.end;
	return damage;
}
/*
// Used to check if target it visible or hittable. Used in aimbot.
// enabled_hitbox will be used to know what hitboxes are enabled by the user (cuz now its iterating all due to bodyaim_if_lethal)
autowall_data_t aimbot::autowall::handle_walls(player_t* local_player, entity_t* entity, const vec3_t& destination, const weapon_info_t* weapon_data, bool enabled_hitbox) {
	if (!enabled_hitbox) return { false, -1.f };

	float damage = static_cast<float>(weapon_data->weapon_damage);
	vec3_t start = local_player->get_eye_pos();
	vec3_t direction = (destination - start);
	direction /= direction.length();
	float distance = 0.f;			// Will store traveled distance in the loop, max is the weapon range

	ray_t ray;						// Declare ray once before loop
	trace_filter filter;
	filter.skip = local_player;		// Initialize filter for ray before loop

	int hits_left = 4;
	while (damage >= 1.0f && hits_left) {
		vec3_t end = start + (direction * (weapon_data->weapon_range - distance));
		ray.initialize(start, end);

		static trace_t trace;
		interfaces::trace_ray->trace_ray(ray, 0x4600400B, &filter, &trace);
		if (trace.flFraction == 1.0f) break;

		distance += trace.flFraction * (weapon_data->weapon_range - distance);
		damage = damage * get_damage_multiplier(trace.hitGroup, weapon_data->weapon_headshot_multiplier) * powf(weapon_data->weapon_range_mod, distance / 500.0f);
		if (trace.entity == entity && trace.hitGroup > hitgroup_generic && trace.hitGroup <= hitgroup_rightleg) {
			if (float armor_ratio{ weapon_data->weapon_armor_ratio / 2.0f }; is_armored(trace.hitGroup, trace.entity->has_helmet()))
				damage -= (trace.entity->armor() < damage * armor_ratio / 2.0f ? trace.entity->armor() * 4.0f : damage) * (1.0f - armor_ratio);

			// If we can kill and we have the setting enabled, ignore enabled hitboxes and shoot
			if (variables::bodyaim_if_lethal && reinterpret_cast<player_t*>(entity)->health() < damage)
				return { true, damage };
			// If we can't kill, the best place to shoot is the closest enabled hitbox
			else if (enabled_hitbox)
				return { false, damage };
		}
		// Return invalid if we care only about visible and we dont see the target. This should never happen.
		//if (variables::aim::autowall.idx == 0) return { false, -1.f };

		const auto surface_data = interfaces::surface_props_physics->get_surface_data(trace.surface.surfaceProps);
		if (surface_data->penetrationmodifier < 0.1f)
			break;

		// Start and damage are changed from handle_bullet_penetration()
		if (!autowall::handlebulletpenetration(surface_data, trace, direction, start, weapon_data->weapon_penetration, damage))
			return { false, -1.f };

		hits_left--;
	}

	return { false, -1.f };
}
*/