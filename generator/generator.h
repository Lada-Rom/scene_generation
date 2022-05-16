#ifndef GENERATOR_GENERATOR_H_20221305
#define GENERATOR_GENERATOR_H_20221305

#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>

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
	void saveGenRCOJSON(const std::string&,
		const std::vector<std::vector<std::array<double, 3>>>&,
		const std::vector<std::vector<std::array<double, 2>>>&);

	std::vector<cv::Point2d> readInputImgpoints(size_t);
	std::string readInputImage(size_t);
	std::array<double, 9> readCameraRMat(size_t);
	std::array<double, 3> readCameraTVec(size_t);
	std::array<double, 3> readCameraSVec(size_t);

	void writeCameraRMat(const cv::Mat&, size_t);
	void writeCameraTVec(const cv::Mat&, size_t);
	void writeCameraSVec(std::array<double, 3>, size_t);

	bool checkIfInputExists(const std::string&);
	void addInputToMainJSON(const std::string&, const std::string&);
	void addCameraParamsToMainJSON(size_t);

	void predictPoints(std::vector<cv::Point2d>&, const std::vector<cv::Point3d>&,
		const std::array<double, 9>&,
		const std::array<double, 9>&, const std::array<double, 3>&);
	void predictPoints(std::vector<std::vector<std::array<double, 2>>>&,
		const std::vector<std::vector<std::array<double, 3>>>&,
		const std::array<double, 9>&,
		const std::array<double, 9>&, const std::array<double, 3>&);
	void showPointGrid(size_t, const cv::Size&,
		const std::array<double, 3>& shift = { 0., 0., 0. }, bool save = false);

	void genRandomClip(size_t, size_t, size_t, std::string path = { });

	std::vector<double> cvtMatToVector(const cv::Mat&);

private:
	static Generator* curr_this_;
	int argc_{ };
	char** argv_{ };

	std::random_device rd_{ };

	static void displayPointGrid();
	static void displayRandomClip();
	static void reshape(int, int);

	const std::string main_json_filename_		{ "../../data/json/generator_params.json" };
	const std::string grid_glut_filename_		{ "../../data/grid/grid_glut.png" };
	const std::string grid_merged_filename_		{ "../../data/grid/grid_merged.png" };
	const std::string generation_path_			{ "../../data/" };

	const std::string RCO_generation_main_dir_	{ "RCO_generation/" };
	const std::string generation_frames_dir_	{ "frames/" };
	const std::string generation_json_dir_		{ "json/" };
	const std::string frames_glut_dir_			{ "glut/" };
	const std::string frames_merged_dir_		{ "merged/" };
	const std::string generation_json_name_		{ "gen_annotation.json" };

	json main_json_{ };
	Scene main_scene_{ };

	json gen_RCO_json_{ };
};

#endif
