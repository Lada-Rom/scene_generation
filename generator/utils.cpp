#include "utils.h"

namespace add_cv {

void cross(cv::InputOutputArray& img, const cv::Point point,
	const cv::Size& size, const cv::Scalar& color) {
	cv::line(img,
		cv::Point2d{ 1.0 * point.x - size.width, 1.0 * point.y },
		cv::Point2d{ 1.0 * point.x + size.width, 1.0 * point.y }, color);
	cv::line(img,
		cv::Point2d{ 1.0 * point.x, 1.0 * point.y - size.height },
		cv::Point2d{ 1.0 * point.x, 1.0 * point.y + size.height }, color);
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
