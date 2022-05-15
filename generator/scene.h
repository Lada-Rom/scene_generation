#ifndef SCENE_SCENE_H_20221305
#define SCENE_SCENE_H_20221305

#include <array>
#include <vector>
#include <string>

#include "camera.h"
#include "aquarium.h"

#include <stb_image.h>
#include <stb_image_write.h>

#include <opencv2/opencv.hpp>
#include <GL/freeglut.h>

class Scene {
public:
	Scene();
	~Scene() = default;

	cv::Size2i getRenderImageSize();
	std::array<double, 9> getIntrinsicCameraMatrix();
	std::array<double, 3> getAquariumSize();

	double getScale();

	void setCameraRMat(const std::array<double, 9>&);
	void setCameraTVec(const std::array<double, 3>&);
	void setObjGridPoints(const std::vector<cv::Point3d>&);
	void setGridFilename(const std::string&);
	void setGridShift(const std::array<double, 3>&);

	void calcOuterCameraParams(const std::vector<cv::Point2d>&,
		const cv::Mat&, cv::Mat&, cv::Mat&, cv::Mat&);
	void saveImage(int, int, std::string&);

	void initGLUT(GLfloat red = 1, GLfloat green = 1, GLfloat blue = 1);
	void drawAxis();
	void reshape(int, int);
	void displayPointGrid();

private:
	cv::Size2i render_image_size_{ 1280, 1024 };
	std::array<double, 2> z_plane_{ 0.8, 1000 };
	double scale_{ 0.9 };

	Camera camera_{ };
	Aquarium aquarium_{ };

	std::vector<cv::Point3d> objgridpoints_{ };
	std::array<double, 3> grid_shift_{ };
	std::string grid_filename_{ };
};

#endif
