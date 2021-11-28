#include <iostream>

#include "daphnia.h"
#include "aquarium.h"
#include "glut.h"


Aquarium a;             // aquarium
Daphnia d{ 0, 0, 1 };   // daphnia

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_FLAT);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);

    glLoadIdentity();
    //gluLookAt(0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    gluLookAt(0.0, 16.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);

    // drawing content
    a.draw(true);
    d.draw();
    
    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, (GLfloat)w / (GLfloat)h, 1, 50);
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
    glutMainLoop();
}