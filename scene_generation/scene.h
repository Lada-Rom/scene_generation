#ifndef SCENE_SCENE_H_20211112
#define SCENE_SCENE_H_20211112

#include <vector>
#include <random>

#include "aquarium.h"
#include "daphnia.h"

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

private:
    Aquarium aquarium_;
    std::vector<Daphnia> daphnias_;

    float yaw_{ };      //рыскание
    float pitch_{ };    //тангаж
    float roll_{ };     //крен

    float camera_distance_{ };
    float camera_new_angle_{ 90 };
    float camera_prev_angle_{ 90 };

    std::random_device random_device_;
    //std::mt19937 generator_;

};

#endif
