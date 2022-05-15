#ifndef GENERATOR_GENERATOR_H_20221305
#define GENERATOR_GENERATOR_H_20221305

#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "scene.h"
#include "utils.h"

#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <GL/freeglut.h>

using json = nlohmann::json;

class Generator {
public:
	Generator();
	Generator(int, char**);
	~Generator() = default;

	std::string getMainJSONFilename();

	void constructMainJSON(bool load = true);
	void loadMainJSON();
	void saveMainJSON();

	std::vector<cv::Point2d> readInputImgpoints(size_t);
	std::string readInputImage(size_t);
	std::array<double, 9> readCameraRMat(size_t);
	std::array<double, 3> readCameraTVec(size_t);

	void writeRotationMatrix(const cv::Mat&, size_t);
	void writeTranslationVector(const cv::Mat&, size_t);
	void writeShiftVector(std::array<double, 3>, size_t);

	bool checkIfInputExists(const std::string&);
	void addInputToMainJSON(const std::string&, const std::string&);
	void addCameraParamsToMainJSON(size_t);

	void predictPoints(std::vector<cv::Point2d>&, const std::vector<cv::Point3d>&,
		const std::array<double, 9>&,
		const std::array<double, 9>&, const std::array<double, 3>&);
	void showPointGrid(size_t, const cv::Size&);

	std::vector<double> cvtMatToVector(const cv::Mat&);

private:
	static Generator* curr_this_;
	int argc_{ };
	char** argv_{ };

	static void displayPointGrid();
	static void reshape(int, int);

	const std::string main_json_filename_	{ "../../data/json/generator_params.json" };
	const std::string grid_glut_filename_	{ "../../data/grid/grid_glut.png" };
	const std::string grid_merged_filename	{ "../../data/grid/grid_merged.png" };

	json main_json_{ };
	Scene main_scene_{ };
};

#endif
