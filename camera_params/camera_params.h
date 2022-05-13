#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>

#include "scene.h"

#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>

# define PI 3.14159265358979323846
using json = nlohmann::json;

Scene main_scene;


void displayPointCorrespondence() {
    main_scene.displayPointCorrespondence();
}


void displayPointCorrespondencev2() {
    main_scene.displayPointCorrespondencev2();
}


void displayPointCorrespondenceParamsSelectionInnerGL() {
    main_scene.displayPointCorrespondenceParamsSelectionInnerGL();
}

void displayPointCorrespondenceParamsSelectionOuterGL() {
    main_scene.displayPointCorrespondenceParamsSelectionOuterGL();
}


void display() {
    main_scene.display();
}



void reshape(int width, int heght) {
    main_scene.reshapeContent(width, heght);
}


std::vector<cv::Point3d> getObjPoints() {
    std::vector<double> verticies = main_scene.getAquariumVerticies();
    double scale = main_scene.getScalingFactor();
    //double scale = 1.;

    std::vector<cv::Point3d> objpoints;
    for (int i{}; i < verticies.size(); i += 3)
        objpoints.push_back({ scale * verticies[i], scale * verticies[i + 1], scale * verticies[i + 2]});

    return objpoints;
}


std::vector<std::vector<cv::Point2d>> getImgpointsFromJSON(const std::string& filename) {
    //read file
    std::ifstream project_annotation_fstream(filename);
    if (project_annotation_fstream.fail())
        throw std::exception("Source JSON file not found");
    json project_annotation;
    project_annotation_fstream >> project_annotation;

    //getting data arrays
    json regions = project_annotation["_via_img_metadata"]["cp.src1.png1010197"]["regions"];
    json first_part = regions[0]["shape_attributes"];
    json second_part = regions[1]["shape_attributes"];

    std::vector<cv::Point2d> imgpoints;
    for (auto i = 0; i < 4; ++i)
        imgpoints.push_back({ (double)first_part["all_points_x"][i], (double)first_part["all_points_y"][i] });
    for (auto i = 0; i < 4; ++i)
        imgpoints.push_back({ (double)second_part["all_points_x"][i], (double)second_part["all_points_y"][i] });

    return { imgpoints };
}


void getOuterCameraParams(std::vector<cv::Point3d> objpoints, const std::vector<cv::Point2d>& imgpoints,
    const cv::Mat& camera_mat, cv::Mat& dist_coeffs, cv::Mat& rvec, cv::Mat& tvec,
    const float ddepth = 0, const float dwidth = 0, const float dheight = 0) {

    for (int i{}; i < objpoints.size(); ++i) {
        objpoints[i].x += (objpoints[i].x > 0) ? dwidth : -dwidth;
        objpoints[i].y += (objpoints[i].y > 0) ? dheight : -dheight;
        objpoints[i].z += (objpoints[i].z > 0) ? ddepth : -ddepth;
    }

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
    //cv::solvePnPRefineLM(objpoints, imgpoints, camera_mat, dist_coeffs, rvec, tvec);
    //cv::solvePnPRansac(objpoints, imgpoints, camera_mat, dist_coeffs, rvec, tvec, true);
}


void getPredictPoints(std::vector<cv::Point2d>& predpoints,
    const std::vector<cv::Point3d>& objpoints,
    const cv::Mat& camera_mat, const cv::Mat& rmat, const cv::Mat& tvec) {

    cv::Mat objpoint_mat;
    cv::Mat predpoint_mat;
    cv::Point3d predpoint;
    for (auto& objpoint : objpoints) {
        //convert point to mat
        std::vector<cv::Point3d> temp = { objpoint };
        objpoint_mat = cv::Mat(3, 1, CV_64FC1, temp.data());

        //calc prediction mat
        predpoint_mat = camera_mat * (rmat * objpoint_mat + tvec);

        //build point from prediction
        predpoint.x = predpoint_mat.at<double>(0, 0);
        predpoint.y = predpoint_mat.at<double>(1, 0);
        predpoint.z = predpoint_mat.at<double>(2, 0);
        predpoints.push_back({predpoint.x / predpoint.z, predpoint.y / predpoint.z});
    }
}


void drawAquarium(cv::Mat& dst,
    const std::vector<cv::Point2d>& imgpoints, 
    const std::vector<cv::Point2d>& predpoints) {

    //draw points from json
    for (const auto& point : imgpoints) {
        cv::circle(dst, point, 1.5, { 0, 0, 255 }, -1);
    }

    //draw lines
    cv::line(dst, predpoints[0], predpoints[1], { 255, 0, 0 });
    cv::line(dst, predpoints[1], predpoints[2], { 255, 0, 0 });
    cv::line(dst, predpoints[2], predpoints[3], { 255, 0, 0 });
    cv::line(dst, predpoints[3], predpoints[0], { 255, 0, 0 });

    cv::line(dst, predpoints[4], predpoints[5], { 255, 0, 0 });
    cv::line(dst, predpoints[5], predpoints[6], { 255, 0, 0 });
    cv::line(dst, predpoints[6], predpoints[7], { 255, 0, 0 });
    cv::line(dst, predpoints[7], predpoints[4], { 255, 0, 0 });
}


void cross(cv::InputOutputArray img, const cv::Point point, const cv::Size& size, const cv::Scalar& color) {
    cv::line(img,
        cv::Point2d{ 1.0 * point.x - size.width, 1.0 * point.y },
        cv::Point2d{ 1.0 * point.x + size.width, 1.0 * point.y }, color);
    cv::line(img,
        cv::Point2d{ 1.0 * point.x, 1.0 * point.y - size.height },
        cv::Point2d{ 1.0 * point.x, 1.0 * point.y + size.height }, color);
}


void showDifferentDepth(const cv::Mat& src,
    const std::vector<std::vector<cv::Point3d>>& src_objpoints,
    const std::vector<std::vector<cv::Point2d>>& imgpoints,
    const cv::Mat& camera_mat,
    const size_t experiment_num = 5, const float step = 0.1,
    const float dwidth = 0.25, const float dheight = 0.25) {

    size_t count = 0;

    //array of objpoints array with varying z
    std::vector< std::vector<std::vector<cv::Point3d>> > modified_objpoints{ experiment_num };
    for (auto& e : modified_objpoints) {
        e.push_back({});
        for (int i{}; i < src_objpoints[0].size(); ++i) {
            cv::Point3d point = src_objpoints[0][i];
            if (i >= 4)
                e[0].push_back({ point.x, point.y, point.z + count * step }); //depth is getting smaller
            else
                e[0].push_back({ point.x, point.y, point.z });
            e[0][e[0].size() - 1].x += (e[0][e[0].size() - 1].x > 0) ? dwidth : -dwidth;
            e[0][e[0].size() - 1].y += (e[0][e[0].size() - 1].y > 0) ? dheight : -dheight;
        }
        ++count;
    }

    //array of 3-channel images with src appearence
    std::vector<cv::Mat> dst_3_images{ experiment_num };
    for (auto& dst_image : dst_3_images)
        cv::merge(std::array<cv::Mat, 3>{ src, src, src }, dst_image);

    //for all dst images
    cv::Mat distortion_coeffs, rotation_vec, translation_vec;
    for (int i{}; i < experiment_num; ++i) {
        //recalc outer camera params
        getOuterCameraParams(modified_objpoints[i][0], imgpoints[0], camera_mat,
            distortion_coeffs, rotation_vec, translation_vec);

        //get camera mat from vec
        cv::Mat rotation_mat;
        cv::Rodrigues(rotation_vec, rotation_mat);

        //predict points with new params
        std::vector<cv::Point2d> predpoints;
        getPredictPoints(predpoints, modified_objpoints[i][0],
            camera_mat, rotation_mat, translation_vec);

        //draw result - src points, predicted lines
        drawAquarium(dst_3_images[i], imgpoints[0], predpoints);
        std::cout << "img " << i << ":  w " << 2 * abs(modified_objpoints[i][0][0].x)
            << ",  h " << 2 * abs(modified_objpoints[i][0][0].y)
            << ", d " << abs(modified_objpoints[i][0][4].z) << std::endl;
    }   
    
}


void showPointCorrespondence(const cv::Mat& src,
    std::vector<cv::Point3d> objpoints, const std::vector<cv::Point2d>& imgpoints,
    const cv::Mat& camera_mat, const cv::Size& quantity) {

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 1024);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Point array");

    //get outer params
    cv::Mat distortion_coeffs, rotation_vec, translation_vec;
    getOuterCameraParams(objpoints, imgpoints, camera_mat,
        distortion_coeffs, rotation_vec, translation_vec);
    cv::Mat rotation_mat;
    cv::Rodrigues(rotation_vec, rotation_mat);

    //set outer params to OpenGL
    main_scene.initializeGLUT();
    main_scene.setRotationMatrix(rotation_mat);
    main_scene.setTranslationVector(translation_vec);
    main_scene.calcLookAtArray();
    
    //set experimental params
    //  points for grid in 3D (aquarium)
    std::vector<cv::Point3d> objgridpoints; 
    double width_step = main_scene.getAquariumWidth() / (quantity.width - 1);
    double height_step = main_scene.getAquariumHeight() / (quantity.height - 1);
    std::cout << "\nStep:  w " << width_step << "  h " << height_step << std::endl;
    for (int i{}; i < quantity.width; ++i) {
        for (int j{}; j < quantity.height; ++j) {
            objgridpoints.push_back({
                main_scene.getScalingFactor() * (-0.5 * main_scene.getAquariumWidth() + i * width_step),
                main_scene.getScalingFactor() * (-0.5 * main_scene.getAquariumHeight() + j * height_step), 0 }); 
        }
    }

    //printing grid values
    std::cout << "Horizontal values: \t";
    for (int i{}; i < objgridpoints.size(); i += quantity.height)
        std::cout << objgridpoints[i].x << ", ";

    std::cout << "\nVertical values: \t";
    for (int i{}; i < quantity.height; ++i)
        std::cout << objgridpoints[i].y << ", ";
    std::cout << std::endl;

    //  point for grid in 2D (image)
    std::vector<cv::Point2d> imggridpoints; 
    getPredictPoints(imggridpoints, objgridpoints, camera_mat, rotation_mat, translation_vec);
    main_scene.setObjGridPoints(objgridpoints);
    //main_scene.setImgGridPoints(imggridpoints);

    //glut rendering
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutDisplayFunc(displayPointCorrespondence);
    glutReshapeFunc(reshape);
    glutMainLoop();

    //merging glut and opencv
    cv::Mat grid_glut = cv::imread("../../data/grid/grid_glut.png", cv::IMREAD_GRAYSCALE);

    cv::Mat grid_merged_1c;
    cv::bitwise_and(src, grid_glut, grid_merged_1c, grid_glut);

    cv::Mat grid_merged_3c;
    cv::merge(std::array<cv::Mat, 3>{ grid_merged_1c, grid_merged_1c, grid_merged_1c }, grid_merged_3c);

    for (auto& point : imggridpoints) {
        //cv::circle(grid_merged, point, 1, {0, 0, 255});
        cv::line(grid_merged_3c,
            cv::Point2d{ point.x - 2, point.y }, cv::Point2d{ point.x + 2, point.y }, {0, 0, 255});
        cv::line(grid_merged_3c,
            cv::Point2d{ point.x, point.y - 2 }, cv::Point2d{ point.x, point.y + 2 }, {0, 0, 255});
    }

    cv::imwrite("../../data/grid/grid_merged_3c.png", grid_merged_3c);

    std::cout << "Hello!" << std::endl;
}


void showPointCorrespondencev2(const cv::Mat& src,
    std::vector<cv::Point3d> objpoints, const std::vector<cv::Point2d>& imgpoints,
    const cv::Mat& camera_mat, const cv::Size& quantity) {

    //get outer params
    cv::Mat distortion_coeffs, rotation_vec, translation_vec;
    getOuterCameraParams(objpoints, imgpoints, camera_mat,
        distortion_coeffs, rotation_vec, translation_vec);
    cv::Mat rotation_mat;
    cv::Rodrigues(rotation_vec, rotation_mat);

    //set outer params to OpenGL
    main_scene.setRotationMatrix4(rotation_mat);
    main_scene.setTranslationVector(translation_vec);
    //main_scene.calcLookAtArray();

    //set experimental params
    //  points for grid in 3D (aquarium)
    std::vector<cv::Point3d> objgridpoints;
    double width_step = main_scene.getAquariumWidth() / (quantity.width - 1);
    double height_step = main_scene.getAquariumHeight() / (quantity.height - 1);
    std::cout << "\nStep:  w " << width_step << "  h " << height_step << std::endl;
    for (int i{}; i < quantity.width; ++i) {
        for (int j{}; j < quantity.height; ++j) {
            objgridpoints.push_back({
                /*main_scene.getScalingFactor() * */(-0.5 * main_scene.getAquariumWidth() + i * width_step),
                /*main_scene.getScalingFactor() * */(-0.5 * main_scene.getAquariumHeight() + j * height_step), 0 });
        }
    }

    //printing grid values
    std::cout << "Horizontal values: \t";
    for (int i{}; i < objgridpoints.size(); i += quantity.height)
        std::cout << objgridpoints[i].x << ", ";

    std::cout << "\nVertical values: \t";
    for (int i{}; i < quantity.height; ++i)
        std::cout << objgridpoints[i].y << ", ";
    std::cout << std::endl;

    //  point for grid in 2D (image)
    std::vector<cv::Point2d> imggridpoints;
    getPredictPoints(imggridpoints, objgridpoints, camera_mat, rotation_mat, translation_vec);
    main_scene.setObjGridPoints(objgridpoints);
    //main_scene.setImgGridPoints(imggridpoints);

    //glut rendering
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 1024);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Point array");

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutDisplayFunc(displayPointCorrespondencev2);
    glutReshapeFunc(reshape);
    main_scene.initializeGLUT();
    glutMainLoop();

    //merging glut and opencv
    cv::Mat grid_glut = cv::imread("../../data/grid/grid_glut.png", cv::IMREAD_GRAYSCALE);
    //cv::Mat grid_glut_flipped;
    //cv::flip(grid_glut, grid_glut_flipped, 0);

    cv::Mat grid_merged_1c;
    cv::bitwise_and(src, grid_glut, grid_merged_1c, grid_glut);
    //cv::bitwise_and(src, grid_glut_flipped, grid_merged_1c, grid_glut_flipped);

    cv::Mat grid_merged_3c;
    cv::merge(std::array<cv::Mat, 3>{ grid_merged_1c, grid_merged_1c, grid_merged_1c }, grid_merged_3c);

    for (auto& point : imggridpoints) {
        cross(grid_merged_3c, point, { 2, 2 }, { 0, 0, 255 });
    }

    cv::imwrite("../../data/grid/grid_merged_3c.png", grid_merged_3c);

    std::cout << "Hello!" << std::endl;
}


std::string getFilenameParamsSelection(std::stringstream& sstream, const std::string& path,
    const size_t count, const double i, const double j, const double k, const std::string& ending) {
    sstream.str(std::string());
    sstream << path << std::to_string(count) << std::fixed << std::setprecision(2)
        << "_x" << i
        << "_y" << j
        << "_z" << k << ending;
    return sstream.str();
}


void paramsSelectionInnerGL(const cv::Mat& src,
    std::vector<cv::Point3d> objpoints, const std::vector<cv::Point2d>& imgpoints,
    const cv::Mat& camera_mat, const cv::Size& quantity, const cv::Point3d& deye,
    const std::vector<double>& reye0, const std::vector<double>& reye1, const std::vector<double>& reye2,
    const std::string& path, int argc, char** argv) {
    
    if ((reye0.size() != 2) || (reye1.size() != 2) || (reye2.size() != 2))
        throw std::length_error("reye must containt only start and end points");

    //get outer params
    cv::Mat distortion_coeffs, rotation_vec, translation_vec;
    getOuterCameraParams(objpoints, imgpoints, camera_mat,
        distortion_coeffs, rotation_vec, translation_vec);
    cv::Mat rotation_mat;
    cv::Rodrigues(rotation_vec, rotation_mat);

    //set outer params to OpenGL
    main_scene.setRotationMatrix(rotation_mat);
    main_scene.setTranslationVector(translation_vec);

    //set experimental params
    //  points for grid in 3D (aquarium)
    std::vector<cv::Point3d> objgridpoints;
    double width_step = main_scene.getAquariumWidth() / (quantity.width - 1);
    double height_step = main_scene.getAquariumHeight() / (quantity.height - 1);
    std::cout << "\nStep:  w " << width_step << "  h " << height_step << std::endl;
    for (int i{}; i < quantity.width; ++i) {
        for (int j{}; j < quantity.height; ++j) {
            objgridpoints.push_back({
                main_scene.getScalingFactor() * (-0.5 * main_scene.getAquariumWidth() + i * width_step),
                main_scene.getScalingFactor() * (-0.5 * main_scene.getAquariumHeight() + j * height_step), 0 });
        }
    }

    //printing grid values
    std::cout << "Grid horizontal values: \t";
    for (int i{}; i < objgridpoints.size(); i += quantity.height)
        std::cout << objgridpoints[i].x << ", ";

    std::cout << "\nGrid vertical values: \t";
    for (int i{}; i < quantity.height; ++i)
        std::cout << objgridpoints[i].y << ", ";
    std::cout << "\n" << std::endl;

    //  point for grid in 2D (image)
    std::vector<cv::Point2d> imggridpoints;
    getPredictPoints(imggridpoints, objgridpoints, camera_mat, rotation_mat, translation_vec);
    main_scene.setObjGridPoints(objgridpoints);

    //filling array of eye values
    std::vector<double> xrange;
    std::vector<double> yrange;
    std::vector<double> zrange;

    std::cout << "\nX eye range: \t";
    for (double i{ reye0[0] }; i <= reye0[1]; i += deye.x) {
        xrange.push_back(i);
        std::cout << i << ", ";
    }
    std::cout << "\nY eye range: \t";
    for (double j{ reye1[0] }; j <= reye1[1]; j += deye.y) {
        yrange.push_back(j);
        std::cout << j << ", ";
    }
    std::cout << "\nZ eye range: \t";
    for (double k{ reye2[0] }; k <= reye2[1]; k += deye.z) {
        zrange.push_back(k);
        std::cout << k << ", ";
    }
    std::cout << "\n" << std::endl;

    //glut rendering to file (name: {count}_x{xval}_y{yval}_z{zval}.png)
    size_t count { 1 };
    std::stringstream sstream_4_names;
    std::string filename_glut;
    for (int i{ }; i < xrange.size(); ++i) {
        for (int j{ }; j < yrange.size(); ++j) {
            for (int k{ }; k < zrange.size(); ++k) {
                
                //construct filename and refresh lootAt params
                filename_glut = getFilenameParamsSelection(sstream_4_names, path, count,
                    xrange[i], yrange[j], zrange[k], ".png");
                std::cout << filename_glut << std::endl;
                main_scene.setFilenameParamsSelection(filename_glut);
                main_scene.calcLookAtArray(xrange[i], yrange[j], zrange[k]);

                //glut initialization
                if (!glutGet(GLUT_INIT_STATE))
                    glutInit(&argc, argv);
                glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
                glutInitWindowSize(1280, 1024);
                glutInitWindowPosition(0, 0);
                glutCreateWindow("Point array");

                glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
                glutDisplayFunc(displayPointCorrespondenceParamsSelectionInnerGL);
                glutReshapeFunc(reshape);
                main_scene.initializeGLUT();
                glutMainLoop();
                ++count;
            }
        }
    }
    std::cout << std::endl;

    //merging glut and opencv
    count = 1;
    std::string filename_opencv;
    cv::Mat grid_glut, grid_merged_1c, grid_merged_3c;
    for (int i{ }; i < xrange.size(); ++i) {
        for (int j{ }; j < yrange.size(); ++j) {
            for (int k{ }; k < zrange.size(); ++k) {

                //read glut output
                filename_glut = getFilenameParamsSelection(sstream_4_names, path, count,
                    xrange[i], yrange[j], zrange[k], ".png");
                grid_glut = cv::imread(filename_glut, cv::IMREAD_GRAYSCALE);

                //merge glut output with src image
                grid_merged_1c.release();
                cv::bitwise_and(src, grid_glut, grid_merged_1c, grid_glut);

                //making 3c image from previous step
                cv::merge(std::array<cv::Mat, 3>
                    { grid_merged_1c, grid_merged_1c, grid_merged_1c }, grid_merged_3c);

                //drawing opencv points
                for (auto& point : imggridpoints)
                    cross(grid_merged_3c, point, { 2, 2 }, { 0, 0, 255 });

                //writing opencv result
                filename_opencv = getFilenameParamsSelection(sstream_4_names, path, count,
                    xrange[i], yrange[j], zrange[k], "_merged.png");
                std::cout << filename_opencv << std::endl;
                cv::imwrite(filename_opencv, grid_merged_3c);
                ++count;
            }
        }
    }

    std::cout << "End of program" << std::endl;
}


void paramsSelectionOuterGL(const cv::Mat& src,
    std::vector<cv::Point3d> objpoints, const std::vector<cv::Point2d>& imgpoints,
    const cv::Mat& camera_mat, const cv::Size& quantity, const cv::Point3d& deye,
    const std::vector<double>& reye0, const std::vector<double>& reye1, const std::vector<double>& reye2,
    const std::string& path) {

    if ((reye0.size() != 2) || (reye1.size() != 2) || (reye2.size() != 2))
        throw std::length_error("reye must containt only start and end points");

    //get outer params
    cv::Mat distortion_coeffs, rotation_vec, translation_vec;
    getOuterCameraParams(objpoints, imgpoints, camera_mat,
        distortion_coeffs, rotation_vec, translation_vec);
    cv::Mat rotation_mat;
    cv::Rodrigues(rotation_vec, rotation_mat);

    //set outer params to OpenGL
    main_scene.setRotationMatrix(rotation_mat);
    main_scene.setTranslationVector(translation_vec);

    //set experimental params
    //  points for grid in 3D (aquarium)
    std::vector<cv::Point3d> objgridpoints;
    double width_step = main_scene.getAquariumWidth() / (quantity.width - 1);
    double height_step = main_scene.getAquariumHeight() / (quantity.height - 1);
    std::cout << "\nStep:  w " << width_step << "  h " << height_step << std::endl;
    for (int i{}; i < quantity.width; ++i) {
        for (int j{}; j < quantity.height; ++j) {
            objgridpoints.push_back({
                main_scene.getScalingFactor() * (-0.5 * main_scene.getAquariumWidth() + i * width_step),
                main_scene.getScalingFactor() * (-0.5 * main_scene.getAquariumHeight() + j * height_step), 0 });
        }
    }

    //printing grid values
    std::cout << "Grid horizontal values: \t";
    for (int i{}; i < objgridpoints.size(); i += quantity.height)
        std::cout << objgridpoints[i].x << ", ";

    std::cout << "\nGrid vertical values: \t";
    for (int i{}; i < quantity.height; ++i)
        std::cout << objgridpoints[i].y << ", ";
    std::cout << "\n" << std::endl;

    //  point for grid in 2D (image)
    std::vector<cv::Point2d> imggridpoints;
    getPredictPoints(imggridpoints, objgridpoints, camera_mat, rotation_mat, translation_vec);
    main_scene.setObjGridPoints(objgridpoints);

    //filling array of eye values
    std::vector<double> xrange;
    std::vector<double> yrange;
    std::vector<double> zrange;

    std::cout << "\nX eye range: \t";
    for (double i{ reye0[0] }; i <= reye0[1]; i += deye.x) {
        xrange.push_back(i);
        std::cout << i << ", ";
    }
    std::cout << "\nY eye range: \t";
    for (double j{ reye1[0] }; j <= reye1[1]; j += deye.y) {
        yrange.push_back(j);
        std::cout << j << ", ";
    }
    std::cout << "\nZ eye range: \t";
    for (double k{ reye2[0] }; k <= reye2[1]; k += deye.z) {
        zrange.push_back(k);
        std::cout << k << ", ";
    }
    std::cout << "\n" << std::endl;

    //filling eye data and filenames for glut
    size_t count{ 1 };
    std::string filename_glut;
    std::stringstream sstream_4_names;
    const std::string opengl_suffix = "opengl/";
    const std::string opencv_suffix = "opencv/";
    for (int i{ }; i < xrange.size(); ++i) {
        for (int j{ }; j < yrange.size(); ++j) {
            for (int k{ }; k < zrange.size(); ++k) {

                filename_glut = getFilenameParamsSelection(sstream_4_names, path + opengl_suffix, count,
                    xrange[i], yrange[j], zrange[k], ".png");

                main_scene.addFilenameEyeParamsSelection(std::pair<cv::Point3d, std::string>{
                    { xrange[i], yrange[j], zrange[k] }, filename_glut });

                std::cout << filename_glut << std::endl;
                ++count;
            }
        }
    }
    std::cout << std::endl;

    //glut rendering to files (name: {count}_x{xval}_y{yval}_z{zval}.png)
    //glutInit(&argc, argv);    //initialized in main
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 1024);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Point array");

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutDisplayFunc(displayPointCorrespondenceParamsSelectionOuterGL);
    glutReshapeFunc(reshape);
    main_scene.initializeGLUT();
    main_scene.setFrameCount(0);
    glutMainLoop();

    //merging glut and opencv
    count = 1;
    std::string filename_opencv;
    cv::Mat grid_glut, grid_merged_1c, grid_merged_3c;
    for (int i{ }; i < xrange.size(); ++i) {
        for (int j{ }; j < yrange.size(); ++j) {
            for (int k{ }; k < zrange.size(); ++k) {

                //read glut output
                filename_glut = getFilenameParamsSelection(sstream_4_names, path + opengl_suffix, count,
                    xrange[i], yrange[j], zrange[k], ".png");
                grid_glut = cv::imread(filename_glut, cv::IMREAD_GRAYSCALE);

                //merge glut output with src image
                grid_merged_1c.release();
                cv::bitwise_and(src, grid_glut, grid_merged_1c, grid_glut);

                //making 3c image from previous step
                cv::merge(std::array<cv::Mat, 3>
                { grid_merged_1c, grid_merged_1c, grid_merged_1c }, grid_merged_3c);

                //drawing opencv points
                for (auto& point : imggridpoints)
                    cross(grid_merged_3c, point, { 2, 2 }, { 0, 0, 255 });

                //writing opencv result
                filename_opencv = getFilenameParamsSelection(sstream_4_names, path + opencv_suffix, count,
                    xrange[i], yrange[j], zrange[k], "_merged.png");
                std::cout << filename_opencv << std::endl;
                cv::imwrite(filename_opencv, grid_merged_3c);
                ++count;
            }
        }
    }

    std::cout << "End of program" << std::endl;

}


void testdisplay(std::vector<cv::Point3d> objpoints, const std::vector<cv::Point2d>& imgpoints,
    const cv::Mat& camera_mat) {

    //get outer params
    //cv::Mat distortion_coeffs, rotation_vec, translation_vec;
    //getOuterCameraParams(objpoints, imgpoints, camera_mat,
    //    distortion_coeffs, rotation_vec, translation_vec);
    //cv::Mat rotation_mat;
    //cv::Rodrigues(rotation_vec, rotation_mat);
    //
    ////set outer params to OpenGL
    //main_scene.setRotationMatrix4(rotation_mat);
    //main_scene.setTranslationVector(translation_vec);

    double mat_data[] = {
        0.9848, 0.1736, 0, 0,
        -0.1736, 0.9848, 0, 0,
        0, 0, 1, 0,
        0.5, 0, 0, 1
    };
    
    //set outer params to OpenGL
    main_scene.setRotationMatrix(glm::make_mat4(mat_data));

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 1024);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Point array");

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    main_scene.initializeGLUT();
    glutMainLoop();
}
