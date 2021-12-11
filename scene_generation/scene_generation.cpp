#include <iostream>

#include "daphnia.h"
#include "aquarium.h"
#include "glut.h"


Aquarium a;             // aquarium
Daphnia d{ 0, 0, 1 };   // daphnia

float yaw{ };             //рыскание
float pitch{ };           //тангаж
float roll{ };            //крен

int mod{ };
float z{ };               //расстояние до аквариума
int angle{ 90 };          //угол обзора
int angle_prev{ 90 };

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_FLAT);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        pitch += 1;
        break;
    case 's':
        pitch -= 1;
        break;
    case 'a':
        roll += 1;
        break;
    case 'd':
        roll -= 1;
        break;
    case 'q':
        yaw -= 1;
        break;
    case 'e':
        yaw += 1;
        break;
    default:
        break;
    }
}

void arrows(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        z += 1;
        break;
    case GLUT_KEY_DOWN:
        z -= 1;
        break;
    case GLUT_KEY_LEFT:
        angle -= 1;
        break;
    case GLUT_KEY_RIGHT:
        angle += 1;
        break;
    default:
        break;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0);
    //gluLookAt(0.0, 16.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);

    // drawing content
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glRotatef(pitch, 1, 0, 0);
    glRotatef(roll, 0, 1, 0);
    glRotatef(yaw, 0, 0, 1);
    glTranslatef(0, 0, -z);
    a.draw(true);
    d.draw();
    glPopMatrix();
    
    if (angle != angle_prev) {
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(angle, (GLfloat)viewport[2] / (GLfloat)viewport[3], 1, 50);
        glMatrixMode(GL_MODELVIEW);
        angle_prev = angle;
    }

    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle, (GLfloat)w / (GLfloat)h, 1, 50);
    glMatrixMode(GL_MODELVIEW);
}

void main() {
    d.setRange(a.getWidth() * 0.5, a.getHeight() * 0.5, a.getDepth() * 0.5);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Scene");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrows);
    glutMainLoop();
}