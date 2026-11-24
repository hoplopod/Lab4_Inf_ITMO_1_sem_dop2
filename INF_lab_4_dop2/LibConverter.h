#pragma once

#include <string>

#include "json.hpp"
#include "ini.h"

class LibConverter {
public:
	static void convert(const std::string& jsonPath, const std::string& iniPath);

private:
	static void fillIni(const nlohmann::ordered_json& node, const std::string& prefix, mINI::INIStructure& ini);
	static std::string formatPrimitive(const nlohmann::ordered_json& node);
};