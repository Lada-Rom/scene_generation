#include <iostream>
#include <exception>

#include "camera_params.h"

#include <opencv2/opencv.hpp>
#include <GL/freeglut.h>

void main(int argc, char** argv) {
try {
    glutInit(&argc, argv);
    cv::Mat src = cv::imread("../../data/src/src.1.png", cv::IMREAD_GRAYSCALE);
    if (src.empty())
        throw std::exception("Source image not found");

    //3D points
    std::vector<std::vector<cv::Point3d>> objpoints;
    main_scene.addWHDToAquarium(0.8, 0.5, -0.4, true);
    //objpoints.push_back({
    //    {-8.25, 7.25, -2.5}, {8.25, 7.25, -2.5}, {8.25, -7.25, -2.5}, {-8.25, -7.25, -2.5},
    //    {-8.25, 7.25, 0}, {8.25, 7.25, 0}, {8.25, -7.25, 0}, {-8.25, -7.25, 0} });
    objpoints.push_back( getObjPoints() );

    //2D points
    std::vector<std::vector<cv::Point2d>> imgpoints = getImgpointsFromJSON("../../data/json/annotation.json");

    //camera params
    double camera_data[] = { 1509.434, 0, (1280 - 1) * 0.5, 0, 1509.434, (1024 - 1) * 0.5, 0, 0, 1 };
    cv::Mat camera_mat = cv::Mat(3, 3, CV_64FC1, camera_data);

    //draw 5 different results
    //showDifferentDepth(src, objpoints, imgpoints, camera_mat, 7, 0.1, 0.8, 0.5);
    //showDifferentDepth(src, objpoints, imgpoints, camera_mat, 10, 0.1, 0, 0);

    //check point correspondence
    showPointCorrespondence(src, objpoints[0], imgpoints[0], camera_mat, {10, 8});

    //select eye param
    //paramsSelectionInnerGL(src, objpoints[0], imgpoints[0], camera_mat, { 2, 2 },
    //    { 1., 1., 1. }, { -1, 0 }, { -1, 0 }, { -1, 0 }, "../../data/params_selection/", argc, argv);
    //paramsSelectionOuterGL(src, objpoints[0], imgpoints[0], camera_mat, { 8, 8 },
    //    { 0.1, 0.1, 0.1 }, { -2, 1 }, { 0, 0 }, { 0, 0 }, "../../data/params_selection/");

    //testdisplay(objpoints[0], imgpoints[0], camera_mat);
}
catch (const std::exception& ex) {
    std::cout << ex.what() << std::endl;
}
}