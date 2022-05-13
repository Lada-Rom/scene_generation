#ifndef AQUARIUM_AQUARIUM_H_20212811
#define AQUARIUM_AQUARIUM_H_20212811

#include <vector>
#include <GL/freeglut.h>

class Aquarium {
public:
    Aquarium();
    ~Aquarium() = default;

    double getWidth();
    double getHeight();
    double getDepth();
    std::vector<double> getVerticies();

    void setWidth(const double);
    void setHeight(const double);
    void setDepth(const double);

    void addWidthHeightDepth(
        const double, const double, const double, bool recalc = true);
    void makeWidthHeightDepthDeafault();

    void recalcVertices();

    void draw(bool draw_axes = false, bool center_axes = true);

private:
    double width_{ 16.5 };
    double height_{ 14.5 };
    double depth_{ 2.5 };

    const double default_width_{ 16.5 };
    const double default_height_{ 14.5 };
    const double default_depth_{ 2.5 };

    std::vector<double> vertices_;

};

#endif
