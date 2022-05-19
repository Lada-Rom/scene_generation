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

////////// getRCODaphnia //////////
Daphnia Scene::getRCODaphnia(size_t frame, size_t object) {
    return random_clip_objects_[frame][object];
}

////////// getRCODaphniaLength //////////
double Scene::getRCODaphniaLength(size_t frame, size_t object) {
    return random_clip_objects_[frame][object].getLength();
}

////////// getRCOObjectsNum //////////
size_t Scene::getRCOObjectsNum(size_t frame) {
    return random_clip_objects_[frame].size();
}

////////// setCameraRMat //////////
void Scene::setCameraRMat(const std::array<double, 9>& rmat) {
    camera_.setRMat(rmat);
}

////////// setCameraTVec //////////
void Scene::setCameraTVec(const std::array<double, 3>& tvec) {
    camera_.setTVec(tvec);
}

////////// setCameraSVec //////////
void Scene::setCameraSVec(const std::array<double, 3>& svec) {
    camera_.setSVec(svec);
}

////////// setObjGridPoints //////////
void Scene::setObjGridPoints(const std::vector<cv::Point3d>& objgridpoints) {
    objgridpoints_ = objgridpoints;
}

////////// setGridFilename //////////
void Scene::setGridFilename(const std::string& filename) {
    grid_filename_ = filename;
}

////////// setRCOFrames //////////
void Scene::setRCOFrames(const size_t& num_frames) {
    random_clip_objects_ = std::vector<std::vector<Daphnia>>{ num_frames };
}

////////// setRCOObjects //////////
void Scene::setRCOObjects(const size_t& index, const size_t& num_objects) {
    random_clip_objects_[index] = std::vector<Daphnia>{ num_objects };
}

////////// setRCODaphniaCoords //////////
void Scene::setRCODaphniaCoords(const size_t& frame, const size_t& object,
    const std::array<double, 3>& coords) {
    random_clip_objects_[frame][object].setCoords(coords);
}

////////// setRCODaphniaAngles //////////
void Scene::setRCODaphniaAngles(const size_t& frame, const size_t& object,
    const std::array<double, 3>& angles) {
    random_clip_objects_[frame][object].setAngles(angles);
}

////////// setRCODaphniaScale //////////
void Scene::setRCODaphniaScale(const size_t& frame, const size_t& object,
    const double& scale) {
    random_clip_objects_[frame][object].setScale(scale);
}

////////// setGenFramesPath //////////
void Scene::setGenFramesPath(const std::string& path) {
    generation_frames_path_ = path;
}

////////// resetFrameCount //////////
void Scene::resetFrameCount() {
    frame_count_ = 0;
}

////////// calcOuterCameraParams //////////
void Scene::calcOuterCameraParams(const std::vector<cv::Point2d>& imgpoints,
    const cv::Mat& camera_mat, cv::Mat& dist_coeffs, cv::Mat& rvec, cv::Mat& tvec) {
    std::vector<cv::Point3d> objpoints = aquarium_.getObjpoints(scale_);
    std::cout << "Scaled verticies:\n" << objpoints << std::endl;
    std::cout << "Vertices:\n" << aquarium_.getVertices() << "\n" << std::endl;

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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_NORMALIZE);
    //glEnable(GL_COLOR_MATERIAL);
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

////////// drawReflection //////////
void Scene::drawReflection(const Daphnia& daphnia,
    const std::array<double, 3>& color3d) {
    std::array<double, 3> aq_size = aquarium_.getSize();
    std::array<double, 3> coords = daphnia.getCoords();

    //right plane
    if (coords[0] > 0.5 * aq_size[0] - reflection_distance_limit_)
        drawRightPlaneReflection(daphnia, color3d);

    //left plane
    if (coords[0] < -0.5 * aq_size[0] + reflection_distance_limit_)
        drawLeftPlaneReflection(daphnia, color3d);

    //upper plane
    //if (coords[1] > 0.5 * aq_size[1] - reflection_distance_limit_) {
    //
    //}

    //lower plane
    //if (coords[1] < -0.5 * aq_size[1] + reflection_distance_limit_) {
    //
    //}

}

////////// drawRightPlaneReflection //////////
void Scene::drawRightPlaneReflection(const Daphnia& daphnia,
    const std::array<double, 3>& color3d) {
    glEnable(GL_STENCIL_TEST);

    glColorMask(0, 0, 0, 0); //disable drawing colors
    glStencilFunc(GL_ALWAYS, 1, 1); //make the stencil test always pass
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //set 1 to pixels that passed stencil test
    aquarium_.drawRightPlane(); //set right plane pixels to 1

    glColorMask(1, 1, 1, 1); //enable drawing colors
    glStencilFunc(GL_EQUAL, 1, 1); //make pixel pass when pixel is 1 in stencil buffer
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //make the stencil buffer not change

    //draw reflection
    std::array<double, 3> aq_size = aquarium_.getSize();
    std::array<double, 3> coords = daphnia.getCoords();
    double diff = abs(abs(0.5 * aq_size[0]) - abs(coords[0]));
    daphnia.drawReflection(
        { -0.5 * aq_size[0] - diff, coords[1], coords[2] },
        { color3d[0], color3d[1], color3d[2], reflection_strength_});

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
}

////////// drawLeftPlaneReflection //////////
void Scene::drawLeftPlaneReflection(const Daphnia& daphnia,
    const std::array<double, 3>& color3d) {
    glEnable(GL_STENCIL_TEST);

    glColorMask(0, 0, 0, 0); //disable drawing colors
    glStencilFunc(GL_ALWAYS, 1, 1); //make the stencil test always pass
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //set 1 to pixels that passed stencil test
    aquarium_.drawLeftPlane(); //set right plane pixels to 1

    glColorMask(1, 1, 1, 1); //enable drawing colors
    glStencilFunc(GL_EQUAL, 1, 1); //make pixel pass when pixel is 1 in stencil buffer
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //make the stencil buffer not change

    //draw reflection
    std::array<double, 3> aq_size = aquarium_.getSize();
    std::array<double, 3> coords = daphnia.getCoords();
    double diff = abs(abs(0.5 * aq_size[0]) - abs(coords[0]));
    daphnia.drawReflection(
        { 0.5 * aq_size[0] + diff, coords[1], coords[2] },
        { color3d[0], color3d[1], color3d[2], reflection_strength_ });

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
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
    glTranslated(
        camera_.getSVec()[0],
        camera_.getSVec()[1],
        camera_.getSVec()[2]);

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

////////// displayRandomClip //////////
void Scene::displayRandomClip() {
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
    glTranslated(
        camera_.getSVec()[0],
        camera_.getSVec()[1],
        camera_.getSVec()[2]);

    //objects
    for (auto& daphnia : random_clip_objects_[frame_count_]) {
        daphnia.draw({ 0., 0., 0., 1. });
        drawReflection(daphnia, { 0., 0., 0. });
    }

    //aquarium with axis
    glColor3d(0., 0., 0.);
    aquarium_.draw();

    //write to file
    saveImage(viewport[2], viewport[3], generation_frames_path_
        + std::to_string(frame_count_) + generation_frames_ending_);
    ++frame_count_;

    glutSwapBuffers();
    glutPostRedisplay();

    if (frame_count_ == random_clip_objects_.size())
        glutLeaveMainLoop();
}
