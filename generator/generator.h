#ifndef GENERATOR_GENERATOR_H_20221305
#define GENERATOR_GENERATOR_H_20221305

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Generator {
public:
	Generator() = default;
	~Generator() = default;

	std::string getMainJSONFilename();

	void constructMainJSON();
	void loadMainJSON();
	void addInputToMainJSON(const std::string&, const std::string&);

private:
	const std::string main_json_filename_{ "../../data/json/generator_params.json" };
	json main_json_{ };
};

#endif
