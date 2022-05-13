#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS

#include "scene.h"

std::ostream& operator<<(std::ostream& ostrm, const glm::dmat3& rhs) {
    ostrm << "{ { " << rhs[0][0] << " " << rhs[0][1] << " " << rhs[0][2] << " },\n"
        << "  { " << rhs[1][0] << " " << rhs[1][1] << " " << rhs[1][2] << " },\n"
        << "  { " << rhs[2][0] << " " << rhs[2][1] << " " << rhs[2][2] << " } }";
    return ostrm;
}

std::ostream& operator<<(std::ostream& ostrm, const glm::dmat4& rhs) {
    ostrm << "{ { " << rhs[0][0] << " " << rhs[0][1] << " " << rhs[0][2] << " " << rhs[0][3] << " },\n"
        << "  { " << rhs[1][0] << " " << rhs[1][1] << " " << rhs[1][2] << " " << rhs[1][3] << " },\n"
        << "  { " << rhs[2][0] << " " << rhs[2][1] << " " << rhs[2][2] << " " << rhs[2][3] << " },\n"
        << "  { " << rhs[3][0] << " " << rhs[3][1] << " " << rhs[3][2] << " " << rhs[3][3] << " } }";
    return ostrm;
}

std::ostream& operator<<(std::ostream& ostrm, const glm::dvec3& rhs) {
    ostrm << "{ " << rhs[0] << " " << rhs[1] << " " << rhs[2] << " }";
    return ostrm;
}

void Scene::initializeGLUT(GLfloat red, GLfloat green, GLfloat blue) {
    glClearColor(red, green, blue, 0.0);
    glShadeModel(GL_FLAT);
}

void Scene::displayPointCorrespondence() {
    //glViewport(0, 0, 1280, 1280);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glLoadIdentity();
    //gluLookAt(0, 0, 0,
    //          0, 0, 0,
    //          0, 1, 0);
    gluLookAt(eye_[0], eye_[1], eye_[2],
              center_[0], center_[1], center_[2],
              up_[0], up_[1], up_[2]);

    //вне Push-Pop - перенос и поворот всех объектов сцены
    //static
    //    //float z = -20;
    //    float z = 0;
    //z -= 0.05;
    //std::cout << z << std::endl;
    //glTranslatef(0, 0, z);
    
    //object grid
    glColor3f(0, 0, 0);
    for (auto& e : objgridpoints) {
        glPushMatrix();
            glTranslated(e.x, e.y, e.z);
            glutSolidSphere(0.1, 10, 10);
        glPopMatrix();
    }

    //aquarium with axis
    glScaled(scaling_factor_, scaling_factor_, scaling_factor_);
    aquarium_.draw();

    //write to file
    std::string filename = "../../data/grid_glut.png";
    saveImage(viewport[2], viewport[3], filename);

    glutSwapBuffers();
    glutPostRedisplay();

    glutLeaveMainLoop();
}

void Scene::displayPointCorrespondencev2() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0, 0, 0,
              0, 0, -1,
              0, 1, 0);

    glLoadMatrixd(&rotation_4_[0][0]);
    glTranslated(translation_[0], -translation_[1], -abs(translation_[2]));
    glTranslated(-0.7, 0.21, 0);

    //object grid
    glColor3f(0, 0, 0);
    for (auto& e : objgridpoints) {
        glPushMatrix();
        glTranslated(e.x, e.y, e.z);
        glutSolidSphere(0.1, 10, 10);
        glPopMatrix();
    }

    //aquarium with axis
    //glScaled(scaling_factor_, scaling_factor_, scaling_factor_);
    aquarium_.draw();

    //write to file
    std::string filename = "../../data/grid/grid_glut.png";
    saveImage(viewport[2], viewport[3], filename);

    glutSwapBuffers();
    glutPostRedisplay();

    glutLeaveMainLoop();
}

void Scene::displayPointCorrespondenceParamsSelectionInnerGL() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(eye_[0], eye_[1], eye_[2],
        center_[0], center_[1], center_[2],
        up_[0], up_[1], up_[2]);

    //object grid
    glColor3f(0, 0, 0);
    for (auto& e : objgridpoints) {
        glPushMatrix();
        glTranslated(e.x, e.y, e.z);
        glutSolidSphere(0.1, 10, 10);
        glPopMatrix();
    }

    //aquarium with axis
    glScaled(scaling_factor_, scaling_factor_, scaling_factor_);
    aquarium_.draw();

    //write to file
    if (filename_params_selection_.empty())
        std::cout << "Empty filename" << std::endl;
        //throw std::invalid_argument("Empty filename");
    saveImage(viewport[2], viewport[3], filename_params_selection_);

    glutSwapBuffers();
    glutPostRedisplay();

    glutLeaveMainLoop();
}

void Scene::displayPointCorrespondenceParamsSelectionOuterGL() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glClear(GL_COLOR_BUFFER_BIT);

    //recalc lookAt corresponding to params
    calcLookAtArray(filename_eye_params_selection[frame_count].first);

    glLoadIdentity();
    gluLookAt(eye_[0], eye_[1], eye_[2],
        center_[0], center_[1], center_[2],
        up_[0], up_[1], up_[2]);

    //object grid
    glColor3f(0, 0, 0);
    for (auto& e : objgridpoints) {
        glPushMatrix();
        glTranslated(e.x, e.y, e.z);
        glutSolidSphere(0.1, 10, 10);
        glPopMatrix();
    }

    //aquarium with axis
    glScaled(scaling_factor_, scaling_factor_, scaling_factor_);
    aquarium_.draw();

    //write to file
    if (filename_eye_params_selection[frame_count].second.empty())
        std::cout << "Empty filename" << std::endl;
        //throw std::invalid_argument("Empty filename");
    saveImage(viewport[2], viewport[3], filename_eye_params_selection[frame_count].second);

    glutSwapBuffers();
    glutPostRedisplay();
    ++frame_count;

    if (frame_count == filename_eye_params_selection.size())
        glutLeaveMainLoop();
}

void Scene::display() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glLoadIdentity();
    gluLookAt(0, 0, 0,
              0, 0, -1,
              0, 1, 0);

    glLoadMatrixd(&rotation_4_[0][0]);

    glColor3f(0, 0, 0);
    glPushMatrix();
        glTranslatef(-1, -1, -5);
        glutSolidSphere(0.1, 10, 10);
    glPopMatrix();

    glColor3f(1, 0, 0);
    glPushMatrix();
        glTranslatef(1, -1, -5);
        glutSolidSphere(0.1, 10, 10);
    glPopMatrix();

    glColor3f(0, 1, 0);
    glPushMatrix();
        glTranslatef(-1, 1, -5);
        glutSolidSphere(0.1, 10, 10);
    glPopMatrix();

    glColor3f(0, 0, 1);
    glPushMatrix();
        glTranslatef(1, 1, -5);
        glutSolidSphere(0.1, 10, 10);
    glPopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}

void Scene::reshapeContent(unsigned int width, unsigned height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera_angle_, (GLfloat)width / (GLfloat)height, z_near_, z_far_);
    glMatrixMode(GL_MODELVIEW);
}


void Scene::setFrameCount(const size_t& count) {
    frame_count = count;
}

void Scene::setFilenameParamsSelection(const std::string& filename) {
    filename_params_selection_ = filename;
}

void Scene::addFilenameEyeParamsSelection(std::pair<cv::Point3d, std::string> element) {
    filename_eye_params_selection.push_back(element);
}


void Scene::setRotationMatrix(const cv::Mat& rmat) {
    rotation_ = glm::make_mat3((double*)rmat.data);
    std::cout << "Rotation matrix:\n" << rotation_ << "\n" << std::endl;
}

void Scene::setRotationMatrix4(const cv::Mat& rmat, bool transponate, bool inverse) {
    rotation_ = glm::make_mat3((double*)rmat.data);
    
    rotation_4_[0][0] = rotation_[0][0];
    rotation_4_[0][1] = rotation_[0][1];
    rotation_4_[0][2] = rotation_[0][2];

    rotation_4_[1][0] = rotation_[1][0];
    rotation_4_[1][1] = rotation_[1][1];
    rotation_4_[1][2] = rotation_[1][2];

    rotation_4_[2][0] = rotation_[2][0];
    rotation_4_[2][1] = rotation_[2][1];
    rotation_4_[2][2] = rotation_[2][2];

    rotation_4_[3][3] = 1.;

    if (transponate)
        rotation_4_ = glm::transpose(rotation_4_);

    if (inverse)
        rotation_4_ = glm::inverse(rotation_4_);

    std::cout << "Rotation matrix4:\n" << rotation_4_ << "\n" << std::endl;
}

void Scene::setRotationMatrix(double *data) {
     rotation_data_ = data;
}

void Scene::setRotationMatrix(const glm::mat4& rmat) {
    rotation_4_ = rmat;
    std::cout << "Rotation matrix4:\n" << rotation_4_ << "\n" << std::endl;
}

void Scene::setTranslationVector(const cv::Mat& tvec) {
    translation_ = glm::make_vec3((double*)tvec.data);
    std::cout << "Translation vector:\n" << translation_ << " \n"  << std::endl;
}

void Scene::setObjGridPoints(const std::vector<cv::Point3d>& src) {
    objgridpoints.clear();
    for (auto& e : src)
        objgridpoints.push_back({ e.x, e.y, e.z });
}


double Scene::getScalingFactor() {
    return scaling_factor_;
}


double Scene::getAquariumWidth() {
    return aquarium_.getWidth();
}

double Scene::getAquariumHeight() {
    return aquarium_.getHeight();
}

double Scene::getAquariumDepth() {
    return aquarium_.getDepth();
}

std::vector<double> Scene::getAquariumVerticies() {
    return aquarium_.getVerticies();
}


void Scene::addWHDToAquarium(
    const double dw, const double dh, const double dd, bool recalc) {
    aquarium_.addWidthHeightDepth(dw, dh, dd, recalc);
}


void Scene::saveImage(int width, int height, std::string& filepath) {
    unsigned char* data = (unsigned char*)malloc((int)(width * height * (3)));
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_flip_vertically_on_write(true);
    stbi_write_png(filepath.c_str(), width, height, 3, data, 0);
}

void Scene::calcLookAtArray() {
    glm::dmat3 rotation_i = glm::inverse(rotation_);
    glm::dmat3 rotation_t = glm::transpose(rotation_i);

    //eye
    //eye_ = translation_;
    //eye_[0] *= 2;
    //eye_[1] = -eye_[1];

    //eye_[0] = -1.3;
    //eye_[1] = 0.;

    //eye_[0] = scaling_factor_ * translation_[0];
    //eye_[1] = scaling_factor_ * translation_[1];
    eye_[0] = -0.4;
    eye_[1] = 0.7;
    eye_[2] = std::abs(translation_[2]) - 0.3;
    std::cout << "eye: " << eye_[0] << " " << eye_[1] << " " << eye_[2] << std::endl;

    //center
    //glm::dvec3 view = { 0, 0, eye_[2] };
    //center_[0] = view[0] / view[2] * (-eye_[2]) + eye_[0];
    //center_[1] = view[1] / view[2] * (-eye_[2]) + eye_[1];
    //center_[2] = 0;

    //center_ = -scaling_factor_ * translation_;
    //center_[2] = 0;
    center_ = {0., 0., 0.};

    //center_ = glm::normalize(center_);
    std::cout << "center: " << center_[0] << " " << center_[1] << " " << center_[2] << std::endl;

    //up
    up_ = -glm::normalize(rotation_ * up_);
    std::cout << "up: " << up_[0] << " " << up_[1] << " " << up_[2] << std::endl;
}

void Scene::calcLookAtArray(const double eye0, const double eye1, const double eye2) {
    //eye
    eye_[0] = eye0;
    eye_[1] = eye1;
    eye_[2] = std::abs(translation_[2]) - eye2;
    //std::cout << "\teye: " << eye_[0] << " " << eye_[1] << " " << eye_[2] << std::endl;

    //center
    center_ = { 0., 0., 0. };
    //std::cout << "\tcenter: " << center_[0] << " " << center_[1] << " " << center_[2] << std::endl;

    //up
    up_ = -glm::normalize(rotation_ * up_);
    //std::cout << "\tup: " << up_[0] << " " << up_[1] << " " << up_[2] << "\n" << std::endl;
}

void Scene::calcLookAtArray(const cv::Point3d& eye) {
    //eye
    eye_[0] = eye.x;
    eye_[1] = eye.y;
    eye_[2] = std::abs(translation_[2]) - eye.z;
    //std::cout << "\teye: " << eye_[0] << " " << eye_[1] << " " << eye_[2] << std::endl;

    //center
    center_ = { 0., 0., 0. };
    //std::cout << "\tcenter: " << center_[0] << " " << center_[1] << " " << center_[2] << std::endl;

    //up
    up_ = -glm::normalize(rotation_ * up_);
    //std::cout << "\tup: " << up_[0] << " " << up_[1] << " " << up_[2] << "\n" << std::endl;
}
