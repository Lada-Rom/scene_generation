#ifndef AQUARIUM_AQUARIUM_H_20212811
#define AQUARIUM_AQUARIUM_H_20212811

#include <vector>

class Aquarium {
public:
    Aquarium();
    ~Aquarium() = default;

    double getWidth();
    double getHeight();
    double getDepth();

    void draw(bool draw_axes = false, bool center_axes = true);

private:
    const double width_    { 16.5 };
    const double height_   { 14.5 };
    const double depth_    { 2.5 };

    std::vector<double> vertices_;

};

#endif
