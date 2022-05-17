#include <iostream>

#include "generator.h"

void main(int argc, char** argv)
try{
	Generator generator(argc, argv);
	//generator.showPointGrid(0, { 8, 8 }, 0, { -0.7, 0.23, 0 }, true);

	//double step = -0.3;
	//for (int i{}; i <= 7; ++i)
	//	generator.showPointGrid(0, { 8, 8 }, i * step, i, { -0.7, 0.23, 0 }, true);

	generator.genRandomClip(0, 30, 25);
}
catch (const std::exception& exc) {
	std::cout << exc.what() << std::endl;
}
