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
	void saveGenRCOJSON(const std::string& path,
		const std::vector<std::vector<std::array<double, 3>>>& objpoints,
		const std::vector<std::vector<std::array<double, 3>>>& objdirections,
		const std::vector<std::vector<std::array<double, 2>>>& imgpoints,
		const std::vector<std::vector<std::array<double, 2>>>& imgdirections);

	std::vector<cv::Point2d> readInputImgpointsD(size_t);
	std::vector<cv::Point2i> readInputImgpointsI(size_t);
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
	void makeEdgeTextures(size_t index);
	void makeCVDaphniaTexture(size_t index, bool ovoid = true);
	void makeCVDaphniaMask(bool ovoid = true);
	void makeGLUTDaphniaTexture(size_t index);
	void processDaphniaTexture(
		const std::string& src_texture_filename,
		const cv::Mat& background,
		const std::array<double, 2>& center,
		const std::string& dst_texture_filename);

	void predictPoints(std::vector<cv::Point2d>&, const std::vector<cv::Point3d>&,
		const std::array<double, 9>&,
		const std::array<double, 9>&, const std::array<double, 3>&);
	void predictPoints(std::vector<std::vector<std::array<double, 2>>>& imgpoints,
		const std::vector<std::vector<std::array<double, 3>>>& objpoints,
		const std::array<double, 9>& cmat, const std::array<double, 9>& rmat,
		const std::array<double, 3>& tvec, const std::array<double, 3>& svec);
	void predictDirections(
		std::vector<std::vector<std::array<double, 2>>>& imgdirections,
		std::vector<std::vector<std::array<double, 3>>> objdirections,
		const std::vector<std::vector<std::array<double, 2>>>& imgpoints,
		const std::vector<std::vector<std::array<double, 3>>>& objpoints,
		const std::array<double, 9>& cmat, const std::array<double, 9>& rmat,
		const std::array<double, 3>& tvec, const std::array<double, 3>& svec);

	void showPointGrid(size_t, const cv::Size&, double,
		const std::array<double, 3>& shift = { 0., 0., 0. }, bool save = false);
	void showPointGrid(size_t, const cv::Size&, double, size_t,
		const std::array<double, 3>& shift = { 0., 0., 0. }, bool save = false);

	void genUntexturedRandomClip(size_t index, size_t num_frames,
		const std::array<double, 2>& num_objects_range,
		const std::array<double, 2>& size_objects_range = { 0.5, 0.5 },
		std::string path = {});
	void genUntexturedRandomClip(size_t index,
		const std::string& config_filename, std::string path = {});
	void genTexturedRandomClip(size_t index,
		const std::string& config_filename, std::string path = {});

	std::vector<double> cvtMatToVector(const cv::Mat&);

private:
	static Generator* curr_this_;
	int argc_{ };
	char** argv_{ };

	std::random_device rd_{ };

	static void displayPointGrid();
	static void displayUntexturedRandomClip();
	static void displayTexturedRandomClip();
	static void displayMaskRandomClip();
	static void reshape(int, int);
	static void controlKey(unsigned char key, int x, int y);
	static void controlSpec(int key, int x, int y);
	double normDistGenInRange(std::normal_distribution<>, const double&, const double&);

	const std::string main_json_filename_		{ "../../data/json/generator_params.json" };
	const std::string json_dir_					{ "json/" };

	const std::string config_json_dir_			{ "config/" };
	const std::string config_json_name_			{ "config" };

	const std::string src_dir_					{ "src/" };
	const std::string edges_dir_				{ "edges/" };
	const std::string right_edge_name_			{ ".right" };
	const std::string left_edge_name_			{ ".left" };
	const std::string upper_edge_name_			{ ".upper" };
	const std::string lower_edge_name_			{ ".lower" };
	const std::string bottom_edge_name_			{ ".bottom" };

	const std::string daphnia_texture_dir_		{ "daphnia/" };
	const std::string daphnia_ovoid_dir_		{ "ovoid/" };
	const std::string daphnia_circle_dir_		{ "circle/" };
	const std::string daphnia_glut_dir_			{ "glut/" };

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
	const std::string generation_textures_dir_	{ "textures/" };
	const std::string frames_glut_dir_			{ "glut/" };
	const std::string frames_merged_dir_		{ "merged/" };
	const std::string frames_mask_dir_			{ "mask/" };

	unsigned int texture_size_{ 128 };

	json main_json_{ };
	json config_json_{ };
	json gen_RCO_json_{ };
	Scene main_scene_{ };
};

#endif
