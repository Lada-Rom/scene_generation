#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "utils.h"

namespace add_cv {

void cross(cv::InputOutputArray& img, const cv::Point& point,
	const cv::Size& size, const cv::Scalar& color) {
	cv::line(img,
		cv::Point2d{ 1.0 * point.x - size.width, 1.0 * point.y },
		cv::Point2d{ 1.0 * point.x + size.width, 1.0 * point.y }, color);
	cv::line(img,
		cv::Point2d{ 1.0 * point.x, 1.0 * point.y - size.height },
		cv::Point2d{ 1.0 * point.x, 1.0 * point.y + size.height }, color);
}

void cross(cv::InputOutputArray& img, const std::array<double, 2>& point,
	const cv::Size& size, const cv::Scalar& color) {
	cv::line(img,
		cv::Point2d{ 1.0 * point[0] - size.width, 1.0 * point[1] },
		cv::Point2d{ 1.0 * point[0] + size.width, 1.0 * point[1] }, color);
	cv::line(img,
		cv::Point2d{ 1.0 * point[0], 1.0 * point[1] - size.height },
		cv::Point2d{ 1.0 * point[0], 1.0 * point[1] + size.height }, color);
}

////////// mergeUntexturedImageAndPoints //////////
void mergeUntexturedImageAndPoints(const std::string& src_filename,
	const std::vector<cv::Point2d>& imgpoints,
	const std::string& glut_filename, const std::string& dst_filename) {

	cv::Mat grid_glut = cv::imread(glut_filename, cv::IMREAD_GRAYSCALE);
	cv::Mat src = cv::imread(src_filename, cv::IMREAD_GRAYSCALE);

	cv::Mat grid_merged_1c;
	cv::bitwise_and(src, grid_glut, grid_merged_1c, grid_glut);

	cv::Mat grid_merged_3c;
	cv::merge(std::array<cv::Mat, 3>
	{ grid_merged_1c, grid_merged_1c, grid_merged_1c }, grid_merged_3c);

	for (auto& point : imgpoints)
		add_cv::cross(grid_merged_3c, point, { 2, 2 }, { 0, 0, 255 });
	cv::imwrite(dst_filename, grid_merged_3c);
}

////////// mergeUntexturedImageAndPoints //////////
void mergeUntexturedImageAndPoints(const std::string& src_filename,
	const std::vector<std::array<double, 2>>& imgpoints,
	const std::string& glut_filename, const std::string& dst_filename) {

	cv::Mat grid_glut = cv::imread(glut_filename, cv::IMREAD_GRAYSCALE);
	cv::Mat src = cv::imread(src_filename, cv::IMREAD_GRAYSCALE);

	cv::Mat grid_merged_1c;
	cv::bitwise_and(src, grid_glut, grid_merged_1c, grid_glut);

	cv::Mat grid_merged_3c;
	cv::merge(std::array<cv::Mat, 3>
	{ grid_merged_1c, grid_merged_1c, grid_merged_1c }, grid_merged_3c);

	for (auto& point : imgpoints)
		add_cv::cross(grid_merged_3c, point, { 2, 2 }, { 0, 0, 255 });
	cv::imwrite(dst_filename, grid_merged_3c);
}

////////// mergeTexturedImageWithSource //////////
void mergeTexturedImageWithSource(const cv::Mat& mask, const cv::Mat& src_image,
	const std::string& glut_filename, const std::string& dst_filename) {
	cv::Mat glut_image = cv::imread(glut_filename, cv::IMREAD_GRAYSCALE);

	cv::bitwise_and(src_image, glut_image, glut_image, mask);
	cv::imwrite(dst_filename, glut_image);
}

////////// mergeTexturedImageWithSource //////////
cv::Mat mergeTexturedImageWithSource(const cv::Mat& mask, const cv::Mat& src_image,
	const std::string& glut_filename) {
	cv::Mat glut_image = cv::imread(glut_filename, cv::IMREAD_GRAYSCALE);

	cv::bitwise_and(src_image, glut_image, glut_image, mask);
	return glut_image;
}

////////// mergeTexturedImageWithSource //////////
void mergeTexturedImageWithSource(
	const std::vector<std::array<double, 2>>& imgpoints,
	const cv::Mat& mask, const cv::Mat& src_image,
	const std::string& glut_filename, const std::string& dst_filename) {
	cv::Mat glut_image = cv::imread(glut_filename, cv::IMREAD_GRAYSCALE);

	cv::bitwise_and(src_image, glut_image, glut_image, mask);
	cv::Mat merged_3c;
	cv::merge(std::array<cv::Mat, 3>{glut_image, glut_image, glut_image}, merged_3c);
	for (auto& point : imgpoints)
		add_cv::cross(merged_3c, point, { 2, 2 }, { 0, 0, 255 });
	cv::imwrite(dst_filename, glut_image);
}

////////// supplementImage //////////
cv::Mat supplementImage(const cv::Mat& src, unsigned int size) {
	cv::Mat dst = cv::Mat(src.rows + size, src.cols + size, src.type());
	cv::Rect upper_roi(0, 0, src.cols, 0.5 * size);
	cv::Rect lower_roi(0, src.rows - 0.5 * size, src.cols, 0.5 * size);
	cv::Rect right_roi(src.cols - 0.5 * size, 0, 0.5 * size, src.rows);
	cv::Rect left_roi(0, 0, 0.5 * size, src.rows);
	cv::Rect upper_left(0, 0, 0.5 * size, 0.5 * size);
	cv::Rect upper_right(src.cols - 0.5 * size, 0, 0.5 * size, 0.5 * size);
	cv::Rect lower_right(src.cols - 0.5 * size, src.rows - 0.5 * size, 0.5 * size, 0.5 * size);
	cv::Rect lower_left(0, src.rows - 0.5 * size, 0.5 * size, 0.5 * size);

	src.copyTo(dst({ (int)(0.5 * size), (int)(0.5 * size + src.rows) },
		{ (int)(0.5 * size), (int)(0.5 * size + src.cols) }));

	cv::Mat temp = src(upper_roi).clone();
	cv::flip(temp, temp, 0);
	temp.copyTo(dst({ 0, (int)(0.5 * size) },
		{ (int)(0.5 * size), (int)(src.cols + 0.5 * size) }));

	temp = src(lower_roi).clone();
	cv::flip(temp, temp, 0);
	temp.copyTo(dst({ (int)(0.5 * size + src.rows), (int)(src.rows + size) },
		{ (int)(0.5 * size), (int)(src.cols + 0.5 * size) }));

	temp = src(right_roi).clone();
	cv::flip(temp, temp, 1);
	temp.copyTo(dst({ (int)(0.5 * size), (int)(src.rows + 0.5 * size) },
		{ (int)(0.5 * size + src.cols), (int)(size + src.cols) }));

	temp = src(left_roi).clone();
	cv::flip(temp, temp, 1);
	temp.copyTo(dst({ (int)(0.5 * size), (int)(src.rows + 0.5 * size) },
		{ 0, (int)(0.5 * size) }));

	temp = src(upper_left).clone();
	cv::flip(temp, temp, -1);
	temp.copyTo(dst({ 0, (int)(0.5 * size) }, { 0, (int)(0.5 * size) }));

	temp = src(upper_right).clone();
	cv::flip(temp, temp, -1);
	temp.copyTo(dst({ 0, (int)(0.5 * size) },
		{ (int)(0.5 * size + src.cols), (int)(size + src.cols) }));

	temp = src(lower_right).clone();
	cv::flip(temp, temp, -1);
	temp.copyTo(dst({ (int)(0.5 * size + src.rows), (int)(size + src.rows) },
		{ (int)(0.5 * size + src.cols), (int)(size + src.cols) }));

	temp = src(lower_left).clone();
	cv::flip(temp, temp, -1);
	temp.copyTo(dst({ (int)(0.5 * size + src.rows), (int)(size + src.rows) },
		{ 0, (int)(0.5 * size) }));

	return dst;
}

////////// textureDaphnia //////////
void textureDaphnia(cv::Mat& img, cv::Mat& obj_texture,
	const std::array<double, 2>& center, const std::array<double, 2>& direction) {
	//roi and mean roi
	cv::Mat res_obj_texture = cv::Mat::zeros(20, 20, obj_texture.type());
	cv::resize(obj_texture, res_obj_texture, res_obj_texture.size(), 0, 0, cv::INTER_LINEAR);
	cv::Rect rect_roi(
		cv::Point2d{
			center[0] - 0.5 * res_obj_texture.cols,
			center[1] - 0.5 * res_obj_texture.rows },
		res_obj_texture.size());
	cv::Mat img_roi = img(rect_roi);
	cv::Mat mean_roi = cv::Mat::zeros(img_roi.size(), img_roi.type());
	mean_roi += cv::mean(img_roi).val[0];

	//rotate by direction
	//std::array<double, 2> normalized_direction = {
	//	direction[0] / std::sqrt(direction[0] * direction[0] + direction[1] * direction[1]),
	//	direction[1] / std::sqrt(direction[0] * direction[0] + direction[1] * direction[1])
	//};
	//double radian_angle = std::acos(normalized_direction[0] * 1. + normalized_direction[1] * 0.);
	//double degree_angle = 180. * radian_angle / std::acos(-1.);
	//cv::RotatedRect          dst = cv::RotatedRect(
	//	cv::Point2f(res_obj_texture.cols - 1, res_obj_texture.rows - 1),
	//	cv::Size2f(res_obj_texture.cols * 2, res_obj_texture.rows * 2), int(degree_angle));
	//cv::Point2f dst_[4];
	//std::vector<cv::Point2f> dst__;
	//dst.points(dst_);
	//for (int i = 0; i < 4; i++) {
	//	dst__.push_back(dst_[i]);
	//}
	//std::vector<cv::Point2f> src_;
	//src_.push_back(cv::Point2f(0, res_obj_texture.rows - 1));
	//src_.push_back(cv::Point2f(0, 0));
	//src_.push_back(cv::Point2f(res_obj_texture.cols - 1, 0));
	//src_.push_back(cv::Point2f(res_obj_texture.cols, res_obj_texture.rows));
	//cv::Mat H = cv::findHomography(src_, dst__);
	//cv::Mat daphnia(cv::Size(res_obj_texture.cols * 2, res_obj_texture.rows * 2), CV_64FC1);
	//cv::warpPerspective(res_obj_texture, res_obj_texture, H,
	//	cv::Size(res_obj_texture.cols * 2, res_obj_texture.rows * 2));

	//level off brightness
	double min_val, max_val;
	cv::minMaxIdx(img, &min_val, &max_val);
	for (int k = 0; k < res_obj_texture.cols; ++k) {
		for (int j = 0; j < res_obj_texture.rows; ++j) {
			if ((double)res_obj_texture.at<uchar>(j, k) < min_val)
				res_obj_texture.at<uchar>(j, k) = min_val;
		}
	}

	cv::Mat textured_roi = mean_roi.clone();
	for (int k = 0; k < res_obj_texture.cols; ++k) {
		for (int j = 0; j < res_obj_texture.rows; ++j) {
			res_obj_texture.at<uchar>(j, k) = cv::saturate_cast<uchar>(
				1. / 255 * (double)res_obj_texture.at<uchar>(j, k) * (
					(double)mean_roi.at<uchar>(j, k) - 0.5 *
					(double)res_obj_texture.at<uchar>(j, k)) +
				1. / 255 * (255 - (double)res_obj_texture.at<uchar>(j, k))
				* (double)img.at<uchar>(
					center[1] - 0.5 * res_obj_texture.rows + j,
					center[0] - 0.5 * res_obj_texture.cols + k));
		}
	}

	for (int k = 0; k < res_obj_texture.cols; ++k) {
		for (int j = 0; j < res_obj_texture.rows; ++j) {
			res_obj_texture.at<uchar>(j, k) = std::min(
					int(img.at<uchar>(
						center[1] - 0.5 * res_obj_texture.rows + j,
						center[0] - 0.5 * res_obj_texture.cols + k)),
					int(res_obj_texture.at<uchar>(j, k)));
		}
	}

	//add to source image
	res_obj_texture.copyTo(img(rect_roi));
}

////////// textureFrameDaphnias //////////
void textureFrameDaphnias(size_t frame_index, cv::Mat& img, const json& gen_json,
	std::random_device& rd, const std::string& texture_path,
	const std::string& dst_filename, const std::string& format) {

	//get info from json
	unsigned int num_objects = gen_json["3D_points"][frame_index].size();
	json frame_3d_json = gen_json["3D_points"][frame_index];
	json frame_2d_json = gen_json["2D_points"][frame_index];

	//get texture
	size_t num_ovoid_textures = std::distance(
		std::filesystem::directory_iterator(texture_path + "ovoid/"),
		std::filesystem::directory_iterator{});
	size_t num_circle_textures = std::distance(
		std::filesystem::directory_iterator(texture_path + "ovoid/"),
		std::filesystem::directory_iterator{});
	std::uniform_int_distribution<> tex_ovoid_number(0, num_ovoid_textures - 2);   //excluding mask
	std::uniform_int_distribution<> tex_circle_number(0, num_circle_textures - 2); //excluding mask
	std::string texture_filename;

	cv::Mat obj_texture;
	std::array<double, 2> center, direction;

	//for each object of frame
	for (int object{}; object < num_objects; ++object) {
		//load texture from directory
		texture_filename = texture_path;
		if (std::abs(frame_3d_json[object]["direction"][0].get<double>()) >= 0.85) {
			texture_filename += "circle/";
			texture_filename += std::to_string(tex_circle_number(rd)) + format;
		}
		else {
			texture_filename += "ovoid/";
			texture_filename += std::to_string(tex_ovoid_number(rd)) + format;
		}
		obj_texture = cv::imread(texture_filename, cv::IMREAD_GRAYSCALE);
	
		//draw objecttexture on frame
		center = frame_2d_json[object]["coords"].get<std::array<double, 2>>();
		direction = frame_2d_json[object]["direction"].get<std::array<double, 2>>();
		add_cv::textureDaphnia(img, obj_texture, center, direction);
	}

	//write image to file
	cv::imwrite(dst_filename, img);
}

} //namespace add_cv


namespace glm {

std::ostream& operator<<(std::ostream& ostrm, const glm::dmat3& rhs) {
	ostrm << std::fixed << std::setprecision(4)
		<< "{\t" << rhs[0][0] << "\t" << rhs[0][1] << "\t" << rhs[0][2] << "\n"
		<< " \t" << rhs[1][0] << "\t" << rhs[1][1] << "\t" << rhs[1][2] << "\n"
		<< " \t" << rhs[2][0] << "\t" << rhs[2][1] << "\t" << rhs[2][2] << " }";
	return ostrm;
}

std::ostream& operator<<(std::ostream& ostrm, const glm::dmat4& rhs) {
	ostrm << std::fixed << std::setprecision(4)
		<< "{\t" << rhs[0][0] << "\t" << rhs[0][1] << "\t" << rhs[0][2] << "\t" << rhs[0][3] << "\n"
		<< " \t" << rhs[1][0] << "\t" << rhs[1][1] << "\t" << rhs[1][2] << "\t" << rhs[1][3] << "\n"
		<< " \t" << rhs[2][0] << "\t" << rhs[2][1] << "\t" << rhs[2][2] << "\t" << rhs[2][3] << "\n"
		<< " \t" << rhs[3][0] << "\t" << rhs[3][1] << "\t" << rhs[3][2] << "\t" << rhs[3][3] << " }";
	return ostrm;
}

std::ostream& operator<<(std::ostream& ostrm, const glm::dvec3& rhs) {
	ostrm << std::fixed << std::setprecision(4)
		<< "{ " << rhs[0] << " " << rhs[1] << " " << rhs[2] << " }";
	return ostrm;
}

} //namespace glm


////////// saveImage //////////
void saveImage(int& width, int& height, std::string& filename) {
	unsigned char* data = (unsigned char*)malloc((int)(width * height * (3)));
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_flip_vertically_on_write(true);
	stbi_write_png(filename.c_str(), width, height, 3, data, 0);
}

////////// loadTexture //////////
void loadTexture(Texture& texture) {
	unsigned char* data = stbi_load(texture.filename_.c_str(),
		&texture.width_, &texture.height_, &texture.comp_, 3);
	if (!data)
		throw std::exception("Unable to read texture data");

	glGenTextures(1, &texture.id_);
	glBindTexture(GL_TEXTURE_2D, texture.id_);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width_, texture.height_,
		GL_RGB, GL_UNSIGNED_BYTE, data);

	free(data);
}

////////// median //////////
uchar median(std::vector<uchar>&vec) {
	size_t n = vec.size() / 2;
	std::nth_element(vec.begin(), vec.begin() + n, vec.end());
	return vec[n];
}

std::ostream& operator<<(std::ostream& ostrm, const std::array<double, 9>& rhs) {
	ostrm << std::fixed << std::setprecision(4)
		<< "{\t" << rhs[0] << "\t" << rhs[1] << "\t" << rhs[2] << "\n"
		<< " \t" << rhs[3] << "\t" << rhs[4] << "\t" << rhs[5] << "\n"
		<< " \t" << rhs[6] << "\t" << rhs[7] << "\t" << rhs[8] << " }";
	return ostrm;
}

std::ostream& operator<<(std::ostream& ostrm, const std::array<double, 3>& rhs) {
	ostrm << std::fixed << std::setprecision(4)
		<< "{ " << rhs[0] << " " << rhs[1] << " " << rhs[2] << " }";
	return ostrm;
}

std::ostream& operator<<(std::ostream& ostrm, const std::vector<cv::Point3d>& rhs) {
	ostrm << std::fixed << std::setprecision(4);
	for (const auto& e : rhs)
		ostrm << "{ " << e.x << " " << e.y << " " << e.z << " },  ";
 	return ostrm;
}

std::ostream& operator<<(std::ostream& ostrm, const std::array<std::array<double, 3>, 8>& rhs) {
	ostrm << std::fixed << std::setprecision(4);
	for (const auto& e : rhs)
		ostrm << "{ " << e[0] << " " << e[1] << " " << e[2] << " },  ";
	return ostrm;
}

////////// makeGenFileTree //////////
void makeGenFileTree(const std::string& path, const std::string& main_dir,
	const std::string& glut_dir, const std::string& merged_dir, const std::string& json_dir) {
	namespace fs = std::filesystem;

	//check if path exists
	fs::file_status s = fs::file_status{};
	if (!fs::exists(path))
		throw std::invalid_argument("Path does not exists");
		
	//check if main_dir exists
	if (fs::exists(path + main_dir))
		fs::remove_all(path + main_dir);

	//making subdirectories
	fs::create_directories(path + main_dir + glut_dir);
	fs::create_directories(path + main_dir + merged_dir);
	fs::create_directories(path + main_dir + json_dir);
}

////////// makeGenFileTree //////////
void makeGenFileTree(const std::string& path, const std::string& main_dir,
	const std::string& glut_dir, const std::string& merged_dir,
	const std::string& mask_dir, const std::string& json_dir) {
	namespace fs = std::filesystem;

	//check if path exists
	fs::file_status s = fs::file_status{};
	if (!fs::exists(path))
		throw std::invalid_argument("Path does not exists");

	//check if main_dir exists
	if (fs::exists(path + main_dir))
		fs::remove_all(path + main_dir);

	//making subdirectories
	fs::create_directories(path + main_dir + glut_dir);
	fs::create_directories(path + main_dir + merged_dir);
	fs::create_directories(path + main_dir + mask_dir);
	fs::create_directories(path + main_dir + json_dir);
}

////////// makeGenFileTree //////////
void makeGenFileTree(const std::string& path, const std::string& main_dir,
	const std::string& glut_dir, const std::string& merged_dir,
	const std::string& mask_dir, const std::string& tex_dir,
	const std::string& json_dir) {
	namespace fs = std::filesystem;

	//check if path exists
	fs::file_status s = fs::file_status{};
	if (!fs::exists(path))
		throw std::invalid_argument("Path does not exists");

	//check if main_dir exists
	if (fs::exists(path + main_dir))
		fs::remove_all(path + main_dir);

	//making subdirectories
	fs::create_directories(path + main_dir + glut_dir);
	fs::create_directories(path + main_dir + merged_dir);
	fs::create_directories(path + main_dir + mask_dir);
	fs::create_directories(path + main_dir + tex_dir);
	fs::create_directories(path + main_dir + json_dir);
}
