#pragma once
#include "../../../dependencies/utilities/json.hpp"
#include "../../../dependencies/utilities/csgo.hpp"

namespace config {
	void initialize();
	void load(const std::string name);
	void save(const std::string name);

	inline std::string directory_path;
}