#include "camera.h"

////////// setOpticalCenter //////////
void Camera::setOpticalCenter(const cv::Size2d& image_size) {
	optical_center_ = {
		0.5 * (image_size.width - 1),
		0.5 * (image_size.height - 1) };
}
