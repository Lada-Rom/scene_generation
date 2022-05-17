#ifndef AQUARIUM_AQUARIUM_H_20221405
#define AQUARIUM_AQUARIUM_H_20221405

#include <array>
#include <vector>

#include <opencv2/opencv.hpp>
#include <GL/freeglut.h>

class Aquarium {
public:
	Aquarium();
	~Aquarium() = default;

	std::array<double, 3> getSize();
	std::array<std::array<double, 3>, 8> getVertices();
	std::vector<cv::Point3d> getObjpoints();
	std::vector<cv::Point3d> getObjpoints(double);

	void calcVerticies();

	void draw();

private:
	std::array<double, 3> size_{ 17.9, 15.5, 2.1 };
	//std::array<double, 3> size_{ 18.1, 15.5, 2.1 };
	const std::array<double, 3> default_size_{ 16.5, 14.5, 2.5};

	std::array<std::array<double, 3>, 8> vertices_{ };
};

#endif
