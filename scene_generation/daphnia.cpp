#include "daphnia.h"
#include "glut.h"

Daphnia::Daphnia(double radius)
    : radius_(radius) { }

Daphnia::Daphnia(unsigned int x, unsigned int y, unsigned int z, double radius)
    : x_(x), y_(y), z_(z), radius_(radius) { }

unsigned int Daphnia::getX() {
    return this->x_;
}

unsigned int Daphnia::getY() {
    return this->y_;
}

unsigned int Daphnia::getZ() {
    return this->z_;
}

double Daphnia::getRadius() {
    return this->radius_;
}

void Daphnia::draw() {
    glutSolidSphere(this->radius_, this->slices_, this->stacks_);
}
