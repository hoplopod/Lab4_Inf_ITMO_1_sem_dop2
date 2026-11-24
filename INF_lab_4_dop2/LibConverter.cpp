#include "LibConverter.h"

#include <fstream>
#include <stdexcept>

void LibConverter::convert(const std::string& jsonPath, const std::string& iniPath)
{
	std::ifstream in(jsonPath);
	if (!in.is_open()) throw std::runtime_error("Cannot open JSON file: " + jsonPath);

	nlohmann::ordered_json root = nlohmann::ordered_json::parse(in);

	mINI::INIStructure ini;
	fillIni(root, "", ini);

	mINI::INIFile file(iniPath);
	if (!file.generate(ini, true)) throw std::runtime_error("Cannot write INI file: " + iniPath);
}

std::string LibConverter::formatPrimitive(const nlohmann::ordered_json& node)
{
	if (!node.is_string()) return node.dump();

	std::string res;
	const std::string& s = node.get_ref<const std::string&>();
	res.reserve(s.size());
	for (char c : s) {
		if (c == '\n') res += "\\n";
		else if (c == '\r') res += "\\r";
		else res.push_back(c);
	}
	return res;
}

void LibConverter::fillIni(const nlohmann::ordered_json& node, const std::string& prefix, mINI::INIStructure& ini)
{
	if (node.is_primitive()) {
		std::string section = prefix.empty() ? "root" : prefix;
		ini[section]["value"] = formatPrimitive(node);
		return;
	}

	if (node.is_object()) {
		std::string section = prefix.empty() ? "root" : prefix;

		for (const auto& item : node.items()) {
			if (item.value().is_primitive()) ini[section][item.key()] = formatPrimitive(item.value());
		}

		for (const auto& item : node.items()) {
			if (!item.value().is_primitive()) {
				std::string nextPrefix = prefix.empty() ? item.key() : prefix + "." + item.key();
				fillIni(item.value(), nextPrefix, ini);
			}
		}
	}
	else if (node.is_array()) {
		std::string section = prefix.empty() ? "root" : prefix;

		for (size_t i = 0; i < node.size(); i++) {
			if (node[i].is_primitive()) ini[section][std::to_string(i)] = formatPrimitive(node[i]);
		}

		for (size_t i = 0; i < node.size(); i++) {
			if (!node[i].is_primitive()) {
				std::string nextPrefix = prefix.empty() ? std::to_string(i) : prefix + "." + std::to_string(i);
				fillIni(node[i], nextPrefix, ini);
			}
		}
	}
}