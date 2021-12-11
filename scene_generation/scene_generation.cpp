#include <iostream>

#include "daphnia.h"
#include "aquarium.h"
#include "scene.h"
#include "glut.h"

Scene main_scene;

void display() {
    main_scene.displayContent();
}

void reshape(int width, int heght) {
    main_scene.reshapeContent(width, heght);
}

void keyboard(unsigned char key, int x, int y) {
    main_scene.keyboardControl(key, x, y);
}

void arrow(int key, int x, int y) {
    main_scene.arrowControl(key, x, y);
}

void main() {
    main_scene.generateDaphnias(2);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Scene");
    main_scene.initializeGLUT();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrow);
    glutMainLoop();
}
