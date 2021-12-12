#include <iostream>

#include "aquarium.h"
#include <GL/glut.h>

Aquarium::Aquarium() {
    this->vertices_ = {
        -0.5 * width_, -0.5 * height_, 0,
         0.5 * width_, -0.5 * height_, 0,
         0.5 * width_,  0.5 * height_, 0,
        -0.5 * width_,  0.5 * height_, 0,
        -0.5 * width_, -0.5 * height_, -depth_,
         0.5 * width_, -0.5 * height_, -depth_,
         0.5 * width_,  0.5 * height_, -depth_,
        -0.5 * width_,  0.5 * height_, -depth_,
    };
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

void Aquarium::draw(bool draw_axes, bool center_axes) {
    // aquarium
    glPushMatrix();
        // near surface
        glBegin(GL_LINE_LOOP);
            glVertex3f(this->vertices_[0], this->vertices_[1], this->vertices_[2]);
            glVertex3f(this->vertices_[3], this->vertices_[4], this->vertices_[5]);
            glVertex3f(this->vertices_[6], this->vertices_[7], this->vertices_[8]);
            glVertex3f(this->vertices_[9], this->vertices_[10], this->vertices_[11]);
        glEnd();

        // far surface
        glBegin(GL_LINE_LOOP);
            glVertex3f(this->vertices_[12], this->vertices_[13], this->vertices_[14]);
            glVertex3f(this->vertices_[15], this->vertices_[16], this->vertices_[17]);
            glVertex3f(this->vertices_[18], this->vertices_[19], this->vertices_[20]);
            glVertex3f(this->vertices_[21], this->vertices_[22], this->vertices_[23]);
        glEnd();

        // side edges
        glBegin(GL_LINES);
            glVertex3f(this->vertices_[0],  this->vertices_[1],  this->vertices_[2]);
            glVertex3f(this->vertices_[12], this->vertices_[13], this->vertices_[14]);

            glVertex3f(this->vertices_[3],  this->vertices_[4],  this->vertices_[5]);
            glVertex3f(this->vertices_[15], this->vertices_[16], this->vertices_[17]);

            glVertex3f(this->vertices_[6],  this->vertices_[7],  this->vertices_[8]);
            glVertex3f(this->vertices_[18], this->vertices_[19], this->vertices_[20]);

            glVertex3f(this->vertices_[9],  this->vertices_[10], this->vertices_[11]);
            glVertex3f(this->vertices_[21], this->vertices_[22], this->vertices_[23]);
        glEnd();
    glPopMatrix();

    // axes
    if (draw_axes) {
        if (center_axes) {
            glPushMatrix();
            glBegin(GL_LINES);
            glColor3f(1, 0, 0); //x-axis
            glVertex3f(0, 0, 0);
            glVertex3f(2, 0, 0);

            glColor3f(0, 1, 0); //y-axis
            glVertex3f(0, 0, 0);
            glVertex3f(0, 2, 0);

            glColor3f(0, 0, 1); //z-axis
            glVertex3f(0, 0, 0);
            glVertex3f(0, 0, 2);
            glEnd();
            glPopMatrix();
        }
        else {
            glPushMatrix();
                glBegin(GL_LINES);
                    glColor3f(1, 0, 0); //x-axis
                    glVertex3f(-0.5 * width_ - 0.5, -0.5 * height_ - 0.5, -depth_);
                    glVertex3f(-0.5 * width_ - 0.5 + 2, -0.5 * height_ - 0.5, -depth_);

                    glColor3f(0, 1, 0); //y-axis
                    glVertex3f(-0.5 * width_ - 0.5, -0.5 * height_ - 0.5, -depth_);
                    glVertex3f(-0.5 * width_ - 0.5, -0.5 * height_ - 0.5 + 2, -depth_);

                    glColor3f(0, 0, 1); //z-axis
                    glVertex3f(-0.5 * width_ - 0.5, -0.5 * height_ - 0.5, -depth_);
                    glVertex3f(-0.5 * width_ - 0.5, -0.5 * height_ - 0.5, -depth_ + 2);
                glEnd();
            glPopMatrix();
        }
    }
}