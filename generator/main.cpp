#include <iostream>

#include "generator.h"

void main()
try{
	Generator generator;
	generator.loadMainJSON();
	generator.constructMainJSON();
	generator.addInputToMainJSON(
		"../../data/json/annotation_json.json", "../../data/src/src.1.png");
}
catch (const std::exception& exc) {
	std::cout << exc.what() << std::endl;
}
