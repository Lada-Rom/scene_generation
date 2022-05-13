#ifndef GENERATOR_GENERATOR_H_20221305
#define GENERATOR_GENERATOR_H_20221305

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "scene.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Generator {
public:
	Generator();
	~Generator() = default;

	std::string getMainJSONFilename();

	void constructMainJSON(bool load = true);
	void loadMainJSON();
	bool checkIfInputExists(const std::string&);
	void addInputToMainJSON(const std::string&, const std::string&);
	void addCameraParamsToMainJSON();

private:
	const std::string main_json_filename_{ "../../data/json/generator_params.json" };
	json main_json_{ };

	Scene main_scene_{ };
};

#endif
