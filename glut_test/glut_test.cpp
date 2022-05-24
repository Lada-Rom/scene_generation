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


//double mat_data[] = {
//    0.9848, 0.1736, 0, 0,
//    -0.1736, 0.9848, 0, 0,
//    0, 0, 1, 0,
//    0, 0, 0, 1
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
//
//    glColor3f(0, 0, 0);
//    glPushMatrix();
//        glTranslatef(-0.5, 0, -5);
//        glRotatef(30, 1, 0, 0);
//        glRotatef(30, 0, 1, 0);
//        //glRotatef(30, 0, 0, 1);
//
//        glPushMatrix();
//            glScalef(0.5, 0.5, 0.5);
//            glScalef(1.5, 1, 1);
//            glutSolidSphere(0.1, 30, 30);
//        glPopMatrix();
//    glPopMatrix();
//
//    glColor3f(0, 0, 0);
//    glPushMatrix();
//        glTranslatef(0.5, 0, -5);
//        glRotatef(30, 1, 0, 0);
//        //glRotatef(30, 0, 1, 0);
//        glRotatef(30, 0, 0, 1);
//
//        glPushMatrix();
//            glScalef(0.5, 0.5, 0.5);
//            glScalef(1.5, 1, 1);
//            glutSolidSphere(0.1, 30, 30);
//        glPopMatrix();
//    glPopMatrix();
//
//    glColor3f(0, 0, 0);
//    glPushMatrix();
//        glTranslatef(0, 0.5, -5);
//        glRotatef(30, 1, 0, 0);
//        glRotatef(30, 0, 1, 0);
//        glRotatef(30, 0, 0, 1);
//
//        glPushMatrix();
//            glScalef(0.5, 0.5, 0.5);
//            glScalef(1.5, 1, 1);
//            glutSolidSphere(0.1, 30, 30);
//        glPopMatrix();
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


//int image_width_{ };
//int image_height_{ };
//int image_comp_{ };
//
//GLuint tex;
//GLUquadric* sphere;
//
//void loadTexture(std::string filename) {
//    unsigned char* data = stbi_load(filename.c_str(), &image_width_, &image_height_, &image_comp_, 3);
//    if (!data)
//        return;
//
//    glGenTextures(1, &tex);
//    glBindTexture(GL_TEXTURE_2D, tex);
//    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image_width_, image_height_, GL_RGB, GL_UNSIGNED_BYTE, data);
//    
//    //texture filtering
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//    free(data);
//}
//
//void draw(void) {
//    glClearColor(0.5, 0.5, 1.0, 1);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glTranslatef(0.0, 0.0, -8.0);
//    //glRotatef(45, 0, 1, 0);
//
//    gluQuadricDrawStyle(sphere, GLU_FILL);
//    glBindTexture(GL_TEXTURE_2D, tex);
//    gluQuadricTexture(sphere, GL_TRUE);
//    gluQuadricNormals(sphere, GLU_SMOOTH);
//    gluSphere(sphere, 1.0, 32, 32);
//
//    glutSwapBuffers();
//}
//
//void resize(int w, int h) {
//    if (!h)
//        h = 1;
//    glViewport(0, 0, w, h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(37, 1.0 * w / h, 0.1, 100.0);
//}
//
//void init(void) {
//    glEnable(GL_DEPTH_TEST);
//    loadTexture("../../data/src/test_texture.png");
//    sphere = gluNewQuadric();
//    glEnable(GL_TEXTURE_2D);
//}
//
//int main(int argc, char** argv) {
//    glutInit(&argc, argv);
//
//    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
//    glutInitWindowSize(640, 512);
//    glutCreateWindow("Test");
//    glutDisplayFunc(draw);
//    glutReshapeFunc(resize);
//    init();
//    glutMainLoop();
//}


//http://www.codeincodeblock.com/2011/11/drawing-reflection-capture-extra.html
//const float LIGHT_POS = 7.0f; //The length of each side of the cube
//const float BOX_HEIGHT = LIGHT_POS; //The height of the box off of the ground
//const float FLOOR_SIZE = 20.0f; //The length of each side of the floor
//
////Draws the cube
//void drawCube(float angle) {
//
//	glPushMatrix();
//	glRotatef(-angle, 1.0f, 1.0f, 0.0f);
//
//	glBegin(GL_QUADS);
//	//Cross back
//	glColor4f(0.0f, 1.0f, 0.0f, 0.9f);
//	glVertex3f(-0.5, 2.5, -0.5);
//	glVertex3f(-0.5, -3.0, -0.5);
//	glVertex3f(0.5, -3.0, -0.5);
//	glVertex3f(0.5, 2.5, -0.5);
//
//	glVertex3f(-2.0, 1.0, -0.5);
//	glVertex3f(-2.0, 0.0, -0.5);
//	glVertex3f(2.0, 0.0, -0.5);
//	glVertex3f(2.0, 1.0, -0.5);
//
//
//	//Cross front
//	glColor4f(1.0f, 1.0f, 0.0f, 0.9f);
//	glVertex3f(-0.5, 2.5, 0.5);
//	glVertex3f(-0.5, -3.0, 0.5);
//	glVertex3f(0.5, -3.0, 0.5);
//	glVertex3f(0.5, 2.5, 0.5);
//
//	glVertex3f(-2.0, 1.0, 0.5);
//	glVertex3f(-2.0, 0.0, 0.5);
//	glVertex3f(2.0, 0.0, 0.5);
//	glVertex3f(2.0, 1.0, 0.5);
//
//	//Cross top
//	glColor3f(1.0, 0.0, 0.0);
//	glVertex3f(0.5, 2.5, 0.5);
//	glVertex3f(-0.5, 2.5, 0.5);
//	glVertex3f(-0.5, 2.5, -0.5);
//	glVertex3f(0.5, 2.5, -0.5);
//	//Cross left top side
//	glColor3f(0.0, 0.0, 1.0);
//	glVertex3f(-0.5, 2.5, 0.5);
//	glVertex3f(-0.5, 2.5, -0.5);
//	glVertex3f(-0.5, 1.0, -0.5);
//	glVertex3f(-0.5, 1.0, 0.5);
//	//Cross left side  top
//
//	glVertex3f(-0.5, 1.0, 0.5);
//	glVertex3f(-0.5, 1.0, -0.5);
//	glVertex3f(-2.0, 1.0, -0.5);
//	glVertex3f(-2.0, 1.0, 0.5);
//
//	//Cross left side  end
//	glColor3f(1.0, 0.0, 0.0);
//	glVertex3f(-2.0, 1.0, 0.5);
//	glVertex3f(-2.0, 1.0, -0.5);
//	glVertex3f(-2.0, 0.0, -0.5);
//	glVertex3f(-2.0, 0.0, 0.5);
//
//	//Cross left side bottom
//	glColor3f(0.0, 0.0, 1.0);
//	glVertex3f(-2.0, 0.0, 0.5);
//	glVertex3f(-0.5, 0.0, 0.5);
//	glVertex3f(-0.5, 0.0, -0.5);
//	glVertex3f(-2.0, 0.0, -0.5);
//	//Cross left bottom side
//	glVertex3f(-0.5, 0.0, 0.5);
//	glVertex3f(-0.5, 0.0, -0.5);
//	glVertex3f(-0.5, -3.0, -0.5);
//	glVertex3f(-0.5, -3.0, 0.5);
//	//Cross  bottom
//	glColor3f(1.0, 0.0, 0.0);
//	glVertex3f(0.5, -3.0, 0.5);
//	glVertex3f(0.5, -3.0, -0.5);
//	glVertex3f(-0.5, -3.0, -0.5);
//	glVertex3f(-0.5, -3.0, 0.5);
//
//	//Cross right bottom side
//	glColor3f(0.0, 0.0, 1.0);
//	glVertex3f(0.5, 0.0, 0.5);
//	glVertex3f(0.5, -3.0, 0.5);
//	glVertex3f(0.5, -3.0, -0.5);
//	glVertex3f(0.5, 0.0, -0.5);
//
//	//Cross right side bottom
//	glVertex3f(0.5, 0.0, 0.5);
//	glVertex3f(0.5, 0.0, -0.5);
//	glVertex3f(2.0, 0.0, -0.5);
//	glVertex3f(2.0, 0.0, 0.5);
//	//Cross right side end
//	glColor3f(1.0, 0.0, 0.0);
//	glVertex3f(2.0, 1.0, 0.5);
//	glVertex3f(2.0, 0.0, 0.5);
//	glVertex3f(2.0, 0.0, -0.5);
//	glVertex3f(2.0, 1.0, -0.5);
//
//	//Cross right side top
//	glColor3f(0.0, 0.0, 1.0);
//	glVertex3f(0.5, 1.0, 0.5);
//	glVertex3f(2.0, 1.0, 0.5);
//	glVertex3f(2.0, 1.0, -0.5);
//	glVertex3f(0.5, 1.0, -0.5);
//	//Cross right side top
//	glVertex3f(0.5, 1.0, 0.5);
//	glVertex3f(0.5, 1.0, -0.5);
//	glVertex3f(0.5, 2.5, -0.5);
//	glVertex3f(0.5, 2.5, 0.5);
//	glEnd();
//	glPopMatrix();
//}
//
////Draws the floor
//void drawFloor() {
//	glBegin(GL_QUADS);
//	glNormal3f(0, 1, 0);
//	glVertex3f(-FLOOR_SIZE / 2, 0, FLOOR_SIZE / 2);
//	glVertex3f(-FLOOR_SIZE / 2, 0, -FLOOR_SIZE / 2);
//	glVertex3f(FLOOR_SIZE / 2, 0, -FLOOR_SIZE / 2);
//	glVertex3f(FLOOR_SIZE / 2, 0, FLOOR_SIZE / 2);
//	glEnd();
//}
//
//float _angle = 0;
//
//void initRendering() {
//	glClearColor(1.0, 1.0, 1.0, 1.0);
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_NORMALIZE);
//	glEnable(GL_COLOR_MATERIAL);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//}
//
//void handleResize(int w, int h) {
//	glViewport(0, 0, w, h);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
//}
//
//void drawScene() {
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//	glTranslatef(0.0f, 0.0f, -40.0f);
//	glRotatef(30, 1, 0, 0);
//
//	GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
//
//	GLfloat lightColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
//	GLfloat lightPos[] = { -2 * LIGHT_POS , LIGHT_POS , 4 * LIGHT_POS , 1.0f };
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
//	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
//
//	glPushMatrix();
//	glTranslatef(0, BOX_HEIGHT, 0);
//	drawCube(_angle);
//	glPopMatrix();
//
//	glEnable(GL_STENCIL_TEST); //Enable using the stencil buffer
//	glColorMask(0, 0, 0, 0); //Disable drawing colors to the screen
//	glDisable(GL_DEPTH_TEST); //Disable depth testing
//	glStencilFunc(GL_ALWAYS, 1, 1); //Make the stencil test always pass
//	//Make pixels in the stencil buffer be set to 1 when the stencil test passes
//	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//	//Set all of the pixels covered by the floor to be 1 in the stencil buffer
//	drawFloor();
//
//	glColorMask(1, 1, 1, 1); //Enable drawing colors to the screen
//	glEnable(GL_DEPTH_TEST); //Enable depth testing
//	//Make the stencil test pass only when the pixel is 1 in the stencil buffer
//	glStencilFunc(GL_EQUAL, 1, 1);
//	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Make the stencil buffer not change
//
//	//Draw the cube, reflected vertically, at all pixels where the stencil
//	//buffer is 1
//	glPushMatrix();
//	glScalef(1, -1, 1);
//	glTranslatef(0, BOX_HEIGHT, 0);
//	drawCube(_angle);
//	glPopMatrix();
//
//	glDisable(GL_STENCIL_TEST); //Disable using the stencil buffer
//
//	//Blend the floor onto the screen
//	glEnable(GL_BLEND);
//	glColor4f(0, 0, 0.5, 0.5f);
//	drawFloor();
//	glDisable(GL_BLEND);
//
//	glutSwapBuffers();
//}
//
//void update(int value) {
//	_angle += 1.0f;
//	if (_angle > 360) {
//		_angle -= 360;
//	}
//
//	glutPostRedisplay();
//	glutTimerFunc(25, update, 0);
//}
//
//int main(int argc, char** argv) {
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
//	glutInitWindowSize(600, 600);
//
//	glutCreateWindow("www.codeincodeblock.blogspot.com");
//	initRendering();
//
//	glutDisplayFunc(drawScene);
//	glutReshapeFunc(handleResize);
//	glutTimerFunc(25, update, 0);
//
//	glutMainLoop();
//	return 0;
//}


//void initRendering() {
//	glClearColor(1.0, 1.0, 1.0, 1.0);
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_NORMALIZE);
//}
//
//void drawScene() {
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//	glTranslatef(0.0f, 0.0f, -20.0f);
//
//	GLfloat no_mat[] = { 0.0,0.0,0.0,1.0 };
//	GLfloat mat_ambient[] = { 0.7,0.7,0.7,1.0 };
//	GLfloat mat_ambient_color[] = { 0.8,0.8,0.2,1.0 };
//	GLfloat mat_diffuse[] = { 0.1,0.5,0.8,1.0 };
//	GLfloat mat_specular[] = { 1.0,1.0,1.0,1.0 };
//	GLfloat no_shininess[] = { 0.0 };
//	GLfloat low_shininess[] = { 5.0 };
//	GLfloat high_shininess[] = { 100.0 };
//	GLfloat mat_emission[] = { 0.3,0.2,0.2,0.0 };
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	//first left sphere
//	glPushMatrix();
//	glTranslatef(-3.75, 0.0, 0.0);
//	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
//	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
//	glutSolidSphere(1.0, 16, 16);
//	glPopMatrix();
//
//	//second sphere
//	glPushMatrix();
//	glTranslatef(-1.25, 0.0, 0.0);
//	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//	glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
//	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
//	glutSolidSphere(1.0, 16, 16);
//	glPopMatrix();
//
//	//third sphere
//	glPushMatrix();
//	glTranslatef(1.25, 0.0, 0.0);
//	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
//	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
//	glutSolidSphere(1.0, 16, 16);
//	glPopMatrix();
//
//	//last right sphere
//	glPushMatrix();
//	glTranslatef(3.75, 0.0, 0.0);
//	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
//	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
//	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
//	glutSolidSphere(1.0, 16, 16);
//	glPopMatrix();
//
//	glutSwapBuffers();
//	glutPostRedisplay();
//}
//
//void handleResize(int w, int h) {
//	glViewport(0, 0, w, h);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(37.0, (float)w / (float)h, 1.0, 200.0);
//}
//
//int main(int argc, char** argv) {
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
//	glutInitWindowSize(600, 600);
//	glutCreateWindow("My reflection");
//	initRendering();
//	glutDisplayFunc(drawScene);
//	glutReshapeFunc(handleResize);
//	glutMainLoop();
//}

//const float LIGHT_POS = 7.0f; //The length of each side of the cube
//const float BOX_HEIGHT = -4; //The height of the box off of the ground
//const float FLOOR_SIZE = 20.0f; //The length of each side of the floor
//const float floor_on = 2;
//float keyboard_x = 0;
//float keyboard_alpha = 1 / (abs(BOX_HEIGHT) + abs(floor_on));
//
////Draws the cube
//void drawCube() {
//	glPushMatrix();
//	glutSolidSphere(3, 10, 10);
//	glPopMatrix();
//}
//
////Draws the floor
//void drawFloor() {
//	glPushMatrix();
//	glTranslated(floor_on, 0, 0);
//		glBegin(GL_QUADS);
//			//glNormal3f(0, 1, 0);
//			glVertex3f(0, -0.5 * FLOOR_SIZE, -0.5 * FLOOR_SIZE);
//			glVertex3f(0, -0.5 * FLOOR_SIZE,  0.5 * FLOOR_SIZE);
//			glVertex3f(0,  0.5 * FLOOR_SIZE,  0.5 * FLOOR_SIZE);
//			glVertex3f(0,  0.5 * FLOOR_SIZE, -0.5 * FLOOR_SIZE);
//		glEnd();
//	glPopMatrix();
//}
//
//void initRendering() {
//	glClearColor(1.0, 1.0, 1.0, 1.0);
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_NORMALIZE);
//	glEnable(GL_COLOR_MATERIAL);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//}
//
//void handleResize(int w, int h) {
//	glViewport(0, 0, w, h);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(37.0, (float)w / (float)h, 1.0, 200.0);
//}
//
//void drawScene() {
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//	glTranslatef(0.0f, 0.0f, -40.0f);
//	glRotatef(20, 0, 1, 0);
//
//	//GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
//	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
//	//
//	//GLfloat lightColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
//	//GLfloat lightPos[] = { -2 * LIGHT_POS , LIGHT_POS , 4 * LIGHT_POS , 1.0f };
//	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
//	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
//
//	glColor3f(0, 0, 0.5);
//	glPushMatrix();
//	glTranslatef(BOX_HEIGHT, 0, 0);
//	glTranslatef(keyboard_x, 0, 0);
//	drawCube();
//	glPopMatrix();
//
//	glEnable(GL_STENCIL_TEST); //Enable using the stencil buffer
//	glColorMask(0, 0, 0, 0); //Disable drawing colors to the screen
//	glDisable(GL_DEPTH_TEST); //Disable depth testing
//	glStencilFunc(GL_ALWAYS, 1, 1); //Make the stencil test always pass
//	//Make pixels in the stencil buffer be set to 1 when the stencil test passes
//	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//	//Set all of the pixels covered by the floor to be 1 in the stencil buffer
//	drawFloor();
//
//	glColorMask(1, 1, 1, 1); //Enable drawing colors to the screen
//	glEnable(GL_DEPTH_TEST); //Enable depth testing
//	//Make the stencil test pass only when the pixel is 1 in the stencil buffer
//	glStencilFunc(GL_EQUAL, 1, 1);
//	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Make the stencil buffer not change
//
//	//Draw the cube, reflected vertically, at all pixels where the stencil
//	//buffer is 1
//	glPushMatrix();
//	glEnable(GL_BLEND);
//	glColor4f(0, 0.5, 0, 2 * (keyboard_alpha - 0.2));
//	glScalef(-1, 1, 1);
//	glTranslatef(keyboard_x, 0, 0);
//	glTranslatef(-2 * floor_on, 0, 0);
//	glTranslatef(BOX_HEIGHT, 0, 0);
//	drawCube();
//	glDisable(GL_BLEND);
//	glPopMatrix();
//
//	glDisable(GL_STENCIL_TEST); //Disable using the stencil buffer
//
//	//Blend the floor onto the screen
//	//glEnable(GL_BLEND);
//	//glColor4f(0.5, 0, 0, 0.5f);
//	//drawFloor();
//	//glDisable(GL_BLEND);
//
//	glutSwapBuffers();
//	glutPostRedisplay();
//}
//
//void handlerArrow(int key, int x, int y) {
//	switch (key) {
//	case GLUT_KEY_LEFT:
//		keyboard_x -= 0.1;
//		keyboard_alpha = 1 / (abs(BOX_HEIGHT) + abs(floor_on) - keyboard_x);
//		std::cout << keyboard_alpha << " " << BOX_HEIGHT << " " << floor_on << " " << keyboard_x << std::endl;
//		break;
//	case GLUT_KEY_RIGHT:
//		keyboard_x += 0.1;
//		keyboard_alpha = 1 / (abs(BOX_HEIGHT) + abs(floor_on) - keyboard_x);
//		std::cout << keyboard_alpha << " " << BOX_HEIGHT << " " << floor_on << " " << keyboard_x << std::endl;
//		break;
//	}
//}
//
//int main(int argc, char** argv) {
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
//	glutInitWindowSize(600, 600);
//
//	glutCreateWindow("Sphere reflection");
//	initRendering();
//	glutDisplayFunc(drawScene);
//	glutReshapeFunc(handleResize);
//	glutSpecialFunc(handlerArrow);
//	glutMainLoop();
//	return 0;
//}


//double yaw{}, pitch{}, roll{};
//int image_width_, image_height_, image_comp_;
//GLuint tex;
//GLUquadric* sphere;
//
//void loadTexture(std::string filename) {
//    unsigned char* data = stbi_load(filename.c_str(), &image_width_, &image_height_, &image_comp_, 3);
//    if (!data)
//        return;
//
//    glGenTextures(1, &tex);
//    glBindTexture(GL_TEXTURE_2D, tex);
//    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image_width_, image_height_, GL_RGB, GL_UNSIGNED_BYTE, data);
//
//    free(data);
//}
//
//void initRendering() {
//	glClearColor(1.0, 1.0, 1.0, 1.0);
//	//glEnable(GL_DEPTH_TEST);
//	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	loadTexture("../../data/src/daphnia/outer.png");
//	sphere = gluNewQuadric();
//	gluQuadricDrawStyle(sphere, GLU_FILL);
//	glBindTexture(GL_TEXTURE_2D, tex);
//	gluQuadricTexture(sphere, GL_TRUE);
//	gluQuadricNormals(sphere, GLU_SMOOTH);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//}
//
//void handleResize(int w, int h) {
//	glViewport(0, 0, w, h);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(37.0, (float)w / (float)h, 1.0, 200.0);
//}
//
//float ratio = 1.5;
//float ratio_head = 1.6;
//
//float scale = 1.7;
//float scale_2 = 1.5;
//float scale_3 = 0.7;
//
//float radius = 0.1;
//void drawScene() {
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//	//gluLookAt(0, 0, 0,
//	//		  0, 0, -1,
//	//		  0, 1, 0);
//
//	glPushMatrix();
//		glTranslated(-0.5, 0, -3);
//		glRotated(-pitch, 1, 0, 0);
//		glRotated(-roll, 0, 1, 0);
//		glRotated(-yaw, 0, 0, 1);
//		glScaled(scale, scale, scale);
//
//		glPushMatrix();
//			glPushMatrix();
//				glTranslated(0.75 * radius * scale_2 * ratio, radius * scale_2 - radius, 0);
//				glRotated(-40, 0, 0, 1);
//				glColor4f(0.5, 0.5, 0.5, 0.5);
//				glScaled(scale_3, scale_3, scale_3);
//				glScaled(ratio_head, 1., 1.);
//				glutSolidSphere(radius, 32, 32);
//			glPopMatrix();
//
//			glPushMatrix();
//				glColor4f(0.5, 0.5, 0.5, 0.5);
//				glScaled(scale_2, scale_2, scale_2);
//				glScaled(ratio, 1., 1.);
//				gluSphere(sphere, radius, 32, 32);
//			glPopMatrix();
//
//			glPushMatrix();
//				glTranslatef(0, radius * scale_2 - radius, 0);
//				glColor4f(0.5, 0.5, 0.5, 0.5);
//				glScaled(ratio, 1., 1.);
//				glutSolidSphere(radius, 32, 32);
//			glPopMatrix();
//		glPopMatrix();
//	glPopMatrix();
//
//	glutSwapBuffers();
//	glutPostRedisplay();
//}
//
//void handlerKeyboard(unsigned char key, int x, int y) {
//	switch (key) {
//	case 'w':
//		pitch += 1;
//		break;
//	case 's':
//		pitch -= 1;
//		break;
//	case 'a':
//		roll += 1;
//		break;
//	case 'd':
//		roll -= 1;
//		break;
//	case 'q':
//		yaw += 1;
//		break;
//	case 'e':
//		yaw -= 1;
//		break;
//	}
//}
//
//void main(int argc, char** argv) {
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowSize(1000, 1000);
//
//	glutCreateWindow("Sphere reflection");
//	initRendering();
//	glutDisplayFunc(drawScene);
//	glutReshapeFunc(handleResize);
//	glutKeyboardFunc(handlerKeyboard);
//	glutMainLoop();
//}


double mat_data[] = {
    0.9848, 0.1736, 0, 0,
    -0.1736, 0.9848, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

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

    glLoadMatrixd(&mat_data[0]);

    glColor3f(0, 0, 0);
    glPushMatrix();
        glTranslatef(-0.5, 0, -5);
        glMultMatrixd(&mat_data[0]);
        //glRotatef(30, 1, 0, 0);
        //glRotatef(30, 0, 1, 0);
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
