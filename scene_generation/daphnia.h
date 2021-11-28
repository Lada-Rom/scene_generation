#ifndef DAPHNIA_DAPHNIA_H_20212811
#define DAPHNIA_DAPHNIA_H_20212811

class Daphnia {
public:
    Daphnia() = default;
    ~Daphnia() = default;

    Daphnia(double radius);
    Daphnia(double x, double y, double z);
    Daphnia(double x, double y, double z, double radius);

    void setRange(double x, double y, double z);

    double getX();
    double getY();
    double getZ();
    double getRadius();

    void renewParams();
    void draw();

private:
    double x_     { };
    double y_     { };
    double z_     { };
    double radius_{ 0.1 };

    const unsigned int slices_{ 15 };
    const unsigned int stacks_{ 15 };

    double x_range_{ };
    double y_range_{ };
    double z_range_{ };
};

#endif
