#ifndef GENERATOR_GENERATOR_H_20221305
#define GENERATOR_GENERATOR_H_20221305

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Generator {
public:
	Generator() = default;
	~Generator() = default;

	std::string getMainJSONFilename();

	void constructMainJSON();

private:
	const std::string generator_main_json{ "../../data/json/generator_params.json" };
};

#endif
