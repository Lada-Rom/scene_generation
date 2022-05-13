#include "generator.h"

////////// getMainJSONFilename //////////
std::string Generator::getMainJSONFilename() {
	return main_json_filename_;
}

////////// constructMainJSON //////////
void Generator::constructMainJSON() {
	json params;
	params["input"] = json::array();
	params["camera"] = json::array();

	std::ofstream file(getMainJSONFilename());
	file << params.dump(4) << std::endl;

}

////////// constructMainJSON //////////
void Generator::loadMainJSON() {
	std::ifstream file(getMainJSONFilename());
	file >> main_json_;
}

////////// checkIfInputExists //////////
bool Generator::checkIfInputExists(const std::string& image) {
	for (auto& element : main_json_["input"])
		if (element["image"] == image)
			return true;
	return false;
}

////////// addInputToMainJSON //////////
void Generator::addInputToMainJSON(
	const std::string& annotation, const std::string& image) {

	if (checkIfInputExists(image))
		return;

	//parse annotation
	std::ifstream file(annotation);
	if (file.fail())
		throw std::exception("Source JSON file not found");
	json annotation_json;
	file >> annotation_json;

	//getting data arrays
	json regions;
	for (auto it = annotation_json.begin(); it != annotation_json.end(); ++it)
		regions = annotation_json[it.key()]["regions"];	//skip name of file

	json part_1 = regions[0]["shape_attributes"];
	json part_2 = regions[1]["shape_attributes"];

	std::vector<size_t> x_coords = part_1["all_points_x"];
	std::vector<size_t> y_coords = part_1["all_points_y"];
	x_coords.insert(x_coords.end(), part_2["all_points_x"].begin(), part_2["all_points_x"].end());
	y_coords.insert(y_coords.end(), part_2["all_points_y"].begin(), part_2["all_points_y"].end());

	//add aanotation and image path to main json
	json element;
	element["x_coords"] = x_coords;
	element["y_coords"] = y_coords;
	element["image"] = image;
	main_json_["input"].push_back(element);

	//writting changes
	std::ofstream main_json_file(getMainJSONFilename());
	main_json_file << main_json_.dump(4) << std::endl;
}
