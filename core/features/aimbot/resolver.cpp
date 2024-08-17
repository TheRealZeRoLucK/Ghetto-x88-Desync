#include "../features.hpp"

//static std::random_device rd;
//static std::mt19937 rng(rd());

float angle_mod(float flAngle)
{
	return ((360.0f / 65536.0f) * ((int32_t)(flAngle * (65536.0f / 360.0f)) & 65535));
}

float approach_angle(float flTarget, float flValue, float flSpeed)
{
	flTarget = angle_mod(flTarget);
	flValue = angle_mod(flValue);

	float delta = flTarget - flValue;


	if (flSpeed < 0)
		flSpeed = -flSpeed;

	if (delta < -180)
		delta += 360;
	else if (delta > 180)
		delta -= 360;

	if (delta > flSpeed)
		flValue += flSpeed;
	else if (delta < -flSpeed)
		flValue -= flSpeed;
	else
		flValue = flTarget;

	return flValue;
}


static auto get_smoothed_vel = [](float min_delta, vec2_t a, vec2_t b) {
	vec2_t delta = a - b;
	float delta_length = delta.length();

	if (delta_length <= min_delta) {
		vec2_t result;
		if (-min_delta <= delta_length) {
			return a;
		}
		else {
			float iradius = 1.0f / (delta_length + FLT_EPSILON);
			return b - ((delta * iradius) * min_delta);
		}
	}
	else {
		float iradius = 1.0f / (delta_length + FLT_EPSILON);
		return b + ((delta * iradius) * min_delta);
	}
};
// no idea if this shit works
void aimbot::resolver::resolver(player_t* player) {
	if (!player || !player->get_anim_state())
		return;

	auto animState = player->get_anim_state();

	if (!animState)
		return;

	vec3_t velocity = player->velocity();
	float spd = velocity.length();
	if (spd > std::powf(1.2f * 260.0f, 2.f)) {
		vec3_t velocity_normalized = velocity.normalized();
		velocity = velocity_normalized * (1.2f * 260.0f);
	}

	float Resolveyaw = animState->goal_feet_yaw;

	auto delta_time = fmaxf(interfaces::globals->cur_time - animState->last_client_side_animation_update_time, 0.f);

	float deltatime = fabs(delta_time);
	float stop_to_full_running_fraction = 0.f;
	bool is_standing = true;
	float v25 = std::clamp(player->duck_amount() + animState->landing_duck_additive_something, 0.0f, 1.0f);
	float v26 = animState->duck_amount;
	float v27 = deltatime * 6.0f;
	float v28;

	if ((v25 - v26) <= v27) {
		if (-v27 <= (v25 - v26))
			v28 = v25;
		else
			v28 = v26 + v27;

	}
	else {
		v28 = v27 + v27;
	}

	float flDuckAmount = std::clamp(v28, 0.0f, 1.0f);

	vec2_t animationVelocity = velocity;
	float speed = std::fminf(animationVelocity.length(), 260.0f);

	weapon_t* weapon = csgo::local_player->active_weapon();
	if (!weapon)
		return;

	auto wpndata = weapon->get_weapon_data();

	if (!wpndata)
		return;

	float flMaxMovementSpeed = 260.0f;
	if (weapon) {
		flMaxMovementSpeed = std::fmaxf(wpndata->weapon_max_speed, 0.001f);
	}

	float flRunningSpeed = speed / (flMaxMovementSpeed * 0.520f);
	float flDuckingSpeed_2 = speed / (flMaxMovementSpeed * 0.340f);

	flRunningSpeed = std::clamp(flRunningSpeed, 0.0f, 1.0f);

	float flYawModifier = (((stop_to_full_running_fraction * -0.3f) - 0.2f) * flRunningSpeed) + 1.0f;
	if (flDuckAmount > 0.0f) {
		float flDuckingSpeed = std::clamp(flDuckingSpeed_2, 0.0f, 1.0f);
		flYawModifier += (flDuckAmount * flDuckingSpeed) * (0.5f - flYawModifier);
	}

	float flMaxBodyYaw = *reinterpret_cast<float*>(&animState->pad10[512]);
	float flMinBodyYaw = *reinterpret_cast<float*>(&animState->pad10[516]);



	float flEyeYaw = player->eye_angles().y;

	float flEyeDiff = std::remainderf(flEyeYaw - Resolveyaw, 360.f);

	if (flEyeDiff <= flMaxBodyYaw) {
		if (flMinBodyYaw > flEyeDiff)
			Resolveyaw = fabs(flMinBodyYaw) + flEyeYaw;
	}
	else {
		Resolveyaw = flEyeYaw - fabs(flMaxBodyYaw);
	}

	if (speed > 0.1f || fabs(velocity.z) > 100.0f) {
		Resolveyaw = approach_angle(
			flEyeYaw,
			Resolveyaw,
			((stop_to_full_running_fraction * 20.0f) + 30.0f)
			* deltatime);
	}
	else {
		Resolveyaw = approach_angle(
			player->lower_body_yaw(),
			Resolveyaw,
			deltatime * 100.0f);
	}

	if (stop_to_full_running_fraction > 0.0 && stop_to_full_running_fraction < 1.0)
	{
		const auto interval = interfaces::globals->interval_per_tick * 2.f;

		if (is_standing)
			stop_to_full_running_fraction = stop_to_full_running_fraction - interval;
		else
			stop_to_full_running_fraction = interval + stop_to_full_running_fraction;

		stop_to_full_running_fraction = std::clamp(stop_to_full_running_fraction, 0.f, 1.f);
	}

	if (speed > 135.2f && is_standing)
	{
		stop_to_full_running_fraction = fmaxf(stop_to_full_running_fraction, .0099999998f);
		is_standing = false;
	}

	if (speed < 135.2f && !is_standing)
	{
		stop_to_full_running_fraction = fminf(stop_to_full_running_fraction, .99000001f);
		is_standing = true;
	}
	// (im going insane) this is fucking retarded 
	auto& log = player_log::get().get_log(player->index());

	switch (log.m_nShots % 5)
	{
	case 0:
		break;
	case 1:
		animState->goal_feet_yaw += 58.0f;
		break;
	case 2:
		animState->goal_feet_yaw -= 58.0f;
		break;
	case 3:
		animState->goal_feet_yaw += 29.0f;
		break;
	case 4:
		animState->goal_feet_yaw -= 29.0f;
		break;
	default:
		break;
	}
}

player_log_t& player_log::get_log(const int index)
{
	return logs[index];
}