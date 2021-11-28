#ifndef DAPHNIA_DAPHNIA_H_20212811
#define DAPHNIA_DAPHNIA_H_20212811

class Daphnia {
public:
    Daphnia() = default;
    ~Daphnia() = default;

    Daphnia(double radius);
    Daphnia(unsigned int x, unsigned int y, unsigned int z, double radius);

    unsigned int getX();
    unsigned int getY();
    unsigned int getZ();
    double getRadius();

    void draw();

private:
    unsigned int x_{ };
    unsigned int y_{ };
    unsigned int z_{ };
    double radius_{ 0.1 };

    unsigned int slices_{ 15 };
    unsigned int stacks_{ 15 };
};

#endif