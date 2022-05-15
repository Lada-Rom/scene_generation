#include "utils.h"

namespace cv {

void cross(cv::InputOutputArray& img, const cv::Point point,
	const cv::Size& size, const cv::Scalar& color) {
	cv::line(img,
		cv::Point2d{ 1.0 * point.x - size.width, 1.0 * point.y },
		cv::Point2d{ 1.0 * point.x + size.width, 1.0 * point.y }, color);
	cv::line(img,
		cv::Point2d{ 1.0 * point.x, 1.0 * point.y - size.height },
		cv::Point2d{ 1.0 * point.x, 1.0 * point.y + size.height }, color);
}

} // namespace cv
