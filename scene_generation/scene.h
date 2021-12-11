#ifndef SCENE_SCENE_H_20211112
#define SCENE_SCENE_H_20211112

#include <vector>
#include <random>

#include "aquarium.h"
#include "daphnia.h"
#include "glut.h"

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

private:
    Aquarium aquarium_;
    std::vector<Daphnia> daphnias_;

    float yaw_{ };      //рыскание
    float pitch_{ };    //тангаж
    float roll_{ };     //крен

    float camera_distance_{ };
    float camera_new_angle_{ 75 };
    float camera_prev_angle_{ };
    float camera_initial_z_{ 9 };
    float z_near_{ 1 };
    float z_far_{ 50 };

    std::random_device random_device_;

    int image_width_{ };
    int image_height_{ };
    int image_comp_{ };
    float texture_height_{ };
    GLuint texture_id_;
};

#endif
