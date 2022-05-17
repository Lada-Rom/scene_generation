#include <iostream>

#include "generator.h"

void main(int argc, char** argv)
try{
	Generator generator(argc, argv);
	//generator.constructMainJSON();
	//generator.addInputToMainJSON("../../data/json/annotation_json.json", "../../data/src/src.1.png");
	//generator.addCameraParamsToMainJSON(0);
	
	//generator.showPointGrid(0, { 8, 8 }, 0, { -0.7, 0.23, 0 }, true);

	//double step = -0.3;
	//for (int i{}; i <= 7; ++i)
	//	generator.showPointGrid(0, { 8, 8 }, i * step, i, { -0.7, 0.23, 0 }, true);

	generator.genRandomClip(0, 30, { 5, 25 }, { 0.5, 0.8 });

	//generator.constructConfigRCOJSON(30, { 20, 30 }, { 0.5, 1. });
}
catch (const std::exception& exc) {
	std::cout << exc.what() << std::endl;
}
