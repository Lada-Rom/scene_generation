#ifndef SCENE_SCENE_H_20221305
#define SCENE_SCENE_H_20221305

#include <array>
#include <vector>

#include "camera.h"
#include "aquarium.h"

#include <opencv2/opencv.hpp>

class Scene {
public:
	Scene();
	~Scene() = default;

	std::array<double, 9> getIntrinsicCameraMatrix();

	void calcOuterCameraParams(const std::vector<cv::Point2d>&,
		const cv::Mat&, cv::Mat&, cv::Mat&, cv::Mat&);

private:
	cv::Size2i render_image_size_{ 1280, 1024 };

	Camera camera_{ };
	Aquarium aquarium_{ };
};

#endif
