#ifndef DAPHNIA_DAPHNIA_H_20212811
#define DAPHNIA_DAPHNIA_H_20212811

#include <random>
#include <GL/freeglut.h>

class Daphnia {
public:
    Daphnia() = default;
    Daphnia(const Daphnia&);
    ~Daphnia() = default;

    Daphnia(double radius);
    Daphnia(double x, double y, double z);
    Daphnia(double x, double y, double z, double radius);

    void setRange(double x, double y, double z);

    double getX() const;
    double getY() const;
    double getZ() const;
    double getRadius() const;

    void renewParams(bool print = false);
    void draw();

private:
    double x_     { };
    double y_     { };
    double z_     { };
    double radius_{ 0.02 };

    const unsigned int slices_{ 10 };
    const unsigned int stacks_{ 10 };

    double x_range_{ };
    double y_range_{ };
    double z_range_{ };

    std::random_device random_device_;
};

#endif
