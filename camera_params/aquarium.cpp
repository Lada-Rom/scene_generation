#include <iostream>

#include "aquarium.h"

Aquarium::Aquarium() {
    recalcVertices();
}

double Aquarium::getWidth() {
    return width_;
}

double Aquarium::getHeight() {
    return height_;
}

double Aquarium::getDepth() {
    return depth_;
}

std::vector<double> Aquarium::getVerticies() {
    return vertices_;
}

void Aquarium::setWidth(const double val) {
    width_ = val;
}

void Aquarium::setHeight(const double val) {
    height_ = val;
}

void Aquarium::setDepth(const double val) {
    depth_ = val;
}

void Aquarium::addWidthHeightDepth(
    const double dw, const double dh, const double dd, bool recalc) {
    width_ += 2 * dw;
    height_ += 2 * dh;
    depth_ += dd;

    if (recalc)
        recalcVertices();
}

void Aquarium::makeWidthHeightDepthDeafault() {
    width_ = default_width_;
    height_ = default_height_;
    depth_ = default_depth_;
}

void Aquarium::recalcVertices() {
    vertices_ = {
        -0.5 * width_, -0.5 * height_, 0,
         0.5 * width_, -0.5 * height_, 0,
         0.5 * width_,  0.5 * height_, 0,
        -0.5 * width_,  0.5 * height_, 0,
        -0.5 * width_, -0.5 * height_, -depth_,
         0.5 * width_, -0.5 * height_, -depth_,
         0.5 * width_,  0.5 * height_, -depth_,
        -0.5 * width_,  0.5 * height_, -depth_,
    };

    //vertices_ = {
    //-width_, -height_, 0,
    // width_, -height_, 0,
    // width_,  height_, 0,
    //-width_,  height_, 0,
    //-width_, -height_, -depth_,
    // width_, -height_, -depth_,
    // width_,  height_, -depth_,
    //-width_,  height_, -depth_,
    //};
}

void Aquarium::draw(bool draw_axes, bool center_axes) {
    // aquarium
    glPushMatrix();
    // near surface
    glBegin(GL_LINE_LOOP);
    glVertex3d(vertices_[0], vertices_[1],  vertices_[2]);
    glVertex3d(vertices_[3], vertices_[4],  vertices_[5]);
    glVertex3d(vertices_[6], vertices_[7],  vertices_[8]);
    glVertex3d(vertices_[9], vertices_[10], vertices_[11]);
    glEnd();

    // far surface
    glBegin(GL_LINE_LOOP);
    glVertex3d(vertices_[12], vertices_[13], vertices_[14]);
    glVertex3d(vertices_[15], vertices_[16], vertices_[17]);
    glVertex3d(vertices_[18], vertices_[19], vertices_[20]);
    glVertex3d(vertices_[21], vertices_[22], vertices_[23]);
    glEnd();

    // side edges
    glBegin(GL_LINES);
    glVertex3d(vertices_[0],  vertices_[1],  vertices_[2]);
    glVertex3d(vertices_[12], vertices_[13], vertices_[14]);

    glVertex3d(vertices_[3],  vertices_[4],  vertices_[5]);
    glVertex3d(vertices_[15], vertices_[16], vertices_[17]);

    glVertex3d(vertices_[6],  vertices_[7],  vertices_[8]);
    glVertex3d(vertices_[18], vertices_[19], vertices_[20]);

    glVertex3d(vertices_[9],  vertices_[10], vertices_[11]);
    glVertex3d(vertices_[21], vertices_[22], vertices_[23]);
    glEnd();
    glPopMatrix();

    // axes
    if (draw_axes) {
        if (center_axes) {
            glPushMatrix();
            glBegin(GL_LINES);
            glColor3d(1, 0, 0); //x-axis
            glVertex3d(0, 0, 0);
            glVertex3d(2, 0, 0);

            glColor3d(0, 1, 0); //y-axis
            glVertex3d(0, 0, 0);
            glVertex3d(0, 2, 0);

            glColor3d(0, 0, 1); //z-axis
            glVertex3d(0, 0, 0);
            glVertex3d(0, 0, 2);
            glEnd();
            glPopMatrix();
        }
        else {
            glPushMatrix();
            glBegin(GL_LINES);
            glColor3d(1, 0, 0); //x-axis
            glVertex3d(-0.5 * width_ - 0.5, -0.5 * height_ - 0.5, -depth_);
            glVertex3d(-0.5 * width_ - 0.5 + 2, -0.5 * height_ - 0.5, -depth_);

            glColor3d(0, 1, 0); //y-axis
            glVertex3d(-0.5 * width_ - 0.5, -0.5 * height_ - 0.5, -depth_);
            glVertex3d(-0.5 * width_ - 0.5, -0.5 * height_ - 0.5 + 2, -depth_);

            glColor3d(0, 0, 1); //z-axis
            glVertex3d(-0.5 * width_ - 0.5, -0.5 * height_ - 0.5, -depth_);
            glVertex3d(-0.5 * width_ - 0.5, -0.5 * height_ - 0.5, -depth_ + 2);
            glEnd();
            glPopMatrix();
        }
    }
}