#include <iostream>

#include "generator.h"

void main(int argc, char** argv)
try{
	Generator generator(argc, argv);
	generator.showPointGrid(0, { 8, 8 }, { -0.7, 0.21, 0 }, true);
}
catch (const std::exception& exc) {
	std::cout << exc.what() << std::endl;
}
