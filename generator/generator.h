#ifndef GENERATOR_GENERATOR_H_20221305
#define GENERATOR_GENERATOR_H_20221305

#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>

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
	void constructConfigRCOJSON(size_t num_frames,
		const std::array<double, 2>& num_object_range,
		const std::array<double, 2>& size_object_range);
	void constructConfigRCOJSON(size_t config_index, size_t num_frames,
		const std::array<double, 2>& num_object_range,
		const std::array<double, 2>& size_object_range);
	void loadMainJSON();
	void loadConfigRCOJSON(const std::string& config_filename);
	void saveMainJSON();
	void saveGenRCOJSON(const std::string&,
		const std::vector<std::vector<std::array<double, 3>>>&,
		const std::vector<std::vector<std::array<double, 2>>>&);

	std::vector<cv::Point2d> readInputImgpoints(size_t);
	std::string readInputImage(size_t);
	std::array<double, 9> readCameraRMat(size_t);
	std::array<double, 3> readCameraTVec(size_t);
	std::array<double, 3> readCameraSVec(size_t);
	void readConfigRCOJSON(size_t&, std::array<double, 2>&,
		std::array<double, 2>&, const std::string& filename);

	void writeCameraRMat(const cv::Mat&, size_t);
	void writeCameraTVec(const cv::Mat&, size_t);
	void writeCameraSVec(std::array<double, 3>, size_t);

	bool checkIfInputExists(const std::string&);
	void addInputToMainJSON(const std::string&, const std::string&);
	void addCameraParamsToMainJSON(size_t);

	void makeBackground(const std::string& video_filename, const std::string& bckg_filename);
	void makeTestTexture(const std::string& filename);

	void predictPoints(std::vector<cv::Point2d>&, const std::vector<cv::Point3d>&,
		const std::array<double, 9>&,
		const std::array<double, 9>&, const std::array<double, 3>&);
	void predictPoints(std::vector<std::vector<std::array<double, 2>>>&,
		const std::vector<std::vector<std::array<double, 3>>>&,
		const std::array<double, 9>&,
		const std::array<double, 9>&, const std::array<double, 3>&);
	void showPointGrid(size_t, const cv::Size&, double,
		const std::array<double, 3>& shift = { 0., 0., 0. }, bool save = false);
	void showPointGrid(size_t, const cv::Size&, double, size_t,
		const std::array<double, 3>& shift = { 0., 0., 0. }, bool save = false);

	void genRandomClip(size_t index, size_t num_frames,
		const std::array<double, 2>& num_objects_range,
		const std::array<double, 2>& size_objects_range = { 0.5, 0.5 },
		std::string path = {});
	void genRandomClip(size_t index,
		const std::string& config_filename, std::string path = {});

	std::vector<double> cvtMatToVector(const cv::Mat&);

private:
	static Generator* curr_this_;
	int argc_{ };
	char** argv_{ };

	std::random_device rd_{ };

	static void displayPointGrid();
	static void displayRandomClip();
	static void reshape(int, int);
	double normDistGenInRange(std::normal_distribution<>, const double&, const double&);

	const std::string main_json_filename_		{ "../../data/json/generator_params.json" };
	const std::string json_dir_					{ "json/" };
	const std::string config_json_dir_			{ "config/" };
	const std::string config_json_name_			{ "config" };
	const std::string generation_json_name_		{ "gen_annotation" };
	const std::string json_ending_				{ ".json" };

	const std::string data_path_				{ "../../data/" };
	const std::string image_ending_				{ ".png" };

	const std::string grid_dir_					{ "grid/" };
	const std::string grid_glut_name_			{ "grid_glut" };
	const std::string grid_merged_name_			{ "grid_merged" };

	const std::string RCO_generation_main_dir_	{ "RCO_generation/" };
	const std::string generation_frames_dir_	{ "frames/" };
	const std::string generation_json_dir_		{ "json/" };
	const std::string frames_glut_dir_			{ "glut/" };
	const std::string frames_merged_dir_		{ "merged/" };

	json main_json_{ };
	json config_json_{ };
	json gen_RCO_json_{ };
	Scene main_scene_{ };
};

#endif
