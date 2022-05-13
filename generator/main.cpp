#include <iostream>

#include "generator.h"

void main()
try{
	Generator generator;
	//generator.constructMainJSON();
	generator.loadMainJSON();
	//generator.addInputToMainJSON(
	//	"../../data/json/annotation_json.json", "../../data/src/src.1.png");

	generator.addCameraParamsToMainJSON();
}
catch (const std::exception& exc) {
	std::cout << exc.what() << std::endl;
}
