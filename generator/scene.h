#ifndef SCENE_SCENE_H_20221305
#define SCENE_SCENE_H_20221305

#include <array>
#include <vector>
#include <string>

#include "camera.h"
#include "aquarium.h"
#include "daphnia.h"
#include "utils.h"

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
	Daphnia getRCODaphnia(size_t, size_t);
	double getRCODaphniaLength(size_t, size_t);
	size_t getRCOObjectsNum(size_t);

	void setCameraRMat(const std::array<double, 9>&);
	void setCameraTVec(const std::array<double, 3>&);
	void setCameraSVec(const std::array<double, 3>&);
	void setObjGridPoints(const std::vector<cv::Point3d>&);
	void setGridFilename(const std::string&);

	void setAquariumEdgeTextureFilename(
		const std::string& edge, const std::string& filename);

	void setRCOFrames(const size_t&);
	void setRCOObjects(const size_t&, const size_t&);
	void setRCODaphniaCoords(const size_t&, const size_t&, const std::array<double, 3>&);
	void setRCODaphniaAngles(const size_t&, const size_t&, const std::array<double, 3>&);
	void setRCODaphniaScale(const size_t&, const size_t&, const double&);
	void setGenFramesPath(const std::string& path);
	void setGenMasksPath(const std::string& path);
	void resetFrameCount();

	void calcOuterCameraParams(const std::vector<cv::Point2d>&,
		const cv::Mat&, cv::Mat&, cv::Mat&, cv::Mat&);

	void initGLUT(GLfloat red = 1, GLfloat green = 1, GLfloat blue = 1);
	void drawAxis();

	void drawSimplifiedReflection(const Daphnia& daphnia,
		const std::array<double, 3>& color3d);
	void drawComplicatedReflection(const Daphnia& daphnia,
		const std::array<double, 4>& color_inner,
		const std::array<double, 4>& color_outer,
		const std::array<double, 4>& color_head);

	void drawRightPlaneReflection(const Daphnia& daphnia,
		const std::array<double, 3>& color3d);
	void drawLeftPlaneReflection(const Daphnia& daphnia,
		const std::array<double, 3>& color3d);
	void drawUpperPlaneReflection(const Daphnia& daphnia,
		const std::array<double, 3>& color3d);
	void drawLowerPlaneReflection(const Daphnia& daphnia,
		const std::array<double, 3>& color3d);

	void drawRightPlaneComplicatedReflection(const Daphnia& daphnia,
		const std::array<double, 4>& color_inner,
		const std::array<double, 4>& color_outer,
		const std::array<double, 4>& color_head);
	void drawLeftPlaneComplicatedReflection(const Daphnia& daphnia,
		const std::array<double, 4>& color_inner,
		const std::array<double, 4>& color_outer,
		const std::array<double, 4>& color_head);
	void drawUpperPlaneComplicatedReflection(const Daphnia& daphnia,
		const std::array<double, 4>& color_inner,
		const std::array<double, 4>& color_outer,
		const std::array<double, 4>& color_head);
	void drawLowerPlaneComplicatedReflection(const Daphnia& daphnia,
		const std::array<double, 4>& color_inner,
		const std::array<double, 4>& color_outer,
		const std::array<double, 4>& color_head);

	void reshape(int, int);
	//void controlSpec(int key, int x, int y);
	//void controlKey(unsigned char key, int x, int y);

	void displayPointGrid();
	void displayUntexturedRandomClip();
	void displayTexturedRandomClip();
	void displayMaskRandomClip();

private:
	cv::Size2i render_image_size_{ 1280, 1024 };
	std::array<double, 2> z_plane_{ 0.8, 1000 };
	double scale_{ 0.9 };
	double reflection_distance_limit_{ 2. };
	double reflection_strength_{ 0.3 };

	Camera camera_{ };
	Aquarium aquarium_{ };

	std::vector<cv::Point3d> objgridpoints_{ };
	std::string grid_filename_{ };

	std::vector<std::vector<Daphnia>> random_clip_objects_{ }; //RCO
	std::string generation_frames_path_{ };
	std::string generation_masks_path_{ };
	std::string generation_frames_ending_{ ".png" };
	size_t frame_count_{ };

	//double test_x{9};
	//double test_y{ };
	//double test_z{-2};
};

#endif
