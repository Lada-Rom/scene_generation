#ifndef AQUARIUM_AQUARIUM_H_20221405
#define AQUARIUM_AQUARIUM_H_20221405

#include <array>
#include <vector>
#include <unordered_map>

#include "texture.h"
#include "utils.h"

#include <opencv2/opencv.hpp>
#include <GL/freeglut.h>

class Aquarium {
public:
	Aquarium();
	~Aquarium() = default;

	std::array<double, 3> getSize();
	std::array<std::array<double, 3>, 8> getVertices();
	std::vector<cv::Point3d> getObjpoints();
	std::vector<cv::Point3d> getObjpoints(double scale);

	void setTextureFilename(const std::string& plane, const std::string& filename);

	void calcVerticies();

	void drawWire();
	void drawTextured();
	void drawRightPlane();
	void drawLeftPlane();
	void drawUpperPlane();
	void drawLowerPlane();
	void drawTexturedRightPlane();
	void drawTexturedLeftPlane();
	void drawTexturedUpperPlane();
	void drawTexturedLowerPlane();
	void drawTexturedBottomPlane();

private:
	std::array<double, 3> size_{ 18.1, 15.5, 2.1 }; //shift { -0.7, 0.23, 0 }
	const std::array<double, 3> default_size_{ 16.5, 14.5, 2.5};

	std::unordered_map<std::string, Texture> plane_textures_{
		{ "right", Texture() },
		{ "left", Texture() },
		{ "upper", Texture() },
		{ "lower", Texture() },
		{ "bottom", Texture() }, };

	std::array<std::array<double, 3>, 8> vertices_{ };
};

#endif
