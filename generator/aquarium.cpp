#include "aquarium.h"

////////// Aquarium //////////
Aquarium::Aquarium() {
    calcVerticies();
}

////////// getSize //////////
std::array<double, 3> Aquarium::getSize() {
	return size_;
}

////////// getVerticies //////////
std::array<std::array<double, 3>, 8> Aquarium::getVertices() {
    return vertices_;
}

////////// getObjpoints //////////
std::vector<cv::Point3d> Aquarium::getObjpoints() {
    std::vector<cv::Point3d> objpoints;
    for (const auto& point : vertices_) {
        objpoints.push_back(cv::Point3d(point[0], point[1], point[2]));
    }
    return objpoints;
}

////////// calcVerticies //////////
void Aquarium::calcVerticies() {
    vertices_ = {
        std::array<double, 3>{ -0.5 * size_[0], -0.5 * size_[1], 0. },
        std::array<double, 3>{  0.5 * size_[0], -0.5 * size_[1], 0. },
        std::array<double, 3>{  0.5 * size_[0],  0.5 * size_[1], 0. },
        std::array<double, 3>{ -0.5 * size_[0],  0.5 * size_[1], 0. },
        std::array<double, 3>{ -0.5 * size_[0], -0.5 * size_[1], -size_[2] },
        std::array<double, 3>{  0.5 * size_[0], -0.5 * size_[1], -size_[2] },
        std::array<double, 3>{  0.5 * size_[0],  0.5 * size_[1], -size_[2] },
        std::array<double, 3>{ -0.5 * size_[0],  0.5 * size_[1], -size_[2] },
    };
}
