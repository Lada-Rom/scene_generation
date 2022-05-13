#ifndef CAMERA_CAMERA_H_20221305
#define CAMERA_CAMERA_H_20221305

#include <array>
#include <opencv2/opencv.hpp>

class Camera {
public:
	Camera() = default;
	~Camera() = default;

	void setOpticalCenter(const cv::Size2d&);
	void calcIntrinsicMatrix();

private:
	cv::Size2d focal_length_{ 8000 / 5.3, 8000 / 5.3 };
	cv::Point2d optical_center_{ };

	std::array<double, 9> intrinsic_matrix_{ };
};

#endif
