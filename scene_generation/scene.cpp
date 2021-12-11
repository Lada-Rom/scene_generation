#include <random>
#include <iostream>

#include "scene.h"
#include "glut.h"

void Scene::initializeGLUT() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_FLAT);
}

void Scene::displayContent() {
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0);

    glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glRotatef(pitch_, 1, 0, 0);
        glRotatef(roll_, 0, 1, 0);
        glRotatef(yaw_, 0, 0, 1);
        glTranslatef(0, 0, -camera_distance_);

        aquarium_.draw(true, true);
        for (Daphnia& daphnia : daphnias_) {
            daphnia.draw();
        }
    glPopMatrix();

    if (camera_new_angle_ != camera_prev_angle_) {
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(camera_new_angle_, (GLfloat)viewport[2] / (GLfloat)viewport[3], 1, 50);
        glMatrixMode(GL_MODELVIEW);
        camera_prev_angle_ = camera_new_angle_;
    }

    glutSwapBuffers();
    glutPostRedisplay();
}

void Scene::reshapeContent(unsigned int width, unsigned height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera_new_angle_, (GLfloat)width / (GLfloat)height, 1, 50);
    glMatrixMode(GL_MODELVIEW);
}

void Scene::keyboardControl(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        pitch_ += 1;
        break;
    case 's':
        pitch_ -= 1;
        break;
    case 'a':
        roll_ += 1;
        break;
    case 'd':
        roll_ -= 1;
        break;
    case 'q':
        yaw_ -= 1;
        break;
    case 'e':
        yaw_ += 1;
        break;
    default:
        break;
    }
}

void Scene::arrowControl(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        camera_distance_ += 1;
        break;
    case GLUT_KEY_DOWN:
        camera_distance_ -= 1;
        break;
    case GLUT_KEY_LEFT:
        camera_new_angle_ -= 1;
        break;
    case GLUT_KEY_RIGHT:
        camera_new_angle_ += 1;
        break;
    default:
        break;
    }
}

void Scene::generateDaphnias(size_t quantity, float radius) {
    float width = aquarium_.getWidth();
    float height = aquarium_.getHeight();
    float depth = aquarium_.getDepth();

    std::uniform_real_distribution<> x_distribution(-0.5 * width + radius, 0.5 * width - radius);
    std::uniform_real_distribution<> y_distribution(-0.5 * height + radius, 0.5 * height - radius);
    std::uniform_real_distribution<> z_distribution(-2 * depth + radius, 0 - radius);
    std::cout << width << " " << height << " " << depth << std::endl;

    float x;
    float y;
    float z;
    for (size_t i{ }; i < quantity; ++i) {
        x = x_distribution(random_device_);
        y = y_distribution(random_device_);
        z = z_distribution(random_device_);
        std::cout << x << " " << y << " " << z << std::endl;
        Daphnia new_daphnia{ x, y, z, radius };
        //new_daphnia.setRandomNumberGenerator();
        new_daphnia.setRange(0.5 * width, 0.5 * height, 0.5 * depth);
        daphnias_.push_back(new_daphnia);
    }
}
