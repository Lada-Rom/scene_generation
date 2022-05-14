#include <iostream>

#include "generator.h"

void main()
try{
	Generator generator;
	generator.addCameraParamsToMainJSON(0);
}
catch (const std::exception& exc) {
	std::cout << exc.what() << std::endl;
}
