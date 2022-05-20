#ifndef UTILS_UTILS_H_20221505
#define UTILS_UTILS_H_20221505

#include <filesystem>

#include "texture.h"

#include <stb_image.h>
#include <stb_image_write.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <opencv2/opencv.hpp>
#include <GL/freeglut.h>

namespace add_cv {

void cross(cv::InputOutputArray& img, const cv::Point& point,
	const cv::Size& size, const cv::Scalar& color);

void cross(cv::InputOutputArray& img, const std::array<double, 2>& point,
	const cv::Size& size, const cv::Scalar& color);

void mergeUntexturedImageAndPoints(const std::string&,
	const std::vector<cv::Point2d>&, const std::string&, const std::string&);
void mergeUntexturedImageAndPoints(const std::string&,
	const std::vector<std::array<double, 2>>&, const std::string&, const std::string&);
void mergeTexturedImageWithSource(const cv::Mat& mask, const cv::Mat& src_image,
	const std::string& glut_filename, const std::string& dst_filename);
void mergeTexturedImageWithSource(
	const std::vector<std::array<double, 2>>& imgpoints,
	const cv::Mat& mask, const cv::Mat& src_image,
	const std::string& glut_filename, const std::string& dst_filename);

} //namespace add_cv

namespace glm {

std::ostream& operator<<(std::ostream& ostrm, const glm::dmat3& rhs);
std::ostream& operator<<(std::ostream& ostrm, const glm::dmat4& rhs);
std::ostream& operator<<(std::ostream& ostrm, const glm::dvec3& rhs);

} //namespace glm

void saveImage(int& width, int& height, std::string& filename);
void loadTexture(Texture& texture);

uchar median(std::vector<uchar>& vec);

std::ostream& operator<<(std::ostream& ostrm, const std::array<double, 9>& rhs);
std::ostream& operator<<(std::ostream& ostrm, const std::array<double, 3>& rhs);

std::ostream& operator<<(std::ostream& ostrm, const std::vector<cv::Point3d>& rhs);
std::ostream& operator<<(std::ostream& ostrm, const std::array<std::array<double, 3>, 8>& rhs);

void makeGenFileTree(const std::string&, const std::string&,
	const std::string&, const std::string&, const std::string&);

#endif
