#include "../features.hpp"

void misc::client::untrusted(c_usercmd* cmd) {
	if (variables::safety == restriction_type::ANTI_UNTRUSTED)
		return

	cmd->viewangles.normalize();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.f, 89.f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.f, 180.f);
	cmd->viewangles.z = std::clamp(cmd->viewangles.z, 0.f, 0.f);
}

void misc::client::smac() {

}