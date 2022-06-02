#include <iostream>

#include "generator.h"

void main(int argc, char** argv)
try{
	Generator generator(argc, argv);
	//generator.constructMainJSON();
	//generator.addInputToMainJSON("../../data/json/annotations/annotation.1.json", "../../data/src/bckg.1.png");
	//generator.addCameraParamsToMainJSON(1, { 18.1, 15.5, 2.1 });
	
	//generator.showPointGrid(0, { 8, 8 }, 0, { -0.7, 0.23, 0 }, true);
	//generator.showPointGrid(1, { 8, 8 }, 0, { -0.23, 0.07, 0. }, true);

	//double step = -0.3;
	//for (int i{}; i <= 7; ++i)
	//	generator.showPointGrid(0, { 8, 8 }, i * step, i, { -0.7, 0.23, 0 }, true);

	//generator.genUntexturedRandomClip(0, 30, { 5, 25 }, { 0.5, 0.8 });

	//generator.constructConfigRCOJSON(30, { 20, 30 }, { 0.5, 0.8 });
	//generator.genUntexturedRandomClip("../../data/json/config/config.0.json");

	//generator.makeBackground("../../data/src/video/4.avi", "../../data/src/bckg.4.png");
	//generator.makeTestTexture("../../data/src/test_texture.png");
	//generator.makeEdgeTextures(1);

	//for (int i{}; i < 20; ++i) {
	//	generator.makeCVDaphniaTexture(i, true);
	//	generator.makeCVDaphniaTexture(i, false);
	//}
	//generator.makeCVDaphniaMask(true);
	//generator.makeCVDaphniaMask(false);

	//generator.genTexturedRandomClip("../../data/json/config/config.0.json");
	generator.genTexturedSequentClip("../../data/json/config/config.1.json");
}
catch (const std::exception& exc) {
	std::cout << exc.what() << std::endl;
}
