#ifndef SCENE_SCENE_H_20211112
#define SCENE_SCENE_H_20211112

#include <iostream>
#include <vector>
#include <string>
#include <random>

#include <stb_image.h>
#include <stb_image_write.h>

#include "aquarium.h"
#include "daphnia.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/freeglut.h>

const float pi = 3.14159265;

class Scene {
public:
    Scene() = default;
    ~Scene() = default;

    void initializeGLUT();
    void displayContent();
    void reshapeContent(unsigned int, unsigned);
    void keyboardControl(unsigned char, int, int);
    void arrowControl(int, int, int);

    void generateDaphnias(size_t, float radius = 0.1);

    void loadTexture(const std::string&);
    void saveImage(int, int, std::string&);
    void calcLookAtArray();

private:
    Aquarium aquarium_;
    std::vector<Daphnia> daphnias_;
    bool draw_aquarium_{ true };
    bool draw_texture_{ false };

    double yaw_{ };      //рыскание
    double pitch_{ };    //тангаж
    double roll_{ };     //крен

    double camera_distance_{ };
    double camera_new_angle_{ 2 * 180 / pi * std::atan(5.43 / (2 * 8)) }; //37.49 45.9296
    double camera_prev_angle_{ };
    double camera_initial_z_{ 22 };
    double camera_x_pose_{ };
    double camera_y_pose_{ };
    double z_near_{ 1 };
    double z_far_{ 50 };

    std::random_device random_device_;

    int image_width_{ };
    int image_height_{ };
    int image_comp_{ };
    double texture_height_{ };
    GLuint texture_id_;

    const glm::dmat3 rotation_ = {
        { 0.9997,        -3.69 * 0.001, -0.02544 },
        { -3.49 * 0.001, -1,            7.9 * 0.001},
        { -0.02547,      -7.81 * 0.001, -0.9996}
    };
    const glm::dvec3 translation_ = { -0.1971, -0.07411, 22.001 };
    glm::vec3 eye_      { 0, 0, 9 };
    glm::vec3 center_   { 0, 0, 0 };
    glm::vec3 up_       { 0, 1, 0 };
};

#endif
