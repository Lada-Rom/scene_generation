#ifndef SCENE_SCENE_H_20211112
#define SCENE_SCENE_H_20211112

#include <vector>
#include <string>
#include <random>

#include "aquarium.h"

#include <stb_image.h>
#include <stb_image_write.h>
#include <opencv2/opencv.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <GL/freeglut.h>

const float pi = 3.14159265;

class Scene {
public:
    Scene() = default;
    ~Scene() = default;

    void initializeGLUT(GLfloat red = 1, GLfloat green = 1, GLfloat blue = 1);
    void displayPointCorrespondence();
    void displayPointCorrespondencev2();
    void displayPointCorrespondenceParamsSelectionInnerGL();
    void displayPointCorrespondenceParamsSelectionOuterGL();
    void display();
    void reshapeContent(unsigned int, unsigned);

    void setFrameCount(const size_t& count = 0);

    void setFilenameParamsSelection(const std::string&);
    void addFilenameEyeParamsSelection(std::pair<cv::Point3d, std::string>);

    void setRotationMatrix(const cv::Mat&);
    void setRotationMatrix4(const cv::Mat&, bool transpose = false, bool inverse = false);
    void setRotationMatrix(double*);
    void setRotationMatrix(const glm::mat4&);
    void setTranslationVector(const cv::Mat&);
    void setObjGridPoints(const std::vector<cv::Point3d>&);

    double getScalingFactor();

    double getAquariumWidth();
    double getAquariumHeight();
    double getAquariumDepth();
    std::vector<double> getAquariumVerticies();

    void addWHDToAquarium(const double, const double, const double, bool recalc = true);

    void saveImage(int, int, std::string&);
    void calcLookAtArray();
    void calcLookAtArray(const double, const double, const double);
    void calcLookAtArray(const cv::Point3d&);

private:
    Aquarium aquarium_;
    bool draw_aquarium_{ true };
    double scaling_factor_{ 0.9 };

    std::string filename_params_selection_{ };
    std::vector<std::pair<cv::Point3d, std::string>> filename_eye_params_selection;
    size_t frame_count{ };

    double camera_distance_{ };
    double camera_angle_{ 2 * 180 / pi * std::atan(5.43 / (2 * 8)) }; //37.49
    //double camera_angle_{ 2 * 180 / pi * std::atan(6.78 / (2 * 8)) }; //45.9296
    //double camera_angle_{ 35 };
    double camera_x_pose_{ };
    double camera_y_pose_{ };
    double camera_z_pose_{ };
    double z_near_{ 1 };
    double z_far_{ 1000 };

    glm::dmat3 rotation_ = {
        { 0.9997,        -3.69 * 0.001, -0.02544 },
        { -3.49 * 0.001, -1,            7.9 * 0.001},
        { -0.02547,      -7.81 * 0.001, -0.9996}
    };
    glm::dmat4 rotation_4_{ };
    double *rotation_data_;

    glm::dvec3 translation_ = { -0.1971, -0.07411, 22.001 };

    glm::dvec3 eye_{ 0, 0, 0 };
    glm::dvec3 center_{ 0, 0, 0 };
    glm::dvec3 up_{ 0, 1, 0 };

    std::vector<glm::dvec3> objgridpoints;
    //std::vector<cv::Point2d> imggridpoints;
};

#endif
