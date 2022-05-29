#include "generator.h"

Generator* Generator::curr_this_;

////////// Generator //////////
Generator::Generator() {
	loadMainJSON();
}

////////// Generator //////////
Generator::Generator(int argc, char** argv) {
	loadMainJSON();
	glutInit(&argc, argv);
	argc_ = argc;
	argv_ = argv;
}

////////// getMainJSONFilename //////////
std::string Generator::getMainJSONFilename() {
	return main_json_filename_;
}

////////// constructMainJSON //////////
void Generator::constructMainJSON(bool load) {
	json params;
	params["input"] = json::array();
	params["camera"] = json::array();

	std::ofstream file(getMainJSONFilename());
	file << params.dump(4) << std::endl;

	if (load)
		loadMainJSON();
}

////////// constructConfigRCOJSON //////////
void Generator::constructConfigRCOJSON(size_t num_frames,
	const std::array<double, 2>& num_object_range,
	const std::array<double, 2>& size_object_range) {
	json config;
	config["num_frames"] = num_frames;
	config["object_quantity_range"] = num_object_range;
	config["object_size_range"] = size_object_range;
	config["camera_params_index"] = 0;

	std::ofstream file(data_path_ + json_dir_
		+ config_json_dir_+ config_json_name_ + json_ending_);
	file << config.dump(4) << std::endl;
	config_json_ = config;
}

////////// constructConfigRCOJSON //////////
void Generator::constructConfigRCOJSON(size_t config_index, size_t num_frames,
	const std::array<double, 2>& num_object_range,
	const std::array<double, 2>& size_object_range) {
	json config;
	config["num_frames"] = num_frames;
	config["object_quantity_range"] = num_object_range;
	config["object_size_range"] = size_object_range;
	config["camera_params_index"] = 0;

	std::ofstream file(data_path_ + json_dir_ + config_json_dir_
		+ config_json_name_ + "." + std::to_string(config_index) + json_ending_);
	file << config.dump(4) << std::endl;
	config_json_ = config;
}

////////// loadMainJSON //////////
void Generator::loadMainJSON() {
	std::ifstream file(getMainJSONFilename());
	file >> main_json_;
}

////////// loadConfigJSON //////////
void Generator::loadConfigJSON(const std::string& config_filename) {
	std::ifstream file(config_filename);
	file >> config_json_;
}

////////// saveMainJSON //////////
void Generator::saveMainJSON() {
	std::ofstream file(getMainJSONFilename());
	file << main_json_.dump(4) << std::endl;
}

////////// saveGenCOJSON //////////
void Generator::saveGenCOJSON(const std::string& path,
	const std::vector<std::vector<std::array<double, 3>>>& objpoints,
	const std::vector<std::vector<std::array<double, 3>>>& objdirections,
	const std::vector<std::vector<std::array<double, 2>>>& imgpoints,
	const std::vector<std::vector<std::array<double, 2>>>& imgdirections) {
	
	gen_RCO_json_["3D_points"] = json::array();
	gen_RCO_json_["2D_points"] = json::array();

	json object_3d, object_2d;
	for (int frame{}; frame < objpoints.size(); ++frame) {
		for (int object{}; object < objpoints[frame].size(); ++object) {
			object_3d["coords"] = objpoints[frame][object];
			object_3d["direction"] = objdirections[frame][object];
			object_2d["coords"] = imgpoints[frame][object];
			object_2d["direction"] = imgdirections[frame][object];
			gen_RCO_json_["3D_points"][frame].push_back(object_3d);
			gen_RCO_json_["2D_points"][frame].push_back(object_2d);
		}
	}

	std::ofstream file(path + generation_json_name_ + json_ending_);
	file << gen_RCO_json_.dump(4) << std::endl;
}

////////// readInputImgpointsD //////////
std::vector<cv::Point2d> Generator::readInputImgpointsD(size_t index) {
	json x_coords = main_json_["input"][index]["x_coords"];
	json y_coords = main_json_["input"][index]["y_coords"];

	std::vector<cv::Point2d> imgpoints;
	for (int j{}; j < x_coords.size(); ++j)
		imgpoints.push_back(cv::Point2d{ (double)x_coords[j], (double)y_coords[j] });

	return imgpoints;
}

////////// readInputImgpointsI //////////
std::vector<cv::Point2i> Generator::readInputImgpointsI(size_t index) {
	json x_coords = main_json_["input"][index]["x_coords"];
	json y_coords = main_json_["input"][index]["y_coords"];

	std::vector<cv::Point2i> imgpoints;
	for (int j{}; j < x_coords.size(); ++j)
		imgpoints.push_back(cv::Point2i{ x_coords[j].get<int>(), y_coords[j].get<int>()});

	return imgpoints;
}

////////// readInputImage //////////
std::string Generator::readInputImage(size_t index) {
	return main_json_["input"][index]["image"].get<std::string>();
}

////////// readCameraRMat //////////
std::array<double, 9> Generator::readCameraRMat(size_t index) {
	return main_json_["camera"][index]["rmat"].get<std::array<double, 9>>();
}

////////// readCameraTVec //////////
std::array<double, 3> Generator::readCameraTVec(size_t index) {
	return main_json_["camera"][index]["tvec"].get<std::array<double, 3>>();
}

////////// readCameraSVec //////////
std::array<double, 3> Generator::readCameraSVec(size_t index) {
	return main_json_["camera"][index]["svec"].get<std::array<double, 3>>();
}

////////// readConfigRCOJSON //////////
void Generator::readConfigRCOJSON(size_t& index, size_t& num_frames,
	std::array<double, 2>& num_objects_range,
	std::array<double, 2>& size_objects_range,
	const std::string& filename) {
	loadConfigJSON(filename);
	index = config_json_["camera_params_index"].get<size_t>();
	num_frames = config_json_["num_frames"].get<size_t>();
	num_objects_range = config_json_["object_quantity_range"].get<std::array<double, 2>>();
	size_objects_range = config_json_["object_size_range"].get<std::array<double, 2>>();
}

////////// readConfigSCOJSON //////////
void Generator::readConfigSCOJSON(size_t& index, size_t& num_frames,
	std::array<double, 2>& num_objects_range,
	std::array<double, 2>& size_objects_range,
	const std::string& filename) {
	loadConfigJSON(filename);
	index = config_json_["camera_params_index"].get<size_t>();
	num_frames = config_json_["duration"].get<int>() * config_json_["fps"].get<int>();
	num_objects_range = config_json_["object_quantity_range"].get<std::array<double, 2>>();
	size_objects_range = config_json_["object_size_range"].get<std::array<double, 2>>();
}

////////// readConfigSCOJSON //////////
void Generator::readConfigSCOJSON(size_t& index, double& fps, size_t& num_frames,
	std::array<double, 2>& num_objects_range,
	std::array<double, 2>& size_objects_range,
	const std::string& filename) {
	loadConfigJSON(filename);
	index = config_json_["camera_params_index"].get<size_t>();
	fps = config_json_["fps"].get<int>();
	num_frames = config_json_["duration"].get<int>() * fps;
	num_objects_range = config_json_["object_quantity_range"].get<std::array<double, 2>>();
	size_objects_range = config_json_["object_size_range"].get<std::array<double, 2>>();
}

////////// writeCameraRMat //////////
void Generator::writeCameraRMat(const cv::Mat& rmat, size_t i) {
	main_json_["camera"][i]["rmat"] = cvtMatToVector(rmat);
}

////////// writeCameraTVec //////////
void Generator::writeCameraTVec(const cv::Mat& rvec, size_t i) {
	main_json_["camera"][i]["tvec"] = cvtMatToVector(rvec);
}

////////// writeCameraSVec //////////
void Generator::writeCameraSVec(std::array<double, 3> svec, size_t i) {
	main_json_["camera"][i]["svec"] = svec;
}

////////// checkIfInputExists //////////
bool Generator::checkIfInputExists(const std::string& image) {
	for (auto& element : main_json_["input"])
		if (element["image"] == image)
			return true;
	return false;
}

////////// addInputToMainJSON //////////
void Generator::addInputToMainJSON(
	const std::string& annotation, const std::string& image) {

	if (checkIfInputExists(image))
		return;

	//parse annotation
	std::ifstream file(annotation);
	if (file.fail())
		throw std::exception("Source JSON file not found");
	json annotation_json;
	file >> annotation_json;

	//getting data arrays
	json regions;
	for (auto it = annotation_json.begin(); it != annotation_json.end(); ++it)
		regions = annotation_json[it.key()]["regions"];	//skip name of file

	json part_1 = regions[0]["shape_attributes"];
	json part_2 = regions[1]["shape_attributes"];

	std::vector<size_t> x_coords = part_1["all_points_x"];
	std::vector<size_t> y_coords = part_1["all_points_y"];
	x_coords.insert(x_coords.end(), part_2["all_points_x"].begin(), part_2["all_points_x"].end());
	y_coords.insert(y_coords.end(), part_2["all_points_y"].begin(), part_2["all_points_y"].end());

	//add aanotation and image path to main json
	json element;
	element["x_coords"] = x_coords;
	element["y_coords"] = y_coords;
	element["image"] = image;
	main_json_["input"].push_back(element);

	//writting changes
	saveMainJSON();
}

////////// addCameraParamsToMainJSON //////////
void Generator::addCameraParamsToMainJSON(size_t index) {
	//get imgpoints
	std::vector<cv::Point2d> imgpoints = readInputImgpointsD(index);

	//calc outer camera params
	cv::Mat camera_mat = cv::Mat(3, 3, CV_64FC1, main_scene_.getIntrinsicCameraMatrix().data());
	cv::Mat dcoeffs, rvec, rmat, tvec;

	main_scene_.calcOuterCameraParams(imgpoints, camera_mat, dcoeffs, rvec, tvec);
	cv::Rodrigues(rvec, rmat);

	//write result to main json
	if (main_json_["camera"].size() < index) //difference more than 1 element
		throw std::out_of_range("Too short main_json[\"camera\"] array");
	if (main_json_["camera"].size() == index) //difference exactly 1 element
		main_json_["camera"].push_back(json::object());
	writeCameraRMat(rmat, index);
	writeCameraTVec(tvec, index);
	writeCameraSVec({0., 0., 0.}, index);
	saveMainJSON();
}

////////// makeBackground //////////
void Generator::makeBackground(
	const std::string& video_filename, const std::string& bckg_filename) {
	cv::VideoCapture video(video_filename);

	if (!video.isOpened())
		throw std::exception("Error opening video stream or file");

	std::cout << "\nReading frames" << std::endl;
	cv::Mat curr_frame;
	std::vector<cv::Mat> frames;
	for (int i{};; ++i) {
		video >> curr_frame;
		if (curr_frame.empty())
			break;
		frames.push_back(curr_frame);
		cv::cvtColor(frames[i], frames[i], cv::COLOR_BGR2GRAY);
	}

	std::cout << "\nCalculating median" << std::endl;
	std::vector<uchar> pixel;
	cv::Mat background_image = cv::Mat(frames[0].size(), frames[0].type());
	for (int i{}; i < frames[0].cols; ++i) {
		for (int j{}; j < frames[0].rows; ++j) {
			for (int f{}; f < frames.size(); ++f) {
				pixel.push_back(frames[f].at<uchar>(j, i));
			}
			background_image.at<uchar>(j, i) = median(pixel);
			pixel.clear();
		}
	}

	cv::imwrite(bckg_filename, background_image);
	std::cout << "Background image is ready" << std::endl;
}

////////// makeTestTexture //////////
void Generator::makeTestTexture(const std::string& filename) {
	cv::Mat texture = cv::Mat::zeros(256, 256, CV_8UC3);
	texture += cv::Scalar{ 255, 255, 255 };

	//vertcal lines
	for (int i{}; i < texture.cols; i += 16) {
		cv::line(texture, { i, 0 }, { i, texture.rows - 1 }, { 0, 0, 0 });
	}

	//horizontal lines
	for (int j{}; j < texture.rows; j += 16) {
		cv::line(texture, { 0, j }, { texture.cols - 1, j }, { 0, 0, 0 });
	}

	//triangle
	int step = 40;
	std::vector<cv::Point> points;
	points.push_back(cv::Point( 0.5 * texture.cols - step, 0.5 * texture.rows + step ));
	points.push_back(cv::Point( 0.5 * texture.cols + step, 0.5 * texture.rows + step ));
	points.push_back(cv::Point( 0.5 * texture.cols, 0.5 * texture.rows - 1.5 * step));
	cv::polylines(texture, points, true, { 0, 0, 255 }, 3);

	//circles
	cv::circle(texture, points[0], 0.5 * step, { 0, 255, 0 }, 2);
	cv::circle(texture, points[1], 0.5 * step, { 255, 0, 0 }, 2);

	cv::imwrite(filename, texture);
}

////////// makeEdgeTextures //////////
void Generator::makeEdgeTextures(size_t index) {
	//get info from json
	std::string filename = readInputImage(index);
	cv::Mat src = cv::imread(filename, cv::IMREAD_GRAYSCALE);
	std::vector<cv::Point2i> imgpoints = readInputImgpointsI(index);

	cv::Mat test;
	cv::merge(std::array<cv::Mat, 3>{ src, src, src }, test);
	for (int i{}; i < 4; ++i)
		add_cv::cross(test, imgpoints[i], { 2, 2 }, { 255, 0, 0 });
	for (int i{4}; i < 8; ++i)
		add_cv::cross(test, imgpoints[i], { 2, 2 }, { 0, 0, 255 });

	//initialize dst images
	cv::Mat right_plane = cv::Mat::zeros(64, src.cols, CV_8UC1);
	cv::Mat left_plane  = cv::Mat::zeros(64, src.cols, CV_8UC1);
	cv::Mat upper_plane = cv::Mat::zeros(64, src.cols, CV_8UC1);
	cv::Mat lower_plane = cv::Mat::zeros(64, src.cols, CV_8UC1);
	cv::Mat bottom_plane = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
	
	cv::Mat homography;
	std::vector<cv::Point2i> plane_points{ 4 };
	std::vector<cv::Point2i> dst_points = {
		{0, 0},
		{right_plane.cols - 1, 0},
		{right_plane.cols - 1, right_plane.rows - 1},
		{0, right_plane.rows - 1} };
	
	////right
	plane_points = { imgpoints[5], imgpoints[6], imgpoints[2], imgpoints[1] };
	homography = cv::findHomography(plane_points, dst_points);
	cv::warpPerspective(src, right_plane, homography, right_plane.size());
	
	////left
	plane_points = { imgpoints[7], imgpoints[4], imgpoints[0], imgpoints[3] };
	homography = cv::findHomography(plane_points, dst_points);
	cv::warpPerspective(src, left_plane, homography, left_plane.size());
	
	//upper
	plane_points = { imgpoints[4], imgpoints[5], imgpoints[1], imgpoints[0] };
	homography = cv::findHomography(plane_points, dst_points);
	cv::warpPerspective(src, upper_plane, homography, upper_plane.size());
	
	//lower
	plane_points = { imgpoints[6], imgpoints[7], imgpoints[3], imgpoints[2] };
	homography = cv::findHomography(plane_points, dst_points);
	cv::warpPerspective(src, lower_plane, homography, lower_plane.size());
	
	//bottom
	plane_points = { imgpoints[0], imgpoints[1], imgpoints[2], imgpoints[3] };
	dst_points = { {0, 0},  {bottom_plane.cols, 0},
		{bottom_plane.cols, bottom_plane.rows}, {0, bottom_plane.rows} };
	homography = cv::findHomography(plane_points, dst_points);
	cv::warpPerspective(src, bottom_plane, homography, bottom_plane.size());

	//saving
	std::string directory = data_path_ + src_dir_ + edges_dir_;
	if (!std::filesystem::exists(directory))
		std::filesystem::create_directories(directory);

	cv::imwrite(directory + std::to_string(index) + right_edge_name_ + image_ending_, right_plane);
	cv::imwrite(directory + std::to_string(index) + left_edge_name_ + image_ending_, left_plane);
	cv::imwrite(directory + std::to_string(index) + upper_edge_name_ + image_ending_, upper_plane);
	cv::imwrite(directory + std::to_string(index) + lower_edge_name_ + image_ending_, lower_plane);
	cv::imwrite(directory + std::to_string(index) + bottom_edge_name_ + image_ending_, bottom_plane);
}

////////// makeCVDaphniaTexture //////////
void Generator::makeCVDaphniaTexture(size_t index, bool ovoid) {
	std::string directory;
	if (ovoid)
		directory = data_path_ + src_dir_ + daphnia_texture_dir_ + daphnia_ovoid_dir_;
	else
		directory = data_path_ + src_dir_ + daphnia_texture_dir_ + daphnia_circle_dir_;

	if (!std::filesystem::exists(directory))
		std::filesystem::create_directories(directory);

	const int  size = texture_size_;
	cv::Mat    img = cv::Mat::zeros(size, size, CV_8UC1);

	float      t = 0.3f;
	float      l = 1.565f;
	uint8_t    base = 200;
	float      gau_a = 1.0f;
	float      gau_c = 12.0f;
	const auto eps = std::numeric_limits<float>::epsilon();
	float      dx = (2.0f - 2 * eps) / img.cols;

	int radius = 0.72 * 0.5 * size;
	int center = 0.5 * size;

	float dy{}, gau{};
	int y_top{}, y_bottom{}, x{};
	if (ovoid)
		for (float fx = -1.0f + eps; fx < 1.0f; fx += dx) {
			dy = t * std::pow((1.0f + fx) / (1.0f - fx),
				(1.0f - l) / (2.0f * (1.0f + l))) * std::sqrt(1.0f - fx * fx);
			int y_top = img.rows * (0.45f - dy);
			int y_bottom = img.rows * (0.55f + dy);
			int x = fx / dx + img.cols / 2;
			cv::line(img, { x, y_top }, { x, y_bottom }, { double(base) }, 1);
		}
	else
		cv::circle(img, cv::Point{ center, center }, radius, { (double)base }, -1);

	cv::Mat imb_egg_dt;
	cv::distanceTransform(img, imb_egg_dt, cv::DIST_L1, 3);

	cv::Mat imf_egg = cv::Mat::zeros(size, size, CV_32FC1);
	int r{};
	if (ovoid)
		for (float fx = -1.0f + eps; fx < 1.0f; fx += dx) {
			dy = t * std::pow((1.0f + fx) / (1.0f - fx),
				(1.0f - l) / (2.0f * (1.0f + l))) *	std::sqrt(1.0f - fx * fx);
			y_top = img.rows * (0.45f - dy);
			y_bottom = img.rows * (0.55f + dy);
			x = fx / dx + img.cols / 2;
			cv::line(img, { x, y_top }, { x, y_bottom }, { double(base) }, 1);
			for (int y = y_top; y <= y_bottom; y += 1) {
				r = y - img.rows / 2;
				gau = gau_a * std::exp(-r * r / (2 * gau_c * gau_c));
				imf_egg.at<float>(y, x) = cv::saturate_cast<float>(base + (255 - base) * gau);
			}
		}
	else
		for (int x = center - radius; x < center + radius; ++x) {
			for (int y = center - radius; y < center + radius; ++y) {
				if (img.at<uchar>(y, x) < base)
					continue;
				r = std::sqrt((x - center) * (x - center) + (y - center) * (y - center));
				gau = gau_a * std::exp(-r * r / (2 * gau_c * gau_c));
				imf_egg.at<float>(y, x) = cv::saturate_cast<float>(base + (255 - base) * gau);
			}
		}

	imf_egg = imf_egg + imf_egg.mul(imb_egg_dt);
	cv::normalize(imf_egg, imf_egg, 1.0f, 0.0f, cv::NormTypes::NORM_MINMAX);
	cv::Mat1f noise_10(size / 10, size / 10);
	cv::Mat1f noise_25(size / 25, size / 25);

	float     m = 0.5;
	float     sigma = 0.5;
	cv::randn(noise_10, m, sigma);
	cv::randn(noise_25, m, sigma);

	cv::Mat1f noise_scaled;
	cv::resize(noise_25, noise_scaled, { size, size });

	cv::Mat1f noise = noise_scaled.clone();
	cv::resize(noise_10, noise_scaled, { size, size });
	noise += noise_scaled;

	imf_egg = imf_egg + imf_egg.mul(noise);
	//cv::resize(imf_egg, imf_egg, { 20, 20 }, 0.0, 0.0, cv::InterpolationFlags::INTER_LINEAR);
	cv::normalize(imf_egg, imf_egg, 1.0f, 0.0f, cv::NormTypes::NORM_MINMAX);

	cv::Mat imf_ret;
	imf_egg.convertTo(imf_ret, CV_8UC1, 255, 0);

	cv::imwrite(directory + std::to_string(index) + image_ending_, imf_ret);
}

////////// makeCVDaphniaMask //////////
void Generator::makeCVDaphniaMask(bool ovoid) {
	std::string directory;
	if (ovoid)
		directory = data_path_ + src_dir_ + daphnia_texture_dir_ + daphnia_ovoid_dir_;
	else
		directory = data_path_ + src_dir_ + daphnia_texture_dir_ + daphnia_circle_dir_;

	if (!std::filesystem::exists(directory))
		std::filesystem::create_directories(directory);

	const int  size = texture_size_;
	cv::Mat    img = cv::Mat::zeros(size, size, CV_8UC1);

	float      t = 0.3f;
	float      l = 1.565f;
	uint8_t    base = 255;
	const auto eps = std::numeric_limits<float>::epsilon();
	float      dx = (2.0f - 2 * eps) / img.cols;

	int radius = 0.72 * 0.5 * size;
	int center = 0.5 * size;

	float dy{};
	int y_top{}, y_bottom{}, x{};
	if (ovoid)
		for (float fx = -1.0f + eps; fx < 1.0f; fx += dx) {
			dy = t * std::pow((1.0f + fx) / (1.0f - fx),
				(1.0f - l) / (2.0f * (1.0f + l))) * std::sqrt(1.0f - fx * fx);
			int y_top = img.rows * (0.45f - dy);
			int y_bottom = img.rows * (0.55f + dy);
			int x = fx / dx + img.cols / 2;
			cv::line(img, { x, y_top }, { x, y_bottom }, { double(base) }, 1);
		}
	else
		cv::circle(img, cv::Point{ center, center }, radius, { (double)base }, -1);

	cv::imwrite(directory + "mask" + image_ending_, img);
}

////////// makeGLUTDaphniaTexture //////////
void Generator::makeGLUTDaphniaTexture(size_t index) {
	std::string directory = data_path_ + src_dir_ + daphnia_texture_dir_ + daphnia_glut_dir_;
	if (!std::filesystem::exists(directory))
		std::filesystem::create_directories(directory);
	
	const int  size = texture_size_;
	cv::Mat    img = cv::Mat::zeros(size + 2, size, CV_8UC1);

	float      t = 0.3f;
	float      l = 1.565f;
	uint8_t    base = 200;
	float      gau_a = 1.0f;
	float      gau_c = 12.0f;
	const auto eps = std::numeric_limits<float>::epsilon();
	float      dx = (2.0f - 2 * eps) / img.cols;
	float dy{}, gau{};
	int y_top{}, y_bottom{}, x{}, r{};
	double center = 0.5 * size;

	img += 255;
	cv::line(img, { 0, 0 }, { img.cols - 1, 0 }, { 0, 0, 0 });
	cv::line(img, { 0, img.rows - 1 }, { img.cols - 1, img.rows - 1 }, { 0, 0, 0 });

	//horizontal heatmap - most brightness in the center
	cv::Mat img_dt;
	cv::distanceTransform(img, img_dt, cv::DIST_L1, 3);
	img_dt = img_dt(cv::Range(1, img_dt.rows - 1), cv::Range(0, img_dt.cols));

	//saturate casting
	cv::Mat img_base = cv::Mat::zeros(size, size, CV_32FC1);
	for (int y = 0; y < img_dt.rows; ++y) {
		r = abs(y - center);
		gau = gau_a * std::exp(-r * r / (2 * gau_c * gau_c));
		cv::line(img_base, { 0, y }, { img_dt.cols - 1, y }, cv::saturate_cast<float>( base + (255 - base) * gau ));
	}

	img_base = img_base + img_base.mul(img_dt);
	cv::normalize(img_base, img_base, 1.0f, 0.0f, cv::NormTypes::NORM_MINMAX);

	//adding noises of different sizes
	float     m = 0.5;
	float     sigma = 0.5;
	cv::Mat1f noise_10(size / 10, size / 10);
	cv::Mat1f noise_25(size / 25, size / 25);
	cv::Mat1f noise_scaled;
	cv::randn(noise_10, m, sigma);
	cv::randn(noise_25, m, sigma);

	cv::resize(noise_25, noise_scaled, { size, size });
	cv::Mat1f noise = noise_scaled.clone();

	cv::resize(noise_10, noise_scaled, { size, size });
	noise += noise_scaled;

	img_base = img_base + img_base.mul(noise);
	
	//making target texture from generated base
	//vertical center element
	cv::Mat dst = cv::Mat::zeros(size, 2 * size, CV_32FC1);
	cv::Mat img_base_res = cv::Mat::zeros(img_base.size(), img_base.type());
	cv::rotate(img_base, img_base_res, cv::ROTATE_90_CLOCKWISE);
	img_base_res.copyTo(dst(cv::Range(0, size), cv::Range(0.5 * size, 1.5 * size)));

	//vetical side elements
	cv::resize(img_base_res, img_base_res, { (int)(0.5 * size), size }, 0, 0, cv::INTER_LINEAR);
	img_base_res.copyTo(dst(cv::Range(0, size), cv::Range(0, 0.5 * size)));
	img_base_res.copyTo(dst(cv::Range(0, size), cv::Range(1.5 * size, 2 * size)));
	
	//horizontal darker part - head
	img_base(cv::Range(0.5 * size, size), cv::Range(0, size)).copyTo(img_base_res);
	img_base_res = img_base_res.mul(img_dt(cv::Range(0.5 * size, size), cv::Range(0, size))) / (0.5 * size);
	cv::resize(img_base_res, img_base_res, {2 * size, size}, 0, 0, cv::INTER_LINEAR);
	dst(cv::Range(0, img_base_res.rows), cv::Range(0, 2 * size))
		= cv::max(img_base_res, dst(cv::Range(0, img_base_res.rows), cv::Range(0, img_base_res.cols)));

	//horizontal lighter part - tail
	cv::Mat img_dt_res = img_dt(cv::Range(0.5 * size, size), cv::Range(0, size)).clone();
	cv::resize(img_dt_res, img_dt_res, {2 * size, (int)(0.6 * size)}, 0, 0, cv::INTER_LINEAR);
	cv::normalize(img_dt_res, img_dt_res, 1.0f, 0.0f, cv::NormTypes::NORM_MINMAX);
	dst(cv::Range(size - img_dt_res.rows, size), cv::Range(0, 2 * size))
		= dst(cv::Range(size - img_dt_res.rows, size), cv::Range(0, 2 * size)).mul(img_dt_res);

	//writing texture template
	cv::normalize(dst, dst, 1.0f, 0.5f, cv::NormTypes::NORM_MINMAX);
	dst.convertTo(dst, CV_8UC1, 255, 0);
	cv::flip(dst, dst, 0);
	
	cv::imwrite(directory + std::to_string(index) + image_ending_, dst);
}

////////// processDaphniaTexture //////////
void Generator::processDaphniaTexture(int size,
	const std::string& src_texture_filename,
	const cv::Mat& background_sup,
	const std::array<double, 2>& center,
	const std::string& dst_texture_filename) {

	cv::Mat texture = cv::imread(src_texture_filename, cv::IMREAD_GRAYSCALE);

	//roi and mean roi
	cv::Rect rect_roi{
		(int)(center[0]), (int)(center[1]), size, size };
	cv::Mat bckg_roi = background_sup(rect_roi);
	cv::Mat mean_roi = cv::Mat::zeros(bckg_roi.size(), bckg_roi.type());
	mean_roi += cv::mean(bckg_roi).val[0];

	//level off brightness
	double scale_x{ 1. * texture.cols / size };
	double scale_y{ 1. * texture.rows / size };
	double min_val, max_val;
	int tj, tk;
	cv::minMaxIdx(background_sup, &min_val, &max_val);
	for (int k = 0; k < size; ++k) {
		for (int j = 0; j < size; ++j) {
			for (int sx{}; sx < scale_x; ++sx) {
				for (int sy{}; sy < scale_y; ++sy) {
					tj = scale_y * j + sy;
					tk = scale_x * k + sx;
					if ((double)texture.at<uchar>(tj, tk) < min_val)
						texture.at<uchar>(tj, tk) = min_val;
				}
			}
		}
	}

	cv::Mat textured_roi = mean_roi.clone();
	for (int k = 0; k < size; ++k) {
		for (int j = 0; j < size; ++j) {
			for (int sx{}; sx < scale_x; ++sx) {
				for (int sy{}; sy < scale_y; ++sy) {
					tj = scale_y * j + sy;
					tk = scale_x * k + sx;
					texture.at<uchar>(tj, tk) = cv::saturate_cast<uchar>(
						1. / 255 * (double)texture.at<uchar>(tj, tk) * (
							(double)mean_roi.at<uchar>(j, k) - 0.5 *
							(double)texture.at<uchar>(tj, tk)) +
						1. / 255 * (255 - (double)texture.at<uchar>(tj, tk))
						* (double)background_sup.at<uchar>( center[1] + j, center[0] + k));
				}
			}
		}
	}

	for (int k = 0; k < size; ++k) {
		for (int j = 0; j < size; ++j) {
			for (int sx{}; sx < scale_x; ++sx) {
				for (int sy{}; sy < scale_y; ++sy) {
					tj = scale_y * j + sy;
					tk = scale_x * k + sx;
					texture.at<uchar>(tj, tk) = std::min(
						int(background_sup.at<uchar>(center[1] + j, center[0] + k)),
						int(texture.at<uchar>(tj, tk)));
				}
			}
		}
	}

	//write result
	cv::imwrite(dst_texture_filename, texture);
}

////////// predictPoints //////////
void Generator::predictPoints(std::vector<cv::Point2d>& imgpoints,
	const std::vector<cv::Point3d>& objpoints,
	const std::array<double, 9>& cmat,
	const std::array<double, 9>& rmat,
	const std::array<double, 3>& tvec) {

	cv::Mat cv_cmat = cv::Mat(3, 3, CV_64FC1, (double*)cmat.data());
	cv::Mat cv_rmat = cv::Mat(3, 3, CV_64FC1, (double*)rmat.data());
	cv::Mat cv_tvec = cv::Mat(3, 1, CV_64FC1, (double*)tvec.data());

	cv::Mat objpoint_mat;
	cv::Mat imgpoint_mat;
	cv::Point3d imgpoint;
	std::vector<cv::Point3d> temp;
	for (auto& objpoint : objpoints) {

		//convert point to mat
		temp = { objpoint };
		temp[0].z = -temp[0].z;
		objpoint_mat = cv::Mat(3, 1, CV_64FC1, temp.data());

		//calc prediction mat
		imgpoint_mat = cv_cmat * (cv_rmat * objpoint_mat + cv_tvec);

		//build point from prediction
		imgpoint.x = imgpoint_mat.at<double>(0, 0);
		imgpoint.y = imgpoint_mat.at<double>(1, 0);
		imgpoint.z = imgpoint_mat.at<double>(2, 0);
		imgpoints.push_back({ imgpoint.x / imgpoint.z, imgpoint.y / imgpoint.z });
	
	}

}

////////// predictPoints //////////
void Generator::predictPoints(std::vector<std::vector<std::array<double, 2>>>& imgpoints,
	const std::vector<std::vector<std::array<double, 3>>>& objpoints,
	const std::array<double, 9>& cmat,
	const std::array<double, 9>& rmat,
	const std::array<double, 3>& tvec,
	const std::array<double, 3>& svec) {

	cv::Mat cv_cmat = cv::Mat(3, 3, CV_64FC1, (double*)cmat.data());
	cv::Mat cv_rmat = cv::Mat(3, 3, CV_64FC1, (double*)rmat.data());
	cv::Mat cv_tvec = cv::Mat(3, 1, CV_64FC1, (double*)tvec.data());

	cv::Mat objpoint_mat;
	cv::Mat imgpoint_mat;
	cv::Point3d imgpoint;
	std::array<double, 3> point;

	imgpoints = std::vector<std::vector<std::array<double, 2>>>{objpoints.size()};
	for (int frame = {}; frame < objpoints.size(); ++frame) {
		imgpoints[frame] = std::vector<std::array<double, 2>>{objpoints[frame].size()};
		for (int object = {}; object < objpoints[frame].size(); ++object) {
			point = objpoints[frame][object];
			point[1] = -point[1];
			point[2] = -point[2] - svec[2];

			//convert point to mat
			objpoint_mat = cv::Mat(3, 1, CV_64FC1, (double*)point.data());

			//calc prediction mat
			imgpoint_mat = cv_cmat * (cv_rmat * objpoint_mat + cv_tvec);

			//build point from prediction
			imgpoint.x = imgpoint_mat.at<double>(0, 0);
			imgpoint.y = imgpoint_mat.at<double>(1, 0);
			imgpoint.z = imgpoint_mat.at<double>(2, 0);
			imgpoints[frame][object] = { imgpoint.x / imgpoint.z, imgpoint.y / imgpoint.z };
		}
	}
}

////////// predictDirections //////////
void Generator::predictDirections(
	std::vector<std::vector<std::array<double, 2>>>& imgdirections,
	std::vector<std::vector<std::array<double, 3>>> objdirections,
	const std::vector<std::vector<std::array<double, 2>>>& imgpoints,
	const std::vector<std::vector<std::array<double, 3>>>& objpoints,
	const std::array<double, 9>& cmat,
	const std::array<double, 9>& rmat,
	const std::array<double, 3>& tvec,
	const std::array<double, 3>& svec) {

	//points for predict - end point of vector
	for (int frame{}; frame < objpoints.size(); ++frame) {
		for (int object{}; object < objpoints[frame].size(); ++object) {
			objdirections[frame][object][0] += objpoints[frame][object][0];
			objdirections[frame][object][1] += objpoints[frame][object][1];
			objdirections[frame][object][2] += objpoints[frame][object][2];
		}
	}

	//predict end point
	predictPoints(imgdirections, objdirections, cmat, rmat, tvec, svec);

	//recalc predicted point as vector length
	for (int frame{}; frame < objpoints.size(); ++frame) {
		for (int object{}; object < objpoints[frame].size(); ++object) {
			imgdirections[frame][object][0] -= imgpoints[frame][object][0];
			imgdirections[frame][object][1] -= imgpoints[frame][object][1];
		}
	}
}

////////// showPointGrid //////////
void Generator::showPointGrid(size_t index, const cv::Size& quantity, double z,
	const std::array<double, 3>& shift, bool save){
	//read info from json
	std::string image_filename = readInputImage(index);
	std::array<double, 9> rmat = readCameraRMat(index);
	std::array<double, 3> tvec = readCameraTVec(index);
	main_scene_.setCameraRMat(rmat);
	main_scene_.setCameraTVec(tvec);
	main_scene_.setCameraSVec(shift);

	std::cout << "Rotation matrix:\n" << rmat << std::endl;
	std::cout << "Translation vector:\t" << tvec << std::endl;
	std::cout << "Shift vector:\t\t" << shift << std::endl;

	//filenames
	std::string grid_glut_filename = data_path_ + grid_dir_ + grid_glut_name_ + image_ending_;
	std::string grid_merged_filename = data_path_ + grid_dir_ + grid_merged_name_ + image_ending_;

	//constructing 3D points
	std::vector<cv::Point3d> objgridpoints;
	double aquarium_width = main_scene_.getAquariumSize()[0];
	double aquarium_height = main_scene_.getAquariumSize()[1];
	double width_step = aquarium_width / (quantity.width - 1);
	double height_step = aquarium_height / (quantity.height - 1);
	for (int i{}; i < quantity.width; ++i) {
		for (int j{}; j < quantity.height; ++j) {
			objgridpoints.push_back({
				(-0.5 * aquarium_width + i * width_step),
				(-0.5 * aquarium_height + j * height_step), z });
		}
	}
	main_scene_.setObjGridPoints(objgridpoints);
	main_scene_.setGridFilename(grid_glut_filename);

	std::cout << "\nHorizontal values: \t";
	for (int i{}; i < objgridpoints.size(); i += quantity.height)
		std::cout << objgridpoints[i].x << ", ";

	std::cout << "\nVertical values: \t";
	for (int i{}; i < quantity.height; ++i)
		std::cout << objgridpoints[i].y << ", ";
	std::cout << std::endl;

	//glut rendering 3D points
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(
		main_scene_.getRenderImageSize().width,
		main_scene_.getRenderImageSize().height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Point array");

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	curr_this_ = this;
	glutDisplayFunc(Generator::displayPointGrid);
	glutReshapeFunc(Generator::reshape);
	main_scene_.initGLUT();
	glutMainLoop();

	//predicting 2D points
	std::vector<cv::Point2d> imggridpoints;
	predictPoints(imggridpoints, objgridpoints,
		main_scene_.getIntrinsicCameraMatrix(), rmat, tvec);

	//opencv & opengl merging
	cv::Mat grid_glut = cv::imread(grid_glut_filename, cv::IMREAD_GRAYSCALE);
	cv::Mat src = cv::imread(image_filename, cv::IMREAD_GRAYSCALE);

	cv::Mat grid_merged_1c;
	cv::bitwise_and(src, grid_glut, grid_merged_1c, grid_glut);

	cv::Mat grid_merged_3c;
	cv::merge(std::array<cv::Mat, 3>
		{ grid_merged_1c, grid_merged_1c, grid_merged_1c }, grid_merged_3c);

	for (auto& point : imggridpoints)
		add_cv::cross(grid_merged_3c, point, { 2, 2 }, { 0, 0, 255 });
	cv::imwrite(grid_merged_filename, grid_merged_3c);

	if (save) {
		writeCameraSVec(shift, index);
		saveMainJSON();
	}

	std::cout << "Successful end of program!" << std::endl;
}

////////// showPointGrid //////////
void Generator::showPointGrid(size_t index, const cv::Size& quantity, double z,
	size_t file_index, const std::array<double, 3>& shift, bool save) {
	//read info from json
	std::string image_filename = readInputImage(index);
	std::array<double, 9> rmat = readCameraRMat(index);
	std::array<double, 3> tvec = readCameraTVec(index);
	main_scene_.setCameraRMat(rmat);
	main_scene_.setCameraTVec(tvec);
	main_scene_.setCameraSVec(shift);

	std::cout << "Rotation matrix:\n" << rmat << std::endl;
	std::cout << "Translation vector:\t" << tvec << std::endl;
	std::cout << "Shift vector:\t\t" << shift << std::endl;

	//filetree
	namespace fs = std::filesystem;
	if (!fs::exists(data_path_ + grid_dir_ + frames_glut_dir_))
		fs::create_directories(data_path_ + grid_dir_ + frames_glut_dir_);
	if (!fs::exists(data_path_ + grid_dir_ + frames_merged_dir_))
		fs::create_directories(data_path_ + grid_dir_ + frames_merged_dir_);

	std::string grid_glut_filename = data_path_ + grid_dir_ + frames_glut_dir_
		+ std::to_string(file_index) + image_ending_;
	std::string grid_merged_filename = data_path_ + grid_dir_ + frames_merged_dir_
		+ std::to_string(file_index) + image_ending_;

	//constructing 3D points
	std::vector<cv::Point3d> objgridpoints;
	double aquarium_width = main_scene_.getAquariumSize()[0];
	double aquarium_height = main_scene_.getAquariumSize()[1];
	double width_step = aquarium_width / (quantity.width - 1);
	double height_step = aquarium_height / (quantity.height - 1);
	for (int i{}; i < quantity.width; ++i) {
		for (int j{}; j < quantity.height; ++j) {
			objgridpoints.push_back({
				(-0.5 * aquarium_width + i * width_step),
				(-0.5 * aquarium_height + j * height_step), z });
		}
	}
	main_scene_.setObjGridPoints(objgridpoints);
	main_scene_.setGridFilename(grid_glut_filename);

	std::cout << "\nHorizontal values: \t";
	for (int i{}; i < objgridpoints.size(); i += quantity.height)
		std::cout << objgridpoints[i].x << ", ";

	std::cout << "\nVertical values: \t";
	for (int i{}; i < quantity.height; ++i)
		std::cout << objgridpoints[i].y << ", ";
	std::cout << std::endl;

	//glut rendering 3D points
	if (!glutGet(GLUT_INIT_STATE))
		glutInit(&argc_, argv_);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(
		main_scene_.getRenderImageSize().width,
		main_scene_.getRenderImageSize().height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Point array");

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	curr_this_ = this;
	glutDisplayFunc(Generator::displayPointGrid);
	glutReshapeFunc(Generator::reshape);
	main_scene_.initGLUT();
	glutMainLoop();

	//predicting 2D points
	std::vector<cv::Point2d> imggridpoints;
	predictPoints(imggridpoints, objgridpoints,
		main_scene_.getIntrinsicCameraMatrix(), rmat, tvec);

	//opencv & opengl merging
	cv::Mat grid_glut = cv::imread(grid_glut_filename, cv::IMREAD_GRAYSCALE);
	cv::Mat src = cv::imread(image_filename, cv::IMREAD_GRAYSCALE);

	cv::Mat grid_merged_1c;
	cv::bitwise_and(src, grid_glut, grid_merged_1c, grid_glut);

	cv::Mat grid_merged_3c;
	cv::merge(std::array<cv::Mat, 3>
	{ grid_merged_1c, grid_merged_1c, grid_merged_1c }, grid_merged_3c);

	for (auto& point : imggridpoints)
		add_cv::cross(grid_merged_3c, point, { 2, 2 }, { 0, 0, 255 });
	cv::imwrite(grid_merged_filename, grid_merged_3c);

	if (save) {
		writeCameraSVec(shift, index);
		saveMainJSON();
	}

	std::cout << "Successful end of program!" << std::endl;
}

////////// genUntexturedRandomClip //////////
void Generator::genUntexturedRandomClip(size_t index, size_t num_frames,
	const std::array<double,2>& num_objects_range,
	const std::array<double, 2>& size_objects_range,
	std::string path) {

	//read info from json and set params
	std::string image_filename = readInputImage(index);
	std::array<double, 9> rmat = readCameraRMat(index);
	std::array<double, 3> tvec = readCameraTVec(index);
	std::array<double, 3> svec = readCameraSVec(index);
	main_scene_.setCameraRMat(rmat);
	main_scene_.setCameraTVec(tvec);
	main_scene_.setCameraSVec(svec);

	std::cout << "Rotation matrix:\n" << rmat << std::endl;
	std::cout << "Translation vector:\t" << tvec << std::endl;
	std::cout << "Shift vector:\t\t" << svec << std::endl;

	//set directories for generation
	if (path.empty()) {
		path = data_path_;
	}
	else if (path.compare(path.size() - 1, 1, "/")) {
		std::cout << "Warning: path ends not with \"/\" and will be supplemented" << std::endl;
		path += "/";
	}
	std::string gen_glut_dir = path + RCO_generation_main_dir_ + generation_frames_dir_ + frames_glut_dir_;
	std::string gen_merged_dir = path + RCO_generation_main_dir_ + generation_frames_dir_ + frames_merged_dir_;
	std::string gen_json_dir = path + RCO_generation_main_dir_ + generation_json_dir_;
	main_scene_.setGenFramesPath(gen_glut_dir);
	makeGenFileTree(data_path_, RCO_generation_main_dir_,
		generation_frames_dir_ + frames_glut_dir_,
		generation_frames_dir_ + frames_merged_dir_, generation_json_dir_);

	//construct 3D params - coords, angles
	std::array<double, 3> aq_size = main_scene_.getAquariumSize();
	size_t num_objects;
	std::uniform_int_distribution<>	num_obj;

	if (num_objects_range[0] == num_objects_range[1])
		num_objects = num_objects_range[0];
	else
		num_obj = std::uniform_int_distribution<>(num_objects_range[0], num_objects_range[1]);
	std::uniform_real_distribution<> angles_dis(0., 180.);
	std::normal_distribution<> size_dis(
		(size_objects_range[0] + size_objects_range[1]) * 0.5,
		(size_objects_range[1] - size_objects_range[0]) / 6);
	std::uniform_real_distribution<> x_dis;
	std::uniform_real_distribution<> y_dis;
	std::uniform_real_distribution<> z_dis;

	std::array<double, 3> buffer;
	double curr_dl;
	double curr_scale;
	main_scene_.setRCOFrames(num_frames);
	std::vector<std::vector<std::array<double, 3>>> objdirections{ num_frames };
	std::cout << "\nGenerating 3D params" << std::endl;
	for (int frame = {}; frame < num_frames; ++frame) {
		if (num_objects_range[0] != num_objects_range[1])
			num_objects = num_obj(rd_);
		main_scene_.setRCOObjects(frame, num_objects);
		objdirections[frame] = std::vector<std::array<double, 3>>{ num_objects };
		for (int object = {}; object < num_objects; ++object) {
			//gen size scale
			curr_scale = normDistGenInRange(size_dis, size_objects_range[0], size_objects_range[1]);
			main_scene_.setRCODaphniaScale(frame, object, curr_scale);
			curr_dl = 0.5 * main_scene_.getRCODaphniaLength(frame, object);

			//set distributions' range by length of object
			x_dis = std::uniform_real_distribution<>(-0.5 * aq_size[0] + curr_dl, 0.5 * aq_size[0] - curr_dl);
			y_dis = std::uniform_real_distribution<>(-0.5 * aq_size[1] + curr_dl, 0.5 * aq_size[1] - curr_dl);
			z_dis = std::uniform_real_distribution<>(-aq_size[2] + curr_dl, 0 - curr_dl);

			//gen coords - x, y, z
			buffer = { x_dis(rd_), y_dis(rd_), z_dis(rd_) };
			main_scene_.setRCODaphniaCoords(frame, object, buffer);

			//gen angles - apha, beta, gamma
			buffer = { angles_dis(rd_), angles_dis(rd_), angles_dis(rd_) };
			main_scene_.setRCODaphniaAngles(frame, object, buffer);
			objdirections[frame][object] = main_scene_.setRCODaphniaDirection(frame, object);
		}
	}

	//calc 2D coords from 3D
	std::cout << "Calculating 2D coords" << std::endl;
	std::vector<std::vector<std::array<double, 3>>> objpoints{num_frames};
	for (int frame{}; frame < num_frames; ++frame) {
		num_objects = main_scene_.getRCOObjectsNum(frame);
		objpoints[frame] = std::vector<std::array<double, 3>>{ num_objects };
		for (int object = {}; object < num_objects; ++object) {
			objpoints[frame][object] =
				main_scene_.getRCODaphniaCoords(frame, object);
		}
	}

	std::vector<std::vector<std::array<double, 2>>> imgpoints;
	std::vector<std::vector<std::array<double, 2>>> imgdirections{ num_frames };
	predictPoints(imgpoints, objpoints,
		main_scene_.getIntrinsicCameraMatrix(), rmat, tvec, svec);
	predictDirections(imgdirections, objdirections, imgpoints, objpoints,
		main_scene_.getIntrinsicCameraMatrix(), rmat, tvec, svec);

	//saving objoints and imgpoints to json
	saveGenCOJSON(gen_json_dir, objpoints, objdirections, imgpoints, imgdirections);

	//glut rendering
	std::cout << "GLUT rendering" << std::endl;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(
		main_scene_.getRenderImageSize().width,
		main_scene_.getRenderImageSize().height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Point array");

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	curr_this_ = this;
	glutDisplayFunc(Generator::displayUntexturedRandomClip);
	glutReshapeFunc(Generator::reshape);
	main_scene_.resetFrameCount();
	main_scene_.initGLUT();
	glutMainLoop();

	//merging glut image with src and imgpoints
	std::cout << "OpenCV merging" << std::endl;
	for (int frame{}; frame < objpoints.size(); ++frame)
		add_cv::mergeUntexturedImageAndPoints(image_filename, imgpoints[frame],
			gen_glut_dir + std::to_string(frame) + ".png",
			gen_merged_dir + std::to_string(frame) + ".png");
	
	std::cout << "Successful end of program!" << std::endl;
}

////////// genUntexturedRandomClip //////////
void Generator::genUntexturedRandomClip(
	const std::string& config_filename,	std::string path) {

	//read params from config json
	size_t index;
	size_t num_frames;
	std::array<double, 2> num_objects_range;
	std::array<double, 2> size_objects_range;
	readConfigRCOJSON(index, num_frames, num_objects_range, size_objects_range, config_filename);

	//read info from main json and set params
	std::string image_filename = readInputImage(index);
	std::array<double, 9> rmat = readCameraRMat(index);
	std::array<double, 3> tvec = readCameraTVec(index);
	std::array<double, 3> svec = readCameraSVec(index);
	main_scene_.setCameraRMat(rmat);
	main_scene_.setCameraTVec(tvec);
	main_scene_.setCameraSVec(svec);

	std::cout << "Rotation matrix:\n" << rmat << std::endl;
	std::cout << "Translation vector:\t" << tvec << std::endl;
	std::cout << "Shift vector:\t\t" << svec << std::endl;

	//set directories for generation
	if (path.empty()) {
		path = data_path_;
	}
	else if (path.compare(path.size() - 1, 1, "/")) {
		std::cout << "Warning: path ends not with \"/\" and will be supplemented" << std::endl;
		path += "/";
	}
	std::string gen_glut_dir = path + RCO_generation_main_dir_ + generation_frames_dir_ + frames_glut_dir_;
	std::string gen_merged_dir = path + RCO_generation_main_dir_ + generation_frames_dir_ + frames_merged_dir_;
	std::string gen_json_dir = path + RCO_generation_main_dir_ + generation_json_dir_;
	main_scene_.setGenFramesPath(gen_glut_dir);
	makeGenFileTree(data_path_, RCO_generation_main_dir_,
		generation_frames_dir_ + frames_glut_dir_,
		generation_frames_dir_ + frames_merged_dir_, generation_json_dir_);

	//construct 3D params - coords, angles
	std::array<double, 3> aq_size = main_scene_.getAquariumSize();
	size_t num_objects;
	std::uniform_int_distribution<>	num_obj;

	if (num_objects_range[0] == num_objects_range[1])
		num_objects = num_objects_range[0];
	else
		num_obj = std::uniform_int_distribution<>(num_objects_range[0], num_objects_range[1]);
	std::uniform_real_distribution<> angles_dis(0., 180.);
	std::normal_distribution<> size_dis(
		(size_objects_range[0] + size_objects_range[1]) * 0.5,
		(size_objects_range[1] - size_objects_range[0]) / 6);
	std::uniform_real_distribution<> x_dis;
	std::uniform_real_distribution<> y_dis;
	std::uniform_real_distribution<> z_dis;

	std::array<double, 3> buffer;
	double curr_dl;
	double curr_scale;
	main_scene_.setRCOFrames(num_frames);
	std::vector<std::vector<std::array<double, 3>>> objdirections{ num_frames };
	std::cout << "\nGenerating 3D params" << std::endl;
	for (int frame = {}; frame < num_frames; ++frame) {
		if (num_objects_range[0] != num_objects_range[1])
			num_objects = num_obj(rd_);
		main_scene_.setRCOObjects(frame, num_objects);
		objdirections[frame] = std::vector<std::array<double, 3>>{ num_objects };
		for (int object = {}; object < num_objects; ++object) {
			//gen size scale
			curr_scale = normDistGenInRange(size_dis, size_objects_range[0], size_objects_range[1]);
			main_scene_.setRCODaphniaScale(frame, object, curr_scale);
			curr_dl = 0.5 * main_scene_.getRCODaphniaLength(frame, object);

			//set distributions' range by length of object
			x_dis = std::uniform_real_distribution<>(-0.5 * aq_size[0] + curr_dl, 0.5 * aq_size[0] - curr_dl);
			y_dis = std::uniform_real_distribution<>(-0.5 * aq_size[1] + curr_dl, 0.5 * aq_size[1] - curr_dl);
			z_dis = std::uniform_real_distribution<>(-aq_size[2] + curr_dl, 0 - curr_dl);

			//gen coords - x, y, z
			buffer = { x_dis(rd_), y_dis(rd_), z_dis(rd_) };
			main_scene_.setRCODaphniaCoords(frame, object, buffer);

			//gen angles - apha, beta, gamma
			buffer = { angles_dis(rd_), angles_dis(rd_), angles_dis(rd_) };
			main_scene_.setRCODaphniaAngles(frame, object, buffer);
			objdirections[frame][object] = main_scene_.setRCODaphniaDirection(frame, object);
		}
	}

	//calc 2D coords from 3D
	std::cout << "Calculating 2D coords" << std::endl;
	std::vector<std::vector<std::array<double, 3>>> objpoints{ num_frames };
	for (int frame{}; frame < num_frames; ++frame) {
		num_objects = main_scene_.getRCOObjectsNum(frame);
		objpoints[frame] = std::vector<std::array<double, 3>>{ num_objects };
		for (int object = {}; object < num_objects; ++object) {
			objpoints[frame][object] =
				main_scene_.getRCODaphniaCoords(frame, object);
		}
	}

	std::vector<std::vector<std::array<double, 2>>> imgpoints;
	std::vector<std::vector<std::array<double, 2>>> imgdirections{ num_frames };
	predictPoints(imgpoints, objpoints,
		main_scene_.getIntrinsicCameraMatrix(), rmat, tvec, svec);
	predictDirections(imgdirections, objdirections, imgpoints, objpoints,
		main_scene_.getIntrinsicCameraMatrix(), rmat, tvec, svec);

	//saving objoints and imgpoints to json
	saveGenCOJSON(gen_json_dir, objpoints, objdirections, imgpoints, imgdirections);

	//glut rendering
	std::cout << "GLUT rendering" << std::endl;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(
		main_scene_.getRenderImageSize().width,
		main_scene_.getRenderImageSize().height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Point array");

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	curr_this_ = this;
	glutDisplayFunc(Generator::displayUntexturedRandomClip);
	glutReshapeFunc(Generator::reshape);
	main_scene_.resetFrameCount();
	main_scene_.initGLUT();
	glutMainLoop();

	//merging glut image with src and imgpoints
	std::cout << "OpenCV merging" << std::endl;
	for (int frame{}; frame < objpoints.size(); ++frame)
		add_cv::mergeUntexturedImageAndPoints(image_filename, imgpoints[frame],
			gen_glut_dir + std::to_string(frame) + ".png",
			gen_merged_dir + std::to_string(frame) + ".png");

	std::cout << "Successful end of program!" << std::endl;
}

////////// genTexturedRandomClip //////////
void Generator::genTexturedRandomClip(
	const std::string& config_filename, std::string path) {
	//read params from config json
	size_t index;
	size_t num_frames;
	std::array<double, 2> num_objects_range;
	std::array<double, 2> size_objects_range;
	readConfigRCOJSON(index, num_frames, num_objects_range, size_objects_range, config_filename);

	//read info from main json and set params
	std::string image_filename = readInputImage(index);
	std::array<double, 9> rmat = readCameraRMat(index);
	std::array<double, 3> tvec = readCameraTVec(index);
	std::array<double, 3> svec = readCameraSVec(index);
	main_scene_.setCameraRMat(rmat);
	main_scene_.setCameraTVec(tvec);
	main_scene_.setCameraSVec(svec);

	std::cout << "Rotation matrix:\n" << rmat << std::endl;
	std::cout << "Translation vector:\t" << tvec << std::endl;
	std::cout << "Shift vector:\t\t" << svec << std::endl;

	//set directories for generation
	if (path.empty()) {
		path = data_path_;
	}
	else if (path.compare(path.size() - 1, 1, "/")) {
		std::cout << "Warning: path ends not with \"/\" and will be supplemented" << std::endl;
		path += "/";
	}
	std::string gen_glut_dir = path + RCO_generation_main_dir_ + generation_frames_dir_ + frames_glut_dir_;
	std::string gen_merged_dir = path + RCO_generation_main_dir_ + generation_frames_dir_ + frames_merged_dir_;
	std::string gen_mask_dir = path + RCO_generation_main_dir_ + generation_frames_dir_ + frames_mask_dir_;
	std::string gen_texture_dir = path + RCO_generation_main_dir_ + generation_textures_dir_;
	std::string gen_json_dir = path + RCO_generation_main_dir_ + generation_json_dir_;
	main_scene_.setGenFramesPath(gen_glut_dir);
	main_scene_.setGenMasksPath(gen_mask_dir);
	makeGenFileTree(data_path_, RCO_generation_main_dir_,
		generation_frames_dir_ + frames_glut_dir_,
		generation_frames_dir_ + frames_merged_dir_,
		generation_frames_dir_ + frames_mask_dir_,
		gen_texture_dir, generation_json_dir_);

	//construct 3D params - coords, angles
	std::array<double, 3> aq_size = main_scene_.getAquariumSize();
	size_t num_objects;
	double object_size;
	std::uniform_int_distribution<>	num_obj;
	std::normal_distribution<> size_dis;

	if (num_objects_range[0] == num_objects_range[1])
		num_objects = num_objects_range[0];
	else
		num_obj = std::uniform_int_distribution<>(num_objects_range[0], num_objects_range[1]);
	if ((size_objects_range[0] - size_objects_range[1]) < std::numeric_limits<double>::epsilon())
		object_size = size_objects_range[0];
	else
		size_dis = std::normal_distribution<>(
			(size_objects_range[0] + size_objects_range[1]) * 0.5,
			(size_objects_range[1] - size_objects_range[0]) / 6);

	std::uniform_real_distribution<> angles_dis(0., 180.);
	std::uniform_real_distribution<> x_dis;
	std::uniform_real_distribution<> y_dis;
	std::uniform_real_distribution<> z_dis;

	std::array<double, 3> buffer;
	double curr_dl;
	double curr_scale;
	main_scene_.setRCOFrames(num_frames);
	std::vector<std::vector<std::array<double, 3>>> objdirections{ num_frames };
	std::cout << "\nGenerating 3D params" << std::endl;
	for (int frame = {}; frame < num_frames; ++frame) {

		if (num_objects_range[0] != num_objects_range[1])
			num_objects = num_obj(rd_);
		main_scene_.setRCOObjects(frame, num_objects);
		objdirections[frame] = std::vector<std::array<double, 3>>{num_objects};

		for (int object = {}; object < num_objects; ++object) {
			//gen size scale
			if ((size_objects_range[0] - size_objects_range[1]) < std::numeric_limits<double>::epsilon())
				curr_scale = object_size;
			else
				curr_scale = normDistGenInRange(size_dis, size_objects_range[0], size_objects_range[1]);
			main_scene_.setRCODaphniaScale(frame, object, curr_scale);
			curr_dl = 0.5 * main_scene_.getRCODaphniaLength(frame, object);

			//set distributions' range by length of object
			x_dis = std::uniform_real_distribution<>(-0.5 * aq_size[0] + curr_dl, 0.5 * aq_size[0] - curr_dl);
			y_dis = std::uniform_real_distribution<>(-0.5 * aq_size[1] + curr_dl, 0.5 * aq_size[1] - curr_dl);
			z_dis = std::uniform_real_distribution<>(-aq_size[2] + curr_dl, 0 - curr_dl);

			//gen coords - x, y, z
			buffer = { x_dis(rd_), y_dis(rd_), z_dis(rd_) };
			//buffer = { 0, 0, -1. };
			main_scene_.setRCODaphniaCoords(frame, object, buffer);

			//gen angles - apha, beta, gamma
			buffer = { angles_dis(rd_), angles_dis(rd_), angles_dis(rd_) };
			//buffer = { 60, 0, 0 };
			main_scene_.setRCODaphniaAngles(frame, object, buffer);
			objdirections[frame][object] = main_scene_.setRCODaphniaDirection(frame, object);
		}
	}

	//calc 2D params from 3D
	std::cout << "Calculating 2D coords" << std::endl;
	std::vector<std::vector<std::array<double, 3>>> objpoints{ num_frames };
	for (int frame{}; frame < num_frames; ++frame) {
		num_objects = main_scene_.getRCOObjectsNum(frame);
		objpoints[frame] = std::vector<std::array<double, 3>>{ num_objects };
		for (int object = {}; object < num_objects; ++object) {
			objpoints[frame][object] =
				main_scene_.getRCODaphniaCoords(frame, object);
		}
	}

	std::vector<std::vector<std::array<double, 2>>> imgpoints;
	std::vector<std::vector<std::array<double, 2>>> imgdirections{ num_frames };
	predictPoints(imgpoints, objpoints,
		main_scene_.getIntrinsicCameraMatrix(), rmat, tvec, svec);
	predictDirections(imgdirections, objdirections, imgpoints, objpoints,
		main_scene_.getIntrinsicCameraMatrix(), rmat, tvec, svec);

	//saving objoints and imgpoints to json
	saveGenCOJSON(gen_json_dir, objpoints, objdirections, imgpoints, imgdirections);

	//texture settings
	std::string edge_texture_path = data_path_ + src_dir_ + edges_dir_ + std::to_string(index);
	main_scene_.setAquariumEdgeTextureFilename("right", edge_texture_path + right_edge_name_ + image_ending_);
	main_scene_.setAquariumEdgeTextureFilename("left", edge_texture_path + left_edge_name_ + image_ending_);
	main_scene_.setAquariumEdgeTextureFilename("upper", edge_texture_path + upper_edge_name_ + image_ending_);
	main_scene_.setAquariumEdgeTextureFilename("lower", edge_texture_path + lower_edge_name_ + image_ending_);
	main_scene_.setAquariumEdgeTextureFilename("bottom", edge_texture_path + bottom_edge_name_ + image_ending_);

	std::string glut_texture_directory =
		data_path_ + src_dir_ + daphnia_texture_dir_ + daphnia_glut_dir_;
	unsigned int num_glut_textures = std::distance(
		std::filesystem::directory_iterator(glut_texture_directory),
		std::filesystem::directory_iterator{});
	if (num_glut_textures < num_objects_range[0]) {
		for (int i{ (int)num_glut_textures }; i < num_objects_range[0]; ++i)
			makeGLUTDaphniaTexture(i);
		num_glut_textures = num_objects_range[0];
	}

	std::cout << "Daphnia textures brightness leveling off" << std::endl;
	cv::Mat src_image = cv::imread(image_filename, cv::IMREAD_GRAYSCALE);
	std::uniform_int_distribution<> texture_index_dis(0, num_glut_textures - 1);
	unsigned int size{ 64 };
	cv::Mat sup_src_image = add_cv::supplementImage(src_image, size);
	for (int frame{}; frame < objpoints.size(); ++frame) {
		for (int object{}; object < objpoints[frame].size(); ++object) {
			int file_index = texture_index_dis(rd_);
			processDaphniaTexture(size, glut_texture_directory + std::to_string(file_index) + image_ending_,
				sup_src_image, imgpoints[frame][object],
				gen_texture_dir + std::to_string(frame) + "." + std::to_string(object) + image_ending_);
			main_scene_.setRCODaphniaTextureFilename(frame, object,
				gen_texture_dir + std::to_string(frame) + "." + std::to_string(object) + image_ending_);
		}
	}

	//glut rendering
	std::cout << "GLUT frames rendering" << std::endl;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(
		main_scene_.getRenderImageSize().width,
		main_scene_.getRenderImageSize().height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Point array");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	curr_this_ = this;
	glutDisplayFunc(Generator::displayTexturedRandomClip);
	glutReshapeFunc(Generator::reshape);
	main_scene_.resetFrameCount();
	main_scene_.initGLUT();
	glutMainLoop();

	//merge glut scene with source image
	std::cout << "OpenCV background merging" << std::endl;
	cv::Mat mask_source = cv::imread(gen_glut_dir + "0" + image_ending_, cv::IMREAD_GRAYSCALE);
	cv::Mat mask;
	cv::threshold(mask_source, mask, 254, 255, cv::THRESH_BINARY);

	std::vector<cv::Mat> merged_frames;
	std::string merged_filename;
	for (int frame = {}; frame < num_frames; ++frame) {
		merged_filename = gen_merged_dir + std::to_string(frame) + image_ending_;
		//merged_frames.push_back(
		//	add_cv::mergeTexturedImageWithSource(mask, src_image,
		//		gen_glut_dir + std::to_string(frame) + image_ending_));
		add_cv::mergeTexturedImageWithSource(mask, src_image,
			gen_glut_dir + std::to_string(frame) + image_ending_, merged_filename);
	}

	std::cout << "Successful end of program!" << std::endl;
}

////////// genTexturedSequentClip //////////
void Generator::genTexturedSequentClip(
	const std::string& config_filename, std::string path) {
	//read params from config json
	size_t index;
	double fps;
	size_t num_frames;
	std::array<double, 2> num_objects_range;
	std::array<double, 2> size_objects_range;
	readConfigSCOJSON(index, fps, num_frames, num_objects_range, size_objects_range, config_filename);

	//read info from main json and set params
	std::string image_filename = readInputImage(index);
	std::array<double, 9> rmat = readCameraRMat(index);
	std::array<double, 3> tvec = readCameraTVec(index);
	std::array<double, 3> svec = readCameraSVec(index);
	main_scene_.setCameraRMat(rmat);
	main_scene_.setCameraTVec(tvec);
	main_scene_.setCameraSVec(svec);

	std::cout << "Rotation matrix:\n" << rmat << std::endl;
	std::cout << "Translation vector:\t" << tvec << std::endl;
	std::cout << "Shift vector:\t\t" << svec << std::endl;

	//set directories for generation
	if (path.empty()) {
		path = data_path_;
	}
	else if (path.compare(path.size() - 1, 1, "/")) {
		std::cout << "Warning: path ends not with \"/\" and will be supplemented" << std::endl;
		path += "/";
	}
	std::string gen_glut_dir = path + SCO_generation_main_dir_ + generation_frames_dir_ + frames_glut_dir_;
	std::string gen_merged_dir = path + SCO_generation_main_dir_ + generation_frames_dir_ + frames_merged_dir_;
	std::string gen_mask_dir = path + SCO_generation_main_dir_ + generation_frames_dir_ + frames_mask_dir_;
	std::string gen_texture_dir = path + SCO_generation_main_dir_ + generation_textures_dir_;
	std::string gen_json_dir = path + SCO_generation_main_dir_ + generation_json_dir_;
	std::string gen_video_dir = path + SCO_generation_main_dir_ + generation_video_dir_;
	main_scene_.setGenFramesPath(gen_glut_dir);
	main_scene_.setGenMasksPath(gen_mask_dir);
	makeGenFileTree(data_path_, SCO_generation_main_dir_,
		generation_frames_dir_ + frames_glut_dir_,
		generation_frames_dir_ + frames_merged_dir_,
		generation_frames_dir_ + frames_mask_dir_,
		gen_texture_dir, gen_video_dir, generation_json_dir_);

	//construct 3D params - coords, directions
	std::array<double, 3> aq_size = main_scene_.getAquariumSize();

	size_t num_objects;
	std::uniform_int_distribution<>	num_obj;
	num_objects = (num_objects_range[0] == num_objects_range[1])
		? num_objects_range[0]
		: std::uniform_int_distribution<>(num_objects_range[0], num_objects_range[1])(rd_);

	double object_size{};
	std::normal_distribution<> size_dis;
	if ((size_objects_range[0] - size_objects_range[1]) < std::numeric_limits<double>::epsilon())
		object_size = size_objects_range[0];
	else
		size_dis = std::normal_distribution<>(
			(size_objects_range[0] + size_objects_range[1]) * 0.5,
			(size_objects_range[1] - size_objects_range[0]) / 6);

	std::uniform_real_distribution<> x_dis;
	std::uniform_real_distribution<> y_dis;
	std::uniform_real_distribution<> z_dis;
	std::uniform_real_distribution<> angles_dis(0., 180.);

	std::array<double, 3> buffer;
	main_scene_.setSCOFrames(num_frames);
	main_scene_.setSCOObjects(num_objects);
	std::cout << "\nGenerating 3D params" << std::endl;

	//frame 0: initial params
	double curr_scale{}, curr_dl{};
	std::vector<std::vector<std::array<double, 3>>> objdirections{ num_frames };
	objdirections[0] = std::vector<std::array<double, 3>>{ num_objects };
	for (int object = {}; object < num_objects; ++object) {
		//gen size scale
		if ((size_objects_range[0] - size_objects_range[1]) < std::numeric_limits<double>::epsilon())
			curr_scale = object_size;
		else
			curr_scale = normDistGenInRange(size_dis, size_objects_range[0], size_objects_range[1]);
		main_scene_.setSCODaphniaScale(object, curr_scale);
		curr_dl = 0.5 * main_scene_.getSCODaphniaLength(object);

		//set distributions' range by length of object
		x_dis = std::uniform_real_distribution<>(-0.5 * aq_size[0] + curr_dl, 0.5 * aq_size[0] - curr_dl);
		y_dis = std::uniform_real_distribution<>(-0.5 * aq_size[1] + curr_dl, 0.5 * aq_size[1] - curr_dl);
		z_dis = std::uniform_real_distribution<>(-aq_size[2] + curr_dl, 0 - curr_dl);

		//gen coords - x, y, z
		buffer = { x_dis(rd_), y_dis(rd_), z_dis(rd_) };
		main_scene_.setSCODaphniaCoords(0, object, buffer);

		//gen angles - apha, beta, gamma
		buffer = { angles_dis(rd_), angles_dis(rd_), angles_dis(rd_) };
		main_scene_.setSCODaphniaAngles(0, object, buffer);
		objdirections[0][object] = main_scene_.setSCODaphniaDirection(0, object);
	}

	//other frames
	angles_dis = std::uniform_real_distribution<> (0., 10.);
	double min_shift{ 0. }, max_shift{ 0.2 };
	double shift{};
	std::normal_distribution<> shift_dis(
		(min_shift + max_shift) * 0.5, (max_shift - min_shift) / 6);
	for (int frame = { 1 }; frame < num_frames; ++frame) {
		objdirections[frame] = std::vector<std::array<double, 3>>{ num_objects };
		for (int object = {}; object < num_objects; ++object) {
			//gen direction deviation
			buffer = { angles_dis(rd_), angles_dis(rd_), angles_dis(rd_) };
			main_scene_.addSCONextDaphniaAngles(frame, object, buffer);
			objdirections[frame][object] = main_scene_.setSCODaphniaDirection(frame, object);

			//gen shift
			shift = shift_dis(rd_);
			main_scene_.applySCODaphniaShift(frame, object, shift);
		}
	}

	//calc 2D params from 3D
	std::cout << "Calculating 2D coords" << std::endl;
	std::vector<std::vector<std::array<double, 3>>> objpoints{ num_frames };
	for (int frame{}; frame < num_frames; ++frame) {
		objpoints[frame] = std::vector<std::array<double, 3>>{ num_objects };
		for (int object = {}; object < num_objects; ++object) {
			objpoints[frame][object] =
				main_scene_.getSCODaphniaCoords(frame, object);
		}
	}

	std::vector<std::vector<std::array<double, 2>>> imgpoints;
	std::vector<std::vector<std::array<double, 2>>> imgdirections{ num_frames };
	predictPoints(imgpoints, objpoints,
		main_scene_.getIntrinsicCameraMatrix(), rmat, tvec, svec);
	predictDirections(imgdirections, objdirections, imgpoints, objpoints,
		main_scene_.getIntrinsicCameraMatrix(), rmat, tvec, svec);

	//saving objoints and imgpoints to json
	saveGenCOJSON(gen_json_dir, objpoints, objdirections, imgpoints, imgdirections);

	//texture settings
	std::string edge_texture_path = data_path_ + src_dir_ + edges_dir_ + std::to_string(index);
	main_scene_.setAquariumEdgeTextureFilename("right", edge_texture_path + right_edge_name_ + image_ending_);
	main_scene_.setAquariumEdgeTextureFilename("left", edge_texture_path + left_edge_name_ + image_ending_);
	main_scene_.setAquariumEdgeTextureFilename("upper", edge_texture_path + upper_edge_name_ + image_ending_);
	main_scene_.setAquariumEdgeTextureFilename("lower", edge_texture_path + lower_edge_name_ + image_ending_);
	main_scene_.setAquariumEdgeTextureFilename("bottom", edge_texture_path + bottom_edge_name_ + image_ending_);

	std::string glut_texture_directory =
		data_path_ + src_dir_ + daphnia_texture_dir_ + daphnia_glut_dir_;
	unsigned int num_glut_textures = std::distance(
		std::filesystem::directory_iterator(glut_texture_directory),
		std::filesystem::directory_iterator{});
	if (num_glut_textures < num_objects_range[0]) {
		for (int i{ (int)num_glut_textures }; i < num_objects_range[0]; ++i)
			makeGLUTDaphniaTexture(i);
		num_glut_textures = num_objects_range[0];
	}

	std::cout << "Daphnia textures brightness leveling off" << std::endl;
	cv::Mat src_image = cv::imread(image_filename, cv::IMREAD_GRAYSCALE);
	std::uniform_int_distribution<> texture_index_dis(0, num_glut_textures - 1);
	unsigned int size{ 64 };
	cv::Mat sup_src_image = add_cv::supplementImage(src_image, size);

	//gen texture number and set for each object on all frames
	int file_index{};
	std::string texture_source_filename{};
	for (int object{}; object < num_objects; ++object) {
		file_index = texture_index_dis(rd_);
		texture_source_filename = glut_texture_directory + std::to_string(file_index) + image_ending_;
		main_scene_.setSCODaphniaTextureSourceFilename(object, texture_source_filename);
	}

	//process texture for each object
	for (int frame{}; frame < num_frames; ++frame) {
		for (int object{}; object < num_objects; ++object) {
			texture_source_filename = main_scene_.getSCODaphniaTextureSourceFilename(frame, object);
			processDaphniaTexture(size, texture_source_filename, sup_src_image, imgpoints[frame][object],
				gen_texture_dir + std::to_string(frame) + "." + std::to_string(object) + image_ending_);
			main_scene_.setSCODaphniaTextureFilename(frame, object,
				gen_texture_dir + std::to_string(frame) + "." + std::to_string(object) + image_ending_);
		}
	}

	//glut rendering
	std::cout << "GLUT frames rendering" << std::endl;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(
		main_scene_.getRenderImageSize().width,
		main_scene_.getRenderImageSize().height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Point array");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	curr_this_ = this;
	glutDisplayFunc(Generator::displayTexturedSequentClip);
	glutReshapeFunc(Generator::reshape);
	main_scene_.resetFrameCount();
	main_scene_.initGLUT();
	glutMainLoop();

	if (!glutGet(GLUT_INIT_STATE))
		glutInit(&argc_, argv_);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(
		main_scene_.getRenderImageSize().width,
		main_scene_.getRenderImageSize().height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Point array");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	curr_this_ = this;
	glutDisplayFunc(Generator::displayMaskSequentClip);
	glutReshapeFunc(Generator::reshape);
	main_scene_.resetFrameCount();
	main_scene_.initGLUT(0, 0, 0);
	glutMainLoop();

	//merge glut scene with source image
	std::cout << "OpenCV background merging" << std::endl;
	cv::Mat mask_source = cv::imread(gen_glut_dir + "0" + image_ending_, cv::IMREAD_GRAYSCALE);
	cv::Mat mask;
	cv::threshold(mask_source, mask, 254, 255, cv::THRESH_BINARY);

	std::vector<cv::Mat> merged_frames;
	std::vector<cv::Mat> masked_frames;
	std::string merged_filename;
	std::string masked_filename;
	for (int frame = {}; frame < num_frames; ++frame) {
		merged_filename = gen_merged_dir + std::to_string(frame) + image_ending_;
		merged_frames.push_back(
			add_cv::mergeTexturedImageWithSource(mask, src_image,
				gen_glut_dir + std::to_string(frame) + image_ending_));

		masked_filename = gen_mask_dir + std::to_string(frame) + image_ending_;
		masked_frames.push_back(cv::imread(masked_filename, cv::IMREAD_GRAYSCALE));
		add_cv::smoothObjectBorders(merged_frames[frame], masked_frames[frame], merged_filename);
	}

	//make video from frames
	std::cout << "Video making" << std::endl;
	auto video = cv::VideoWriter(gen_video_dir + "frames.mp4",
		cv::VideoWriter::fourcc('a', 'v', 'c', '1'), fps, merged_frames[0].size());
	for (const auto& frame : merged_frames)
		video.write(frame);
	video.release();

	video = cv::VideoWriter(gen_video_dir + "masks.mp4",
		cv::VideoWriter::fourcc('a', 'v', 'c', '1'), fps, masked_frames[0].size());
	for (const auto& frame : masked_frames)
		video.write(frame);
	video.release();

	std::cout << "Successful end of program!" << std::endl;

}

////////// cvtMatToVector //////////
std::vector<double> Generator::cvtMatToVector(const cv::Mat& mat) {
	std::vector<double> array;
	if (mat.isContinuous()) {
		array.assign((double*)mat.data, (double*)mat.data + mat.total() * mat.channels());
	}
	else {
		for (int i = 0; i < mat.rows; ++i) {
			array.insert(array.end(), mat.ptr<double>(i), mat.ptr<double>(i) + mat.cols * mat.channels());
		}
	}
	return array;
}

////////// displayPointGrid //////////
void Generator::displayPointGrid() {
	curr_this_->main_scene_.displayPointGrid();
}

////////// displayUntexturedRandomClip //////////
void Generator::displayUntexturedRandomClip() {
	curr_this_->main_scene_.displayUntexturedRandomClip();
}

////////// displayTexturedRandomClip //////////
void Generator::displayTexturedRandomClip() {
	curr_this_->main_scene_.displayTexturedRandomClip();
}

////////// displayMaskRandomClip //////////
void Generator::displayMaskRandomClip() {
	curr_this_->main_scene_.displayMaskRandomClip();
}

////////// displayTexturedSequentClip //////////
void Generator::displayTexturedSequentClip() {
	curr_this_->main_scene_.displayTexturedSequentClip();
}

////////// displayMaskSequentClip //////////
void Generator::displayMaskSequentClip() {
	curr_this_->main_scene_.displayMaskSequentClip();
}

////////// reshape //////////
void Generator::reshape(int width, int height) {
	curr_this_->main_scene_.reshape(width, height);
}

////////// controlKey //////////
void Generator::controlKey(unsigned char key, int x, int y) {
	curr_this_->main_scene_.controlKey(key, x, y);
}

////////// controlSpec //////////
void Generator::controlSpec(int key, int x, int y) {
	curr_this_->main_scene_.controlSpec(key, x, y);
}

////////// normDistGenInRange //////////
double Generator::normDistGenInRange(std::normal_distribution<> norm,
	const double& min, const double& max) {
	double sample;
	do {
		sample = norm(rd_);
	} while (sample < min || sample > max);
	return sample;
}
