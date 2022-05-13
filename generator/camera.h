#ifndef CAMERA_CAMERA_H_20221305
#define CAMERA_CAMERA_H_20221305

#include <opencv2/opencv.hpp>

class Camera {
public:
	Camera() = default;
	~Camera() = default;

private:
	cv::Size2d focal_length_{ };
	cv::Point2d optical_center_{ };
};

#endif
