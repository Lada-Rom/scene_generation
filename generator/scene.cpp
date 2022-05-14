#include "scene.h"

////////// Scene //////////
Scene::Scene() {
	camera_.setOpticalCenter(render_image_size_);
	camera_.calcIntrinsicMatrix();
}

////////// getIntrinsicCameraMatrix //////////
std::array<double, 9> Scene::getIntrinsicCameraMatrix() {
    return camera_.getIntrinsicMatrix();
}

////////// Scene //////////
void Scene::calcOuterCameraParams(const std::vector<cv::Point2d>& imgpoints,
    const cv::Mat& camera_mat, cv::Mat& dist_coeffs, cv::Mat& rvec, cv::Mat& tvec) {
    std::vector<cv::Point3d> objpoints = aquarium_.getObjpoints();

    std::vector<cv::Point3d> objpoints_4;
    std::vector<cv::Point2d> imgpoints_4;

    //second 4 points
    objpoints_4 = std::vector<cv::Point3d>(objpoints.begin() + 4, objpoints.begin() + 8);
    imgpoints_4 = std::vector<cv::Point2d>(imgpoints.begin() + 4, imgpoints.begin() + 8);

    //find rotation and translation
    cv::solvePnP(objpoints_4, imgpoints_4, camera_mat, dist_coeffs, rvec, tvec, false, cv::SOLVEPNP_AP3P);

    //first 4 points
    objpoints_4 = std::vector<cv::Point3d>(objpoints.begin(), objpoints.begin() + 4);
    imgpoints_4 = std::vector<cv::Point2d>(imgpoints.begin(), imgpoints.begin() + 4);

    //optimize rotation and translation
    cv::solvePnP(objpoints_4, imgpoints_4, camera_mat, dist_coeffs, rvec, tvec, true, cv::SOLVEPNP_AP3P);

    //refine values
    cv::solvePnPRefineVVS(objpoints, imgpoints, camera_mat, dist_coeffs, rvec, tvec);
}
