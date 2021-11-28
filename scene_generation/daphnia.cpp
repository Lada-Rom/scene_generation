#include <iostream>
#include <vector>
#include <random>

#include "daphnia.h"
#include "glut.h"

std::piecewise_linear_distribution<double> motion_distribution(double min, double peak, double max) {
    std::vector<double> x{ min, -peak, 0, peak, max };
    std::vector<double> y{ 0, 1, 1, 1, 0 };
    return std::piecewise_linear_distribution<double>(x.begin(), x.end(), y.begin());
}

std::piecewise_linear_distribution<double> linear_distribution(double min, double max) {
    std::vector<double> x{ min, max };
    std::vector<double> y{1, 0 };
    return std::piecewise_linear_distribution<double>(x.begin(), x.end(), y.begin());
}

std::random_device rd;
std::mt19937 gen(rd());
auto tri_dist = motion_distribution(-0.05, 0.0015, 0.05);
auto bool_dist = linear_distribution(0, 1);
//std::uniform_real_distribution<double> uni_dist(0, 1);

double dx = 0;
double dy = 0;
double dz = 0;

Daphnia::Daphnia(double radius)
    : radius_(radius) { }

Daphnia::Daphnia(double x, double y, double z)
    : x_(x), y_(y), z_(z) { }

Daphnia::Daphnia(double x, double y, double z, double radius)
    : x_(x), y_(y), z_(z), radius_(radius) { }

void Daphnia::setRange(double x, double y, double z) {
    x_range_ = x;
    y_range_ = y;
    z_range_ = z;
}

double Daphnia::getX() {
    return this->x_;
}

double Daphnia::getY() {
    return this->y_;
}

double Daphnia::getZ() {
    return this->z_;
}

double Daphnia::getRadius() {
    return this->radius_;
}

void Daphnia::renewParams() {

    if (bool_dist(gen))
        dx = tri_dist(gen);
    if (bool_dist(gen))
        dy = tri_dist(gen);
    if (bool_dist(gen))
        dz = tri_dist(gen);

    x_ += dx;
    y_ += dy;
    z_ += dz;

    if (x_ > x_range_ - radius_) {
        std::cout << "x: " << x_;
        x_ = x_range_ - radius_;
        std::cout << " " << x_ << std::endl;
    }
    if (x_ < -x_range_ + radius_) {
        std::cout << "x: " << x_;
        x_ = -x_range_ + radius_;
        std::cout << " " << x_ << std::endl;
    }


    if (y_ > y_range_ - radius_) {
        std::cout << "y: " << y_;
        y_ = y_range_ - radius_;
        std::cout << " " << y_ << std::endl;
    }
    if (y_ < -y_range_ + radius_) {
        std::cout << "y: " << y_;
        y_ = -y_range_ + radius_;
        std::cout << " " << y_ << std::endl;
    }

    if (z_ > 0 - radius_) {
        std::cout << "z: " << z_;
        z_ = 0 - radius_;
        std::cout << " " << z_ << std::endl;
    }
    if (z_ < -2 * z_range_ + radius_) {
        std::cout << "z: " << z_;
        z_ = -2 * z_range_ + radius_;
        std::cout << " " << z_ << std::endl;
    }
}

void Daphnia::draw() {
    // daphnia
    glColor3f(0, 0, 0);
    glPushMatrix();
        renewParams();
        glTranslatef(x_, y_, z_);
        glutSolidSphere(radius_, slices_, stacks_);
    glPopMatrix();

    //// test points
    //glColor3f(1, 0, 0);
    //glPointSize(4);
    //glPushMatrix();
    //    glBegin(GL_POINTS);
    //        glVertex3f(8.25, 0, 0);
    //        glVertex3f(8.25, 0, -1);
    //        glVertex3f(8.25, 0, -2);
    //        glVertex3f(8.25, 0, -2.5);
    //    glEnd();
    //glPopMatrix();
}
