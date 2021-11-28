#include <iostream>

#include "aquarium.h"
#include "glut.h"

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

void Aquarium::draw() {
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
}