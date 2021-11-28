#include "glut.h"
#include "daphnia.h"

// aquarium params
const float aquarium_width = 16.5;
const float aquarium_height = 14.5;
const float aquarium_depth = 2.5;

const float aquarium_points[] = {
    -0.5 * aquarium_width, -0.5 * aquarium_height, 0,
     0.5 * aquarium_width, -0.5 * aquarium_height, 0,
     0.5 * aquarium_width,  0.5 * aquarium_height, 0,
    -0.5 * aquarium_width,  0.5 * aquarium_height, 0,
    -0.5 * aquarium_width, -0.5 * aquarium_height, -aquarium_depth,
     0.5 * aquarium_width, -0.5 * aquarium_height, -aquarium_depth,
     0.5 * aquarium_width,  0.5 * aquarium_height, -aquarium_depth,
    -0.5 * aquarium_width,  0.5 * aquarium_height, -aquarium_depth,
};

// daphnia
Daphnia d{ 0.1 };

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_FLAT);
}

void drawAquarium() {
    glPushMatrix();
    // near surface
    glBegin(GL_LINE_LOOP);
    glVertex3f(aquarium_points[0], aquarium_points[1], aquarium_points[2]);
    glVertex3f(aquarium_points[3], aquarium_points[4], aquarium_points[5]);
    glVertex3f(aquarium_points[6], aquarium_points[7], aquarium_points[8]);
    glVertex3f(aquarium_points[9], aquarium_points[10], aquarium_points[11]);
    glEnd();

    // far surface
    glBegin(GL_LINE_LOOP);
    glVertex3f(aquarium_points[12], aquarium_points[13], aquarium_points[14]);
    glVertex3f(aquarium_points[15], aquarium_points[16], aquarium_points[17]);
    glVertex3f(aquarium_points[18], aquarium_points[19], aquarium_points[20]);
    glVertex3f(aquarium_points[21], aquarium_points[22], aquarium_points[23]);
    glEnd();

    // side edges
    glBegin(GL_LINES);
    glVertex3f(aquarium_points[0], aquarium_points[1], aquarium_points[2]);
    glVertex3f(aquarium_points[12], aquarium_points[13], aquarium_points[14]);
    glVertex3f(aquarium_points[3], aquarium_points[4], aquarium_points[5]);
    glVertex3f(aquarium_points[15], aquarium_points[16], aquarium_points[17]);
    glVertex3f(aquarium_points[6], aquarium_points[7], aquarium_points[8]);
    glVertex3f(aquarium_points[18], aquarium_points[19], aquarium_points[20]);
    glVertex3f(aquarium_points[9], aquarium_points[10], aquarium_points[11]);
    glVertex3f(aquarium_points[21], aquarium_points[22], aquarium_points[23]);
    glEnd();
    glPopMatrix();
}

void drawDaphnia(int x, int y, int z) {
    glTranslatef(x, y, z);
    glutSolidSphere(0.1, 15, 15);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    drawAquarium();
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Scene");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
}