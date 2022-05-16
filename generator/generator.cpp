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

////////// saveGenRCOJSON //////////
void Generator::saveGenRCOJSON(const std::string& path,
	const std::vector<std::vector<std::array<double, 3>>>& objpoints,
	const std::vector<std::vector<std::array<double, 2>>>& imgpoints) {
	
	gen_RCO_json_["3D_points"] = json::array();
	gen_RCO_json_["2D_points"] = json::array();

	for (int frame{}; frame < objpoints.size(); ++frame) {
		gen_RCO_json_["3D_points"].push_back(json::array());
		for (int object{}; object < objpoints[frame].size(); ++object) {
			gen_RCO_json_["3D_points"][frame].push_back(objpoints[frame][object]);
			gen_RCO_json_["2D_points"][frame].push_back(imgpoints[frame][object]);
		}
	}

	std::ofstream file(path + generation_json_name_);
	file << gen_RCO_json_.dump(4) << std::endl;
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

////////// readCameraSVec //////////
std::array<double, 3> Generator::readCameraSVec(size_t index) {
	return main_json_["camera"][index]["svec"].get<std::array<double, 3>>();
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
	writeCameraRMat(rmat, index);
	writeCameraTVec(tvec, index);
	writeCameraSVec({0., 0., 0.}, index);
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

////////// predictPoints //////////
void Generator::predictPoints(std::vector<std::vector<std::array<double, 2>>>& imgpoints,
	const std::vector<std::vector<std::array<double, 3>>>& objpoints,
	const std::array<double, 9>& cmat,
	const std::array<double, 9>& rmat,
	const std::array<double, 3>& tvec) {

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

////////// showPointGrid //////////
void Generator::showPointGrid(size_t index, const cv::Size& quantity,
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
	cv::Mat grid_glut = cv::imread(grid_glut_filename_, cv::IMREAD_GRAYSCALE);
	cv::Mat src = cv::imread(image_filename, cv::IMREAD_GRAYSCALE);

	cv::Mat grid_merged_1c;
	cv::bitwise_and(src, grid_glut, grid_merged_1c, grid_glut);

	cv::Mat grid_merged_3c;
	cv::merge(std::array<cv::Mat, 3>
		{ grid_merged_1c, grid_merged_1c, grid_merged_1c }, grid_merged_3c);

	for (auto& point : imggridpoints)
		add_cv::cross(grid_merged_3c, point, { 2, 2 }, { 0, 0, 255 });
	cv::imwrite(grid_merged_filename_, grid_merged_3c);

	if (save) {
		writeCameraSVec(shift, index);
		saveMainJSON();
	}

	std::cout << "Successful end of program!" << std::endl;
}

////////// genRandomClip //////////
void Generator::genRandomClip(size_t index, size_t num_frames, size_t num_objects,
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
		path = generation_path_;
	}
	else if (path.compare(path.size() - 1, 1, "/")) {
		std::cout << "Warning: path ends not with \"/\" and will be supplemented" << std::endl;
		path += "/";
	}
	std::string gen_glut_dir = path + RCO_generation_main_dir_ + generation_frames_dir_ + frames_glut_dir_;
	std::string gen_merged_dir = path + RCO_generation_main_dir_ + generation_frames_dir_ + frames_merged_dir_;
	std::string gen_json_dir = path + RCO_generation_main_dir_ + generation_json_dir_;
	main_scene_.setGenFramesPath(gen_glut_dir);
	makeGenFileTree(generation_path_, RCO_generation_main_dir_,
		generation_frames_dir_ + frames_glut_dir_,
		generation_frames_dir_ + frames_merged_dir_, generation_json_dir_);

	//construct 3D params - coords, angles
	std::array<double, 3> aq_size = main_scene_.getAquariumSize();

	std::uniform_real_distribution<> x_dis(-0.5 * aq_size[0], 0.5 * aq_size[0]);
	std::uniform_real_distribution<> y_dis(-0.5 * aq_size[1], 0.5 * aq_size[1]);
	std::uniform_real_distribution<> z_dis(-aq_size[2], 0);
	std::uniform_real_distribution<> angles_dis(0., 180.);

	std::array<double, 3> buffer;
	main_scene_.setRCOFrames(num_frames);
	std::cout << "\nGenerating 3D params" << std::endl;
	for (int frame = {}; frame < num_frames; ++frame) {
		main_scene_.setRCOObjects(frame, num_objects);
		for (int object = {}; object < num_objects; ++object) {

			//gen coords - x, y, z
			buffer = { x_dis(rd_), y_dis(rd_), z_dis(rd_) };
			main_scene_.setRCODaphniaCoords(frame, object, buffer);

			//gen angles - apha, beta, gamma
			buffer = { angles_dis(rd_), angles_dis(rd_), angles_dis(rd_) };
			main_scene_.setRCODaphniaAngles(frame, object, buffer);
		}
	}

	//calc 2D coords from 3D
	std::cout << "Calculating 2D coords" << std::endl;
	std::vector<std::vector<std::array<double, 3>>> objpoints{num_frames};
	for (int frame{}; frame < num_frames; ++frame) {
		objpoints[frame] = std::vector<std::array<double, 3>>{num_objects};
		for (int object = {}; object < num_objects; ++object) {
			objpoints[frame][object] =
				main_scene_.getRCODaphnia(frame, object).getCoords();
		}
	}

	std::vector<std::vector<std::array<double, 2>>> imgpoints;
	predictPoints(imgpoints, objpoints,
		main_scene_.getIntrinsicCameraMatrix(), rmat, tvec);

	//saving objoints and imgpoints to json
	saveGenRCOJSON(gen_json_dir, objpoints, imgpoints);

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
	glutDisplayFunc(Generator::displayRandomClip);
	glutReshapeFunc(Generator::reshape);
	main_scene_.resetFrameCount();
	main_scene_.initGLUT();
	glutMainLoop();

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

////////// displayRandomClip //////////
void Generator::displayRandomClip() {
	curr_this_->main_scene_.displayRandomClip();
}

////////// reshape //////////
void Generator::reshape(int width, int height) {
	curr_this_->main_scene_.reshape(width, height);
}
