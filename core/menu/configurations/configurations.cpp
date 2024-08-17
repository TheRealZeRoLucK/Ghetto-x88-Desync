#include "configurations.hpp"
#include "../variables.hpp"

#define config_header "thefundraiser"
// never finished this
void config::initialize() {
	directory_path = "C:/thefundraiser/configs";
	if (!std::filesystem::exists(directory_path))
		std::filesystem::create_directories(directory_path);
}

void config::save(const std::string name) {
	const auto path = directory_path + "/" + name;
	std::ofstream out(path);

	if (!out.is_open())
		return;

	Json::Value save;

	save[config_header][("bhop")] = variables::bhop;
	save[config_header][("server_hitbox")] = variables::server_hitbox;
	save[config_header][("skate")] = variables::skate;
	save[config_header][("cheat_status")] = variables::cheat_loaded::cheat_status;
	save[config_header][("fake_lag_enable")] = variables::fake_lag_enable;
	save[config_header][("disable_fl_onshot_c4")] = variables::disable_fl_onshot_c4;
	save[config_header][("fake_lag_amount")] = variables::fake_lag_amount;
	save[config_header][("fake_lag_type")] = variables::fake_lag_type;
//	save[config_header][("")] = variables::;



	out << save;
	out.close();
}

void config::load(const std::string name) {
	const auto path = directory_path + "/" + name;
	std::ifstream in(path);

	if (!in.good())
		save(name);

	if (!in.is_open())
		return;

	Json::Value load;
	// asFloat / asBool / asInt
	in >> load;
	variables::bhop = load[config_header][("bhop")].asBool();
	variables::server_hitbox = load[config_header][("server_hitbo")].asBool();
	variables::skate = load[config_header][("skate")].asBool();
	variables::cheat_loaded::cheat_status = load[config_header][("cheat_status")].asInt();
	variables::fake_lag_enable = load[config_header][("fake_lag_enable")].asBool();
	variables::disable_fl_onshot_c4 = load[config_header][("disable_fl_onshot_c4")].asBool();
	variables::fake_lag_amount = load[config_header][("fake_lag_amount")].asInt();
	variables::fake_lag_type = load[config_header][("fake_lag_type")].asInt();



	in.close();
}