#ifndef SCENE_SCENE_H_20221305
#define SCENE_SCENE_H_20221305

#include <vector>

#include "camera.h"

#include <opencv2/opencv.hpp>

class Scene {
public:
	Scene();
	~Scene() = default;

private:
	cv::Size2i render_image_size_{ 1280, 1024 };

	Camera camera_{ };

	//std::vector<double> aquarium_size_{ 16.5, 14.5, 2.5 };
};

#endif
