#include "camera.h"

////////// getIntrinsicMatrix //////////
std::array<double, 9> Camera::getIntrinsicMatrix() {
	return intrinsic_matrix_;
}

////////// setOpticalCenter //////////
void Camera::setOpticalCenter(const cv::Size2d& image_size) {
	optical_center_ = {
		0.5 * (image_size.width - 1),
		0.5 * (image_size.height - 1) };
}

////////// calcIntrinsicMatrix //////////
void Camera::calcIntrinsicMatrix() {
	intrinsic_matrix_ = {
		focal_length_.width, 0., optical_center_.x,
		0., focal_length_.height, optical_center_.y,
		0., 0., 1.
	};
}
