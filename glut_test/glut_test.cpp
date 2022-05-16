#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#define PI 3.14159265
#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include <iostream>

#include <stb_image.h>
#include <stb_image_write.h>
#include <GL/freeglut.h>

//Параметры текстуры
//GLuint texture;
//int w = 0, h = 0, n = 0;
//float angle = 0;
//
//void init() {
//    glClearColor(1.0, 1.0, 1.0, 0.0);
//    glShadeModel(GL_FLAT);
//    glEnable(GL_DEPTH_TEST);
//
//    unsigned char *data = stbi_load(std::string("../data/src.1.png").c_str(), &w, &h, &n, 3);
//    if (!data)
//        return;
//
//    //create a texture
//    glGenTextures(1, &texture);
//    glBindTexture(GL_TEXTURE_2D, texture);
//    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
//    //texture filtering
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//    free(data);
//}
//
//void display() {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glLoadIdentity();
//    gluLookAt(0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0);
//
//    glEnable(GL_TEXTURE_2D);
//    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//    glBindTexture(GL_TEXTURE_2D, texture);
//
//    int height = std::tan(0.5 * 90 * PI / 180) * (3) + 9 * std::tan(0.5 * 90 * PI / 180);
//    //std::cout << height << std::endl;
//    glPushMatrix();
//        glTranslatef(0.0, 0.0, -2);
//        glBegin(GL_QUADS);
//            glTexCoord2f(0.0, 0.0); glVertex3f(-height * w / h,  height, 0.0);
//            glTexCoord2f(0.0, 1.0); glVertex3f(-height * w / h, -height, 0.0);
//            glTexCoord2f(1.0, 1.0); glVertex3f( height * w / h, -height, 0.0);
//            glTexCoord2f(1.0, 0.0); glVertex3f( height * w / h,  height, 0.0);
//        glEnd();
//    glPopMatrix();
//    glDisable(GL_TEXTURE_2D);
//
//    glutSwapBuffers();
//    glutPostRedisplay();
//}
//
//void reshape(int w, int h) {
//    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 1.0, 11.1);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//}

//void init() {
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//}
//
//void display() {
//    glClear(GL_COLOR_BUFFER_BIT);
//    glClearColor(1, 1, 1, 1);
//
//    gluLookAt(0, 0, 0,
//              0, 0, -1,
//              0, 1, 0);
//
//    static
//        float z = -5;
//    z -= 0.01;
//    glLoadIdentity();
//    glTranslatef(0, 0, z);
//    glRotatef(50, 0, 1, 0);
//    glColor4f(1, 0, 0, 1);
//    glutSolidCube(2);
//
//    glutSwapBuffers();
//    glutPostRedisplay();
//}
//
//void reshape(int w, int h) {
//    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(37, (GLfloat)w / (GLfloat)h, 1.0, 500);
//    glMatrixMode(GL_MODELVIEW);
//}
//
//void main(int argc, char *argv) {
//    glutInit(&argc, &argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//    glutInitWindowSize(800, 600);
//    glutCreateWindow("Texture");
//    init();
//    glutDisplayFunc(display);
//    glutReshapeFunc(reshape);
//    glutMainLoop();
//}

//double mat_data[] = {
//    0.9848, 0.1736, 0, 0,
//    -0.1736, 0.9848, 0, 0,
//    0, 0, 1, 0,
//    0.5, 0, 0, 1
//};
//
//void init() {
//    glClearColor(1., 1., 1., 0.);
//    glShadeModel(GL_FLAT);
//}
//
//void display() {
//    GLint viewport[4];
//    glGetIntegerv(GL_VIEWPORT, viewport);
//    glClear(GL_COLOR_BUFFER_BIT);
//    glClearColor(1., 1., 1., 0.);
//
//    glLoadIdentity();
//    gluLookAt(0, 0, 0,
//        0, 0, -1,
//        0, 1, 0);
//
//    glLoadMatrixd(&mat_data[0]);
//    //glTranslatef(-0.5, 0, 0);
//
//    glColor3f(0, 0, 0);
//    glPushMatrix();
//        glTranslatef(-1, -1, -5);
//        glutSolidSphere(0.1, 10, 10);
//    glPopMatrix();
//
//    glPushMatrix();
//        glTranslatef(1, -1, -5);
//        glutSolidSphere(0.1, 10, 10);
//    glPopMatrix();
//
//    glPushMatrix();
//        glTranslatef(-1, 1, -5);
//        glutSolidSphere(0.1, 10, 10);
//    glPopMatrix();
//
//    glPushMatrix();
//        glTranslatef(1, 1, -5);
//        glutSolidSphere(0.1, 10, 10);
//    glPopMatrix();
//
//    glutSwapBuffers();
//    glutPostRedisplay();
//}
//
//void reshape(int w, int h) {
//    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(37, (GLfloat)w / (GLfloat)h, 1.0, 500);
//    glMatrixMode(GL_MODELVIEW);
//}
//
//void main(int argc, char** argv) {
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//    glutInitWindowSize(1280, 1024);
//    glutInitWindowPosition(0, 0);
//    glutCreateWindow("Point array");
//
//    glutDisplayFunc(display);
//    glutReshapeFunc(reshape);
//    glutMainLoop();
//}


void init() {
    glClearColor(1., 1., 1., 0.);
    glShadeModel(GL_FLAT);
}

void display() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1., 1., 1., 0.);

    glLoadIdentity();
    gluLookAt(0, 0, 0,
        0, 0, -1,
        0, 1, 0);

    glColor3f(0, 0, 0);
    glPushMatrix();
        glTranslatef(-0.5, 0, -5);
        glRotatef(30, 1, 0, 0);
        glRotatef(30, 0, 1, 0);
        //glRotatef(30, 0, 0, 1);

        glPushMatrix();
            glScalef(0.5, 0.5, 0.5);
            glScalef(1.5, 1, 1);
            glutSolidSphere(0.1, 30, 30);
        glPopMatrix();
    glPopMatrix();

    glColor3f(0, 0, 0);
    glPushMatrix();
        glTranslatef(0.5, 0, -5);
        glRotatef(30, 1, 0, 0);
        //glRotatef(30, 0, 1, 0);
        glRotatef(30, 0, 0, 1);

        glPushMatrix();
            glScalef(0.5, 0.5, 0.5);
            glScalef(1.5, 1, 1);
            glutSolidSphere(0.1, 30, 30);
        glPopMatrix();
    glPopMatrix();

    glColor3f(0, 0, 0);
    glPushMatrix();
        glTranslatef(0, 0.5, -5);
        glRotatef(30, 1, 0, 0);
        glRotatef(30, 0, 1, 0);
        glRotatef(30, 0, 0, 1);

        glPushMatrix();
            glScalef(0.5, 0.5, 0.5);
            glScalef(1.5, 1, 1);
            glutSolidSphere(0.1, 30, 30);
        glPopMatrix();
    glPopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(37, (GLfloat)w / (GLfloat)h, 1.0, 500);
    glMatrixMode(GL_MODELVIEW);
}

void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 1024);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Point array");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
}
