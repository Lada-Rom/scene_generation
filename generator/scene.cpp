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

////////// getRCODaphniaCoords //////////
std::array<double, 3> Scene::getRCODaphniaCoords(size_t frame, size_t object) {
    return random_clip_objects_[frame][object].getCoords();
}

////////// getRCODaphniaLength //////////
double Scene::getRCODaphniaLength(size_t frame, size_t object) {
    return random_clip_objects_[frame][object].getLength();
}

////////// getRCOObjectsNum //////////
size_t Scene::getRCOObjectsNum(size_t frame) {
    return random_clip_objects_[frame].size();
}

////////// getSCODaphniaCoords //////////
std::array<double, 3> Scene::getSCODaphniaCoords(size_t frame, size_t object) {
    return sequent_clip_objects_[frame][object].getCoords();
}

////////// getSCODaphniaTextureSourceFilename //////////
std::string Scene::getSCODaphniaTextureSourceFilename(size_t frame, size_t object) {
    return sequent_clip_objects_[frame][object].getTextureSourceFilename();
}

////////// getSCODaphniaLength //////////
double Scene::getSCODaphniaLength(size_t object) {
    return sequent_clip_objects_[0][object].getLength();
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

////////// setAquariumEdgeTextureFilename //////////
void Scene::setAquariumEdgeTextureFilename(
    const std::string& edge, const std::string& filename) {
    aquarium_.setTextureFilename(edge, filename);
}

////////// setRCOFrames //////////
void Scene::setRCOFrames(size_t num_frames) {
    random_clip_objects_ = std::vector<std::vector<Daphnia>>{ num_frames };
}

////////// setRCOObjects //////////
void Scene::setRCOObjects(size_t index, size_t num_objects) {
    random_clip_objects_[index] = std::vector<Daphnia>{ num_objects };
}

////////// setRCODaphniaCoords //////////
void Scene::setRCODaphniaCoords(size_t frame, size_t object,
    const std::array<double, 3>& coords) {
    random_clip_objects_[frame][object].setCoords(coords);
}

////////// setRCODaphniaAngles //////////
void Scene::setRCODaphniaAngles(size_t frame, size_t object,
    const std::array<double, 3>& angles) {
    random_clip_objects_[frame][object].setAngles(angles);
}

////////// setRCODaphniaAnglesWithDirection //////////
std::array<double, 3> Scene::setRCODaphniaDirection(size_t frame, size_t object) {
    std::array<double, 3> direction = random_clip_objects_[frame][object].calcDirection();
    random_clip_objects_[frame][object].setDirection(direction);
    return direction;
}

////////// setRCODaphniaScale //////////
void Scene::setRCODaphniaScale(size_t frame, size_t object,
    const double& scale) {
    random_clip_objects_[frame][object].setScale(scale);
}

////////// setRCODaphniaTextureFilename //////////
void Scene::setRCODaphniaTextureFilename(size_t frame, size_t object,
    const std::string& filename) {
    random_clip_objects_[frame][object].setTextureFilename(filename);
}

////////// setGenFramesPath //////////
void Scene::setGenFramesPath(const std::string& path) {
    generation_frames_path_ = path;
}

////////// setGenObjectMasksPath //////////
void Scene::setGenObjectMasksPath(const std::string& path) {
    generation_objects_masks_path_ = path;
}

////////// setGenReflectionMasksPath //////////
void Scene::setGenReflectionMasksPath(const std::string& path) {
    generation_reflections_masks_path_ = path;
}

////////// resetFrameCount //////////
void Scene::resetFrameCount() {
    frame_count_ = 0;
}

////////// resetObjectCount //////////
void Scene::resetObjectCount() {
    object_count_ = 0;
}

////////// setSCOFrames //////////
void Scene::setSCOFrames(size_t num_frames) {
    sequent_clip_objects_ = std::vector<std::vector<Daphnia>>{ num_frames };
}

////////// setSCOObjects //////////
void Scene::setSCOObjects(size_t num_objects) {
    for (auto& frame : sequent_clip_objects_)
        frame = std::vector<Daphnia>{ num_objects };
}

////////// setSCODaphniaScale //////////
void Scene::setSCODaphniaScale(size_t object, const double& scale) {
    for (auto& frame : sequent_clip_objects_)
        frame[object].setScale(scale);
}

////////// setSCODaphniaCoords //////////
void Scene::setSCODaphniaCoords(size_t frame, size_t object,
    const std::array<double, 3>& coords) {
    sequent_clip_objects_[frame][object].setCoords(coords);
}

////////// setSCODaphniaAngles //////////
void Scene::setSCODaphniaAngles(size_t frame, size_t object,
    const std::array<double, 3>& angles) {
    sequent_clip_objects_[frame][object].setAngles(angles);
}

////////// addSCONextDaphniaAngles //////////
void Scene::addSCONextDaphniaAngles(size_t frame, size_t object,
    const std::array<double, 3>& angles) {
    std::array<double, 3> prev_angles = sequent_clip_objects_[frame - 1][object].getAngles();
    sequent_clip_objects_[frame][object].setAngles({
        prev_angles[0] + angles[0],
        prev_angles[1] + angles[1], 
        prev_angles[2] + angles[2]});
}

////////// setSCODaphniaDirection //////////
std::array<double, 3> Scene::setSCODaphniaDirection(size_t frame, size_t object) {
    std::array<double, 3> direction = sequent_clip_objects_[frame][object].calcDirection();
    sequent_clip_objects_[frame][object].setDirection(direction);
    return direction;
}

////////// applySCODaphniaShift //////////
void Scene::applySCODaphniaShift(size_t frame, size_t object,
    const double& shift) {
    std::array<double, 3> prev_coords = sequent_clip_objects_[frame - 1][object].getCoords();
    std::array<double, 3> curr_direction = sequent_clip_objects_[frame][object].getDirection();
    std::array<double, 3> new_coords = {
        prev_coords[0] + curr_direction[0] * shift,
        prev_coords[1] + curr_direction[1] * shift,
        prev_coords[2] + curr_direction[2] * shift};

    std::array<double, 3> aq_size = aquarium_.getSize();
    double daph_length = sequent_clip_objects_[frame][object].getLength();

    if (new_coords[0] > 0.5 * aq_size[0] - daph_length)
        new_coords[0] = 0.5 * aq_size[0] - daph_length;
    if (new_coords[0] < -0.5 * aq_size[0] + daph_length)
        new_coords[0] = -0.5 * aq_size[0] + daph_length;

    if (new_coords[1] > 0.5 * aq_size[1] - daph_length)
        new_coords[1] = 0.5 * aq_size[1] - daph_length;
    if (new_coords[1] < -0.5 * aq_size[1] + daph_length)
        new_coords[1] = -0.5 * aq_size[1] + daph_length;

    if (new_coords[2] > -daph_length)
        new_coords[2] = -daph_length;
    if (new_coords[2] < -aq_size[2] + daph_length)
        new_coords[2] = -aq_size[2] + daph_length;

    sequent_clip_objects_[frame][object].setCoords(new_coords);
}

////////// setSCODaphniaTextureSourceFilename //////////
void Scene::setSCODaphniaTextureSourceFilename(size_t object,
    const std::string& filename) {
    for (auto& frame : sequent_clip_objects_) {
        frame[object].setTextureSourceFilename(filename);
    }
}

////////// setSCODaphniaTextureFilename //////////
void Scene::setSCODaphniaTextureFilename(size_t frame, size_t object,
    const std::string& filename) {
    sequent_clip_objects_[frame][object].setTextureFilename(filename);
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

////////// initGLUT //////////
void Scene::initGLUT(GLfloat red, GLfloat green, GLfloat blue) {
    glClearColor(red, green, blue, 0.0);
    glShadeModel(GL_FLAT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

////////// drawSimplifiedReflection //////////
void Scene::drawSimplifiedReflection(const Daphnia& daphnia,
    const std::array<double, 4>& color4d) {
    std::array<double, 3> aq_size = aquarium_.getSize();
    std::array<double, 3> coords = daphnia.getCoords();

    //right plane
    if (coords[0] > 0.5 * aq_size[0] - reflection_distance_limit_)
        drawRightPlaneReflection(daphnia, color4d);

    //left plane
    if (coords[0] < -0.5 * aq_size[0] + reflection_distance_limit_)
        drawLeftPlaneReflection(daphnia, color4d);

    //upper plane
    if (coords[1] > 0.5 * aq_size[1] - reflection_distance_limit_)
        drawUpperPlaneReflection(daphnia, color4d);

    //lower plane
    if (coords[1] < -0.5 * aq_size[1] + reflection_distance_limit_)
        drawLowerPlaneReflection(daphnia, color4d);
}

////////// drawSimplifiedReflection //////////
void Scene::drawSimplifiedReflection(const Daphnia& daphnia,
    const std::array<double, 4>& right,
    const std::array<double, 4>& left,
    const std::array<double, 4>& upper,
    const std::array<double, 4>& lower) {
    std::array<double, 3> aq_size = aquarium_.getSize();
    std::array<double, 3> coords = daphnia.getCoords();

    //right plane
    if (coords[0] > 0.5 * aq_size[0] - reflection_distance_limit_)
        drawRightPlaneReflection(daphnia, right);

    //left plane
    if (coords[0] < -0.5 * aq_size[0] + reflection_distance_limit_)
        drawLeftPlaneReflection(daphnia, left);

    //upper plane
    if (coords[1] > 0.5 * aq_size[1] - reflection_distance_limit_)
        drawUpperPlaneReflection(daphnia, upper);

    //lower plane
    if (coords[1] < -0.5 * aq_size[1] + reflection_distance_limit_)
        drawLowerPlaneReflection(daphnia, lower);
}

////////// drawComplicatedReflection //////////
void Scene::drawComplicatedReflection(const Daphnia& daphnia,
    const std::array<double, 4>& color_inner,
    const std::array<double, 4>& color_outer,
    const std::array<double, 4>& color_head) {
    std::array<double, 3> aq_size = aquarium_.getSize();
    std::array<double, 3> coords = daphnia.getCoords();

    //right plane
    if (coords[0] > 0.5 * aq_size[0] - reflection_distance_limit_)
        drawRightPlaneComplicatedReflection(daphnia, color_inner, color_outer, color_head);

    //left plane
    if (coords[0] < -0.5 * aq_size[0] + reflection_distance_limit_)
        drawLeftPlaneComplicatedReflection(daphnia, color_inner, color_outer, color_head);

    //upper plane
    if (coords[1] > 0.5 * aq_size[1] - reflection_distance_limit_)
        drawUpperPlaneComplicatedReflection(daphnia, color_inner, color_outer, color_head);

    //lower plane
    if (coords[1] < -0.5 * aq_size[1] + reflection_distance_limit_)
        drawLowerPlaneComplicatedReflection(daphnia, color_inner, color_outer, color_head);
}

////////// drawRightPlaneReflection //////////
void Scene::drawRightPlaneReflection(const Daphnia& daphnia,
    const std::array<double, 4>& color4d) {
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
    daphnia.drawSimplifiedReflection(true,
        { -0.5 * aq_size[0] - diff, coords[1], coords[2] }, color4d);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
}

////////// drawLeftPlaneReflection //////////
void Scene::drawLeftPlaneReflection(const Daphnia& daphnia,
    const std::array<double, 4>& color4d) {
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
    daphnia.drawSimplifiedReflection(true,
        { 0.5 * aq_size[0] + diff, coords[1], coords[2] }, color4d);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
}

////////// drawUpperPlaneReflection //////////
void Scene::drawUpperPlaneReflection(const Daphnia& daphnia,
    const std::array<double, 4>& color4d) {
    glEnable(GL_STENCIL_TEST);

    glColorMask(0, 0, 0, 0); //disable drawing colors
    glStencilFunc(GL_ALWAYS, 1, 1); //make the stencil test always pass
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //set 1 to pixels that passed stencil test
    aquarium_.drawUpperPlane(); //set right plane pixels to 1

    glColorMask(1, 1, 1, 1); //enable drawing colors
    glStencilFunc(GL_EQUAL, 1, 1); //make pixel pass when pixel is 1 in stencil buffer
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //make the stencil buffer not change

    //draw reflection
    std::array<double, 3> aq_size = aquarium_.getSize();
    std::array<double, 3> coords = daphnia.getCoords();
    double diff = abs(abs(0.5 * aq_size[1]) - abs(coords[1]));
    daphnia.drawSimplifiedReflection(false,
        { coords[0], -0.5 * aq_size[1] - diff, coords[2] }, color4d);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
}

////////// drawLowerPlaneReflection //////////
void Scene::drawLowerPlaneReflection(const Daphnia& daphnia,
    const std::array<double, 4>& color4d) {
    glEnable(GL_STENCIL_TEST);

    glColorMask(0, 0, 0, 0); //disable drawing colors
    glStencilFunc(GL_ALWAYS, 1, 1); //make the stencil test always pass
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //set 1 to pixels that passed stencil test
    aquarium_.drawLowerPlane(); //set right plane pixels to 1

    glColorMask(1, 1, 1, 1); //enable drawing colors
    glStencilFunc(GL_EQUAL, 1, 1); //make pixel pass when pixel is 1 in stencil buffer
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //make the stencil buffer not change

    //draw reflection
    std::array<double, 3> aq_size = aquarium_.getSize();
    std::array<double, 3> coords = daphnia.getCoords();
    double diff = abs(abs(0.5 * aq_size[1]) - abs(coords[1]));
    daphnia.drawSimplifiedReflection(false,
        { coords[0], 0.5 * aq_size[1] + diff, coords[2] }, color4d);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
}

////////// drawRightPlaneComplicatedReflection //////////
void Scene::drawRightPlaneComplicatedReflection(const Daphnia& daphnia,
    const std::array<double, 4>& color_inner,
    const std::array<double, 4>& color_outer,
    const std::array<double, 4>& color_head) {
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
    daphnia.drawComplicatedReflection(true,
        { -0.5 * aq_size[0] - diff, coords[1], coords[2] },
        color_inner, color_outer, color_head);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
}

////////// drawLeftPlaneComplicatedReflection //////////
void Scene::drawLeftPlaneComplicatedReflection(const Daphnia& daphnia,
    const std::array<double, 4>& color_inner,
    const std::array<double, 4>& color_outer,
    const std::array<double, 4>& color_head) {
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
    daphnia.drawComplicatedReflection(true,
        { 0.5 * aq_size[0] + diff, coords[1], coords[2] },
        color_inner, color_outer, color_head);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
}

////////// drawUpperPlaneComplicatedReflection //////////
void Scene::drawUpperPlaneComplicatedReflection(const Daphnia& daphnia,
    const std::array<double, 4>& color_inner,
    const std::array<double, 4>& color_outer,
    const std::array<double, 4>& color_head) {
    glEnable(GL_STENCIL_TEST);

    glColorMask(0, 0, 0, 0); //disable drawing colors
    glStencilFunc(GL_ALWAYS, 1, 1); //make the stencil test always pass
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //set 1 to pixels that passed stencil test
    aquarium_.drawUpperPlane(); //set right plane pixels to 1

    glColorMask(1, 1, 1, 1); //enable drawing colors
    glStencilFunc(GL_EQUAL, 1, 1); //make pixel pass when pixel is 1 in stencil buffer
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //make the stencil buffer not change

    //draw reflection
    std::array<double, 3> aq_size = aquarium_.getSize();
    std::array<double, 3> coords = daphnia.getCoords();
    double diff = abs(abs(0.5 * aq_size[1]) - abs(coords[1]));
    daphnia.drawComplicatedReflection(false,
        { coords[0], -0.5 * aq_size[1] - diff, coords[2] },
        color_inner, color_outer, color_head);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
}

////////// drawLowerPlaneComplicatedReflection //////////
void Scene::drawLowerPlaneComplicatedReflection(const Daphnia& daphnia,
    const std::array<double, 4>& color_inner,
    const std::array<double, 4>& color_outer,
    const std::array<double, 4>& color_head) {
    glEnable(GL_STENCIL_TEST);

    glColorMask(0, 0, 0, 0); //disable drawing colors
    glStencilFunc(GL_ALWAYS, 1, 1); //make the stencil test always pass
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //set 1 to pixels that passed stencil test
    aquarium_.drawLowerPlane(); //set right plane pixels to 1

    glColorMask(1, 1, 1, 1); //enable drawing colors
    glStencilFunc(GL_EQUAL, 1, 1); //make pixel pass when pixel is 1 in stencil buffer
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //make the stencil buffer not change

    //draw reflection
    std::array<double, 3> aq_size = aquarium_.getSize();
    std::array<double, 3> coords = daphnia.getCoords();
    double diff = abs(abs(0.5 * aq_size[1]) - abs(coords[1]));
    daphnia.drawComplicatedReflection(false,
        { coords[0], 0.5 * aq_size[1] + diff, coords[2] },
        color_inner, color_outer, color_head);

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

////////// controlSpec //////////
void Scene::controlSpec(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        test_y -= 1;
    }
    if (key == GLUT_KEY_RIGHT) {
        test_y += 1;
    }
    if (key == GLUT_KEY_UP) {
        test_x += 1;
    }
    if (key == GLUT_KEY_DOWN) {
        test_x -= 1;
    }

    random_clip_objects_[0][0].setAngles({ test_y, test_x, test_z });
    std::array<double, 3> dir = random_clip_objects_[0][0].calcDirection();
    random_clip_objects_[0][0].setDirection(dir);
    std::cout << test_y << " " << test_x << " " << test_z << std::endl;
}

////////// controlKey //////////
void Scene::controlKey(unsigned char key, int x, int y) {
    if (key == 'd') {
        test_z -= 1;
    }
    if (key == 'a') {
        test_z += 1;
    }
    random_clip_objects_[0][0].setAngles({ test_y, test_x, test_z });
    std::array<double, 3> dir = random_clip_objects_[0][0].calcDirection();
    random_clip_objects_[0][0].setDirection(dir);
    std::cout << test_y << " " << test_x << " " << test_z << std::endl;
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
    aquarium_.drawWire();

    //write to file
    saveImage(viewport[2], viewport[3], grid_filename_);

    glutSwapBuffers();
    glutPostRedisplay();

    glutLeaveMainLoop();
}

////////// displayUntexturedRandomClip //////////
void Scene::displayUntexturedRandomClip() {
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
        daphnia.drawSimplified({ 0., 0., 0., 1. });
        drawSimplifiedReflection(daphnia, { 0., 0., 0., reflection_strength_ });
    }

    //aquarium
    glColor3d(0., 0., 0.);
    aquarium_.drawWire();

    //write to file
    saveImage(viewport[2], viewport[3], generation_frames_path_
        + std::to_string(frame_count_) + generation_frames_ending_);
    ++frame_count_;

    glutSwapBuffers();
    glutPostRedisplay();

    if (frame_count_ == random_clip_objects_.size())
        glutLeaveMainLoop();
}

////////// displayTexturedRandomClip //////////
void Scene::displayTexturedRandomClip() {
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

    //aquarium
    glColor3d(1., 1., 1.);
    aquarium_.drawTextured();

    //objects
    for (auto& daphnia : random_clip_objects_[frame_count_]) {
        drawSimplifiedReflection(daphnia, { 0.3, 0.3, 0.3, reflection_strength_ });
        daphnia.drawTextured();
        //daphnia.drawSimplified({ 0., 0., 0., 0.3 });
    }

    //write to file
    saveImage(viewport[2], viewport[3], generation_frames_path_
        + std::to_string(frame_count_) + generation_frames_ending_);
    ++frame_count_;

    glutSwapBuffers();
    glutPostRedisplay();

    if (frame_count_ == random_clip_objects_.size())
        glutLeaveMainLoop();
}

////////// displayObjectMaskRandomClip //////////
void Scene::displayObjectMaskRandomClip() {
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

    //object
    for (auto& daphnia : random_clip_objects_[frame_count_])
        daphnia.drawSimplified({1, 1, 1, 1}, false);

    //write to file
    saveImage(viewport[2], viewport[3], generation_objects_masks_path_
        + std::to_string(frame_count_) + generation_frames_ending_);
    ++frame_count_;

    glutSwapBuffers();
    glutPostRedisplay();

    if (frame_count_ == random_clip_objects_.size())
        glutLeaveMainLoop();
}

////////// displayReflectionMaskRandomClip //////////
void Scene::displayReflectionMaskRandomClip() {
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

    //object
    for (auto& daphnia : random_clip_objects_[frame_count_])
        drawSimplifiedReflection(daphnia, { 1, 1, 1, 1 });

    //write to file
    saveImage(viewport[2], viewport[3], generation_reflections_masks_path_
        + std::to_string(frame_count_) + generation_frames_ending_);
    ++frame_count_;

    glutSwapBuffers();
    glutPostRedisplay();

    if (frame_count_ == random_clip_objects_.size())
        glutLeaveMainLoop();
}

////////// displayTexturedSequentClip //////////
void Scene::displayTexturedSequentClip() {
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

    //aquarium
    glColor3d(1., 1., 1.);
    aquarium_.drawTextured();

    //objects
    for (auto& daphnia : sequent_clip_objects_[frame_count_]) {
        drawSimplifiedReflection(daphnia,
            { 0.3, 0.3, 0.3, reflection_strength_ }, //right
            { 0.3, 0.3, 0.3, reflection_strength_ }, //left
            { 0.55, 0.55, 0.55, reflection_strength_ }, //upper
            { 0.55, 0.55, 0.55, reflection_strength_ }); //lower
        daphnia.drawTextured();
        //daphnia.drawSimplified({ 0., 0., 0., 1. });
    }

    //write to file
    saveImage(viewport[2], viewport[3], generation_frames_path_
        + std::to_string(frame_count_) + generation_frames_ending_);
    ++frame_count_;

    glutSwapBuffers();
    glutPostRedisplay();

    if (frame_count_ == sequent_clip_objects_.size())
        glutLeaveMainLoop();
}

////////// displayObjectMaskSequentClip //////////
void Scene::displayObjectMaskSequentClip() {
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
    for (auto& daphnia : sequent_clip_objects_[frame_count_])
        daphnia.drawSimplified({ 1, 1, 1, 1 }, false);

    //write to file
    saveImage(viewport[2], viewport[3], generation_objects_masks_path_
        + std::to_string(frame_count_) + generation_frames_ending_);
    ++frame_count_;

    glutSwapBuffers();
    glutPostRedisplay();

    if (frame_count_ == sequent_clip_objects_.size())
        glutLeaveMainLoop();
}

////////// displayReflectionMaskSequentClip //////////
void Scene::displayReflectionMaskSequentClip() {
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
    for (auto& daphnia : sequent_clip_objects_[frame_count_])
        drawSimplifiedReflection(daphnia, { 1, 1, 1, 1 });

    //write to file
    saveImage(viewport[2], viewport[3], generation_reflections_masks_path_
        + std::to_string(frame_count_) + generation_frames_ending_);
    ++frame_count_;

    glutSwapBuffers();
    glutPostRedisplay();

    if (frame_count_ == sequent_clip_objects_.size())
        glutLeaveMainLoop();
}
