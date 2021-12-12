#include <iostream>

#include "daphnia.h"
#include "aquarium.h"
#include "scene.h"

#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

#include <GL/glut.h>

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

void main(int argc, char** argv) {
    main_scene.generateDaphnias(3);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Scene");

    main_scene.initializeGLUT();
    main_scene.loadTexture("../data/source_image_1.png");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsClassic();
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrow);
    glutMainLoop();

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
}
