#ifndef SCENE_SCENE_H_20221305
#define SCENE_SCENE_H_20221305

#include <vector>

class Scene {
public:
	Scene() = default;
	~Scene() = default;

private:
	std::vector<double> aquarium_size_{ 16.5, 14.5, 2.5 };
};

#endif
