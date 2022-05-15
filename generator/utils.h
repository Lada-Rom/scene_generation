#ifndef UTILS_UTILS_H_20221505
#define UTILS_UTILS_H_20221505

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <opencv2/opencv.hpp>

namespace cv {

void cross(cv::InputOutputArray& img, const cv::Point point,
	const cv::Size& size, const cv::Scalar& color);

} //namespace cv

namespace glm {

std::ostream& operator<<(std::ostream& ostrm, const glm::dmat3& rhs);
std::ostream& operator<<(std::ostream& ostrm, const glm::dmat4& rhs);
std::ostream& operator<<(std::ostream& ostrm, const glm::dvec3& rhs);

} //namespace glm

std::ostream& operator<<(std::ostream& ostrm, const std::array<double, 9>& rhs);
std::ostream& operator<<(std::ostream& ostrm, const std::array<double, 3>& rhs);

#endif
