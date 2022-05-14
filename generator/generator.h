#ifndef GENERATOR_GENERATOR_H_20221305
#define GENERATOR_GENERATOR_H_20221305

#include <array>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "scene.h"

#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>

using json = nlohmann::json;

class Generator {
public:
	Generator();
	~Generator() = default;

	std::string getMainJSONFilename();

	void constructMainJSON(bool load = true);
	void loadMainJSON();
	void saveMainJSON();

	std::vector<cv::Point2d> readInputImgpoints(size_t);

	void writeRotationMatrix(const cv::Mat&, size_t);
	void writeTranslationVector(const cv::Mat&, size_t);
	void writeShiftVector(std::array<double, 3>, size_t);

	bool checkIfInputExists(const std::string&);
	void addInputToMainJSON(const std::string&, const std::string&);
	void addCameraParamsToMainJSON(size_t);

	std::vector<double> cvtMatToVector(const cv::Mat&);

private:
	const std::string main_json_filename_{ "../../data/json/generator_params.json" };
	json main_json_{ };

	Scene main_scene_{ };
};

#endif
