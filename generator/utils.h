#ifndef UTILS_UTILS_H_20221505
#define UTILS_UTILS_H_20221505

#include <opencv2/opencv.hpp>

namespace cv {

void cross(cv::InputOutputArray& img, const cv::Point point,
	const cv::Size& size, const cv::Scalar& color);

} // namespace cv

#endif
