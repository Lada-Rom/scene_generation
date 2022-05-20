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

////////// mergeGLUTandCVImage //////////
void mergeGLUTandCVImage(const std::string& src_filename,
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

////////// mergeGLUTandCVImage //////////
void mergeGLUTandCVImage(const std::string& src_filename,
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
