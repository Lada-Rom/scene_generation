#ifndef UTILS_UTILS_H_20221505
#define UTILS_UTILS_H_20221505

#include <filesystem>
#include <random>

#include "texture.h"

#include <stb_image.h>
#include <stb_image_write.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <GL/freeglut.h>

using json = nlohmann::json;

namespace add_cv {

void cross(cv::InputOutputArray& img, const cv::Point& point,
	const cv::Size& size, const cv::Scalar& color);

void cross(cv::InputOutputArray& img, const std::array<double, 2>& point,
	const cv::Size& size, const cv::Scalar& color);

void mergeUntexturedImageAndPoints(const std::string&,
	const std::vector<cv::Point2d>&, const std::string&, const std::string&);
void mergeUntexturedImageAndPoints(const std::string&,
	const std::vector<std::array<double, 2>>&, const std::string&, const std::string&);

cv::Mat mergeTexturedImageWithSource(const cv::Mat& mask, const cv::Mat& src_image,
	const std::string& glut_filename, const std::string& dst_filename);
cv::Mat mergeTexturedImageWithSource(const cv::Mat& mask, const cv::Mat& src_image,
	const std::string& glut_filename);
cv::Mat mergeTexturedImageWithSource(
	const std::vector<std::array<double, 2>>& imgpoints,
	const cv::Mat& mask, const cv::Mat& src_image,
	const std::string& glut_filename, const std::string& dst_filename);

void smoothObjectBorders(cv::Mat& image, const cv::Mat& mask);
void smoothObjectBorders(cv::Mat& image, const cv::Mat& mask, const std::string& dst_filename);
void smoothReflectionBorders(cv::Mat& image, const std::string& mask_filename);
void smoothReflectionBorders(cv::Mat& image, const std::string& mask_filename, const std::string& dst_filename);

cv::Mat supplementImage(const cv::Mat& src, unsigned int size);

void textureDaphnia(cv::Mat& img, cv::Mat& obj_texture,
	const std::array<double, 2>& center, const std::array<double, 2>& direction);
void textureFrameDaphnias(size_t frame_index, cv::Mat& img, const json& gen_json,
	std::random_device& rd, const std::string& texture_path,
	const std::string& dst_filename, const std::string& format);

cv::Mat makeHeatmap(const cv::Mat& mask, const std::string& filename);

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

void makeGenFileTree(const std::string& path, const std::string& main_dir,
	const std::string& glut_dir, const std::string& merged_dir, const std::string&);
void makeGenFileTree(const std::string& path, const std::string& main_dir,
	const std::string& glut_dir, const std::string& merged_dir,
	const std::string& mask_dir, const std::string& json_dir);
void makeGenFileTree(const std::string& path, const std::string& main_dir,
	const std::string& glut_dir, const std::string& merged_dir,
	const std::string& mask_dir, const std::string& tex_dir,
	const std::string& json_dir);
void makeGenFileTree(const std::string& path, const std::string& main_dir,
	const std::string& glut_dir, const std::string& merged_dir,
	const std::string& mask_dir, const std::string& tex_dir,
	const std::string& video_dir, const std::string& json_dir);
void makeGenFileTree(const std::string& path, const std::string& main_dir,
	const std::string& glut_dir, const std::string& merged_dir,
	const std::string& obj_mask_dir, const std::string& refl_mask_dir,
	const std::string& tex_dir, const std::string& video_dir,
	const std::string& json_dir);
void makeGenFileTree(const std::string& path, const std::string& main_dir,
	const std::string& glut_dir, const std::string& merged_dir,
	const std::string& obj_mask_dir, const std::string& refl_mask_dir,
	const std::string& heatmap_dir,
	const std::string& tex_dir, const std::string& video_dir,
	const std::string& json_dir);

#endif
