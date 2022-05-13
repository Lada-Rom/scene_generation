#include "generator.h"

////////// getMainJSONFilename //////////
std::string Generator::getMainJSONFilename() {
	return generator_main_json;
}

////////// constructMainJSON //////////
void Generator::constructMainJSON() {
	json params;
	params["input"] = { };
	params["camera"] = { };

	std::ofstream file(getMainJSONFilename());
	file << params.dump(4) << std::endl;

}
