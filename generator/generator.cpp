#include "generator.h"

namespace cv {

void cross(cv::InputOutputArray& img, const cv::Point point,
	const cv::Size& size, const cv::Scalar& color) {
	cv::line(img,
		cv::Point2d{ 1.0 * point.x - size.width, 1.0 * point.y },
		cv::Point2d{ 1.0 * point.x + size.width, 1.0 * point.y }, color);
	cv::line(img,
		cv::Point2d{ 1.0 * point.x, 1.0 * point.y - size.height },
		cv::Point2d{ 1.0 * point.x, 1.0 * point.y + size.height }, color);
}

}

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

////////// loadMainJSON //////////
void Generator::loadMainJSON() {
	std::ifstream file(getMainJSONFilename());
	file >> main_json_;
}

////////// saveMainJSON //////////
void Generator::saveMainJSON() {
	std::ofstream file(getMainJSONFilename());
	file << main_json_.dump(4) << std::endl;
}

////////// readInputImgpoints //////////
std::vector<cv::Point2d> Generator::readInputImgpoints(size_t index) {
	json x_coords = main_json_["input"][index]["x_coords"];
	json y_coords = main_json_["input"][index]["y_coords"];

	std::vector<cv::Point2d> imgpoints;
	for (int j{}; j < x_coords.size(); ++j)
		imgpoints.push_back(cv::Point2d{ (double)x_coords[j], (double)y_coords[j] });

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

////////// writeRotationMatrix //////////
void Generator::writeRotationMatrix(const cv::Mat& rmat, size_t i) {
	main_json_["camera"][i]["rmat"] = cvtMatToVector(rmat);
}

////////// writeTranslationVector //////////
void Generator::writeTranslationVector(const cv::Mat& rvec, size_t i) {
	main_json_["camera"][i]["tvec"] = cvtMatToVector(rvec);
}

////////// writeShiftVector //////////
void Generator::writeShiftVector(std::array<double, 3> vec, size_t i) {
	main_json_["camera"][i]["shift"] = vec;
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
	std::vector<cv::Point2d> imgpoints = readInputImgpoints(index);

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
	writeRotationMatrix(rmat, index);
	writeTranslationVector(tvec, index);
	writeShiftVector({0., 0., 0.}, index);
	saveMainJSON();
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

////////// showPointGrid //////////
void Generator::showPointGrid(size_t index, const cv::Size& quantity){
	//read info from json
	std::string image_filename = readInputImage(index);
	std::array<double, 9> rmat = readCameraRMat(index);
	std::array<double, 3> tvec = readCameraTVec(index);
	main_scene_.setCameraRMat(rmat);
	main_scene_.setCameraTVec(tvec);

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
				(-0.5 * aquarium_height + j * height_step), 0 });
		}
	}
	main_scene_.setObjGridPoints(objgridpoints);
	main_scene_.setGridFilename(grid_glut_filename_);

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
	cv::Mat grid_glut = cv::imread(grid_glut_filename_, cv::IMREAD_GRAYSCALE);
	cv::Mat src = cv::imread(image_filename, cv::IMREAD_GRAYSCALE);

	cv::Mat grid_merged_1c;
	cv::bitwise_and(src, grid_glut, grid_merged_1c, grid_glut);

	cv::Mat grid_merged_3c;
	cv::merge(std::array<cv::Mat, 3>
		{ grid_merged_1c, grid_merged_1c, grid_merged_1c }, grid_merged_3c);

	for (auto& point : imggridpoints)
		cv::cross(grid_merged_3c, point, { 2, 2 }, { 0, 0, 255 });
	cv::imwrite(grid_merged_filename, grid_merged_3c);
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

////////// reshape //////////
void Generator::reshape(int width, int height) {
	curr_this_->main_scene_.reshape(width, height);
}
