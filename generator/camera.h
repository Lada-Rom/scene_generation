#ifndef CAMERA_CAMERA_H_20221305
#define CAMERA_CAMERA_H_20221305

#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <opencv2/opencv.hpp>

class Camera {
public:
	Camera() = default;
	~Camera() = default;

	double getAngle();
	std::array<double, 9> getIntrinsicMatrix();
	glm::dmat4 getRMat();
	glm::dvec3 getTVec();
	glm::dvec3 getSVec();

	void setOpticalCenter(const cv::Size2d& image_size);
	void setRMat(const std::array<double, 9>& rmat);
	void setTVec(const std::array<double, 3>& tvec);
	void setSVec(const std::array<double, 3>& svec);

	void calcIntrinsicMatrix();

private:
	cv::Size2d focal_length_{ 8000 / 5.3, 8000 / 5.3 };
	cv::Point2d optical_center_{ };
	double angle_{ 2 * 180 / std::acos(-1) * std::atan(5.43 / (2 * 8))}; //37.49

	std::array<double, 9> intrinsic_matrix_{ };
	glm::dmat4 rmat_{ };
	glm::dvec3 tvec_{ };
	glm::dvec3 svec_{ };
};

#endif
