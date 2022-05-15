#include <iostream>

#include "generator.h"

void main(int argc, char** argv)
try{
	Generator generator(argc, argv);
	generator.addCameraParamsToMainJSON(0);
	generator.showPointGrid(0, { 8, 8 });
}
catch (const std::exception& exc) {
	std::cout << exc.what() << std::endl;
}
