#include "camera.h"

////////// getAngle //////////
double Camera::getAngle() {
	return angle_;
}

////////// getIntrinsicMatrix //////////
std::array<double, 9> Camera::getIntrinsicMatrix() {
	return intrinsic_matrix_;
}

////////// getRMat //////////
glm::dmat4 Camera::getRMat() {
	return rmat_;
}

////////// getTVec //////////
glm::dvec3 Camera::getTVec() {
	return tvec_;
}

////////// setOpticalCenter //////////
void Camera::setOpticalCenter(const cv::Size2d& image_size) {
	optical_center_ = {
		0.5 * (image_size.width - 1),
		0.5 * (image_size.height - 1) };
}

////////// setRMat //////////
void Camera::setRMat(const std::array<double, 9>& rmat) {
	glm::dmat3 rmat3 = glm::make_mat3(rmat.data());
	for (int i{}; i < 3; ++i)
		for (int j{}; j < 3; ++j)
			rmat_[i][j] = rmat3[i][j];
	rmat_[3][3] = 1.;
}

////////// setTVec //////////
void Camera::setTVec(const std::array<double, 3>& tvec) {
	tvec_ = glm::make_vec3(tvec.data());
}

////////// calcIntrinsicMatrix //////////
void Camera::calcIntrinsicMatrix() {
	intrinsic_matrix_ = {
		focal_length_.width, 0., optical_center_.x,
		0., focal_length_.height, optical_center_.y,
		0., 0., 1.
	};
}
