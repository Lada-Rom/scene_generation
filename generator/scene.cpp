#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "scene.h"

////////// Scene //////////
Scene::Scene() {
	camera_.setOpticalCenter(render_image_size_);
	camera_.calcIntrinsicMatrix();
}

////////// getRenderImageSize //////////
cv::Size2i  Scene::getRenderImageSize() {
    return render_image_size_;
}

////////// getIntrinsicCameraMatrix //////////
std::array<double, 9> Scene::getIntrinsicCameraMatrix() {
    return camera_.getIntrinsicMatrix();
}

////////// getAquariumSize //////////
std::array<double, 3> Scene::getAquariumSize() {
    return aquarium_.getSize();
}

////////// getScale //////////
double Scene::getScale() {
    return scale_;
}

////////// setCameraRMat //////////
void Scene::setCameraRMat(const std::array<double, 9>& rmat) {
    camera_.setRMat(rmat);
}

////////// setCameraTVec //////////
void Scene::setCameraTVec(const std::array<double, 3>& tvec) {
    camera_.setTVec(tvec);
}

////////// setObjGridPoints //////////
void Scene::setObjGridPoints(const std::vector<cv::Point3d>& objgridpoints) {
    objgridpoints_ = objgridpoints;
}

////////// setGridFilename //////////
void Scene::setGridFilename(const std::string& filename) {
    grid_filename_ = filename;
}

////////// calcOuterCameraParams //////////
void Scene::calcOuterCameraParams(const std::vector<cv::Point2d>& imgpoints,
    const cv::Mat& camera_mat, cv::Mat& dist_coeffs, cv::Mat& rvec, cv::Mat& tvec) {
    std::vector<cv::Point3d> objpoints = aquarium_.getObjpoints(scale_);

    std::vector<cv::Point3d> objpoints_4;
    std::vector<cv::Point2d> imgpoints_4;

    //second 4 points
    objpoints_4 = std::vector<cv::Point3d>(objpoints.begin() + 4, objpoints.begin() + 8);
    imgpoints_4 = std::vector<cv::Point2d>(imgpoints.begin() + 4, imgpoints.begin() + 8);

    //find rotation and translation
    cv::solvePnP(objpoints_4, imgpoints_4, camera_mat, dist_coeffs, rvec, tvec, false, cv::SOLVEPNP_AP3P);

    //first 4 points
    objpoints_4 = std::vector<cv::Point3d>(objpoints.begin(), objpoints.begin() + 4);
    imgpoints_4 = std::vector<cv::Point2d>(imgpoints.begin(), imgpoints.begin() + 4);

    //optimize rotation and translation
    cv::solvePnP(objpoints_4, imgpoints_4, camera_mat, dist_coeffs, rvec, tvec, true, cv::SOLVEPNP_AP3P);

    //refine values
    cv::solvePnPRefineVVS(objpoints, imgpoints, camera_mat, dist_coeffs, rvec, tvec);
}

////////// saveImage //////////
void Scene::saveImage(int width, int height, std::string& filepath) {
    unsigned char* data = (unsigned char*)malloc((int)(width * height * (3)));
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_flip_vertically_on_write(true);
    stbi_write_png(filepath.c_str(), width, height, 3, data, 0);
}

////////// initGLUT //////////
void Scene::initGLUT(GLfloat red, GLfloat green, GLfloat blue) {
    glClearColor(red, green, blue, 0.0);
    glShadeModel(GL_FLAT);
}

////////// drawAxis //////////
void Scene::drawAxis() {
    glPushMatrix();
        glBegin(GL_LINES);
            //x-axis
            glColor3d(1, 0, 0);
            glVertex3d(0, 0, 0);
            glVertex3d(2, 0, 0);

            //y-axis
            glColor3d(0, 1, 0);
            glVertex3d(0, 0, 0);
            glVertex3d(0, 2, 0);

            //z-axis
            glColor3d(0, 0, 1);
            glVertex3d(0, 0, 0);
            glVertex3d(0, 0, 2);
        glEnd();
    glPopMatrix();
}

////////// reshape //////////
void Scene::reshape(int width, int height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera_.getAngle(),
        (GLfloat)width / (GLfloat)height,
        z_plane_[0], z_plane_[1]);
    glMatrixMode(GL_MODELVIEW);
}

////////// displayPointGrid //////////
void Scene::displayPointGrid() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0, 0, 0,
              0, 0, -1,
              0, 1, 0);

    glLoadMatrixd(&camera_.getRMat()[0][0]);
    glTranslated(
         camera_.getTVec()[0],
        -camera_.getTVec()[1],
        -abs(camera_.getTVec()[2]));
    glTranslated(-0.7, 0.21, 0);

    //object grid
    glColor3f(0, 0, 0);
    for (auto& e : objgridpoints_) {
        glPushMatrix();
            glTranslated(e.x, e.y, e.z);
            glutSolidSphere(0.1, 10, 10);
        glPopMatrix();
    }

    //aquarium with axis
    //glScaled(scale_, scale_, scale_);
    aquarium_.draw();

    //write to file
    saveImage(viewport[2], viewport[3], grid_filename_);

    glutSwapBuffers();
    glutPostRedisplay();

    glutLeaveMainLoop();
}


