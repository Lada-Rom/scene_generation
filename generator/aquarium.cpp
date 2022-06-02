#include "aquarium.h"

////////// Aquarium //////////
Aquarium::Aquarium() {
    calcVerticies();
}

////////// getSize //////////
std::array<double, 3> Aquarium::getSize() {
	return size_;
}

////////// getVerticies //////////
std::array<std::array<double, 3>, 8> Aquarium::getVertices() {
    return vertices_;
}

////////// getObjpoints //////////
std::vector<cv::Point3d> Aquarium::getObjpoints() {
    std::vector<cv::Point3d> objpoints;
    for (const auto& point : vertices_) {
        objpoints.push_back(cv::Point3d(point[0], point[1], point[2]));
    }
    return objpoints;
}

////////// getObjpoints //////////
std::vector<cv::Point3d> Aquarium::getObjpoints(double scale) {
    std::vector<cv::Point3d> objpoints;
    for (const auto& point : vertices_) {
        objpoints.push_back(cv::Point3d(
            scale * point[0], scale * point[1], scale * point[2]));
    }
    return objpoints;
}

////////// setSize //////////
void Aquarium::setSize(const std::array<double, 3>& size) {
    size_ = size;
    calcVerticies();
}

////////// setTextureFilename //////////
void Aquarium::setTextureFilename(
    const std::string& plane, const std::string& filename) {
    plane_textures_[plane].filename_ = filename;
}

////////// calcVerticies //////////
void Aquarium::calcVerticies() {
    vertices_ = {
        std::array<double, 3>{ -0.5 * size_[0], -0.5 * size_[1], 0. },
        std::array<double, 3>{  0.5 * size_[0], -0.5 * size_[1], 0. },
        std::array<double, 3>{  0.5 * size_[0],  0.5 * size_[1], 0. },
        std::array<double, 3>{ -0.5 * size_[0],  0.5 * size_[1], 0. },
        std::array<double, 3>{ -0.5 * size_[0], -0.5 * size_[1], -size_[2] },
        std::array<double, 3>{  0.5 * size_[0], -0.5 * size_[1], -size_[2] },
        std::array<double, 3>{  0.5 * size_[0],  0.5 * size_[1], -size_[2] },
        std::array<double, 3>{ -0.5 * size_[0],  0.5 * size_[1], -size_[2] },
    };
}

////////// drawWire //////////
void Aquarium::drawWire() {
    glPushMatrix();

        // near surface
        glBegin(GL_LINE_LOOP);
            glVertex3dv(&vertices_[0][0]);
            glVertex3dv(&vertices_[1][0]);
            glVertex3dv(&vertices_[2][0]);
            glVertex3dv(&vertices_[3][0]);
        glEnd();

        // far surface
        glBegin(GL_LINE_LOOP);
            glVertex3dv(&vertices_[4][0]);
            glVertex3dv(&vertices_[5][0]);
            glVertex3dv(&vertices_[6][0]);
            glVertex3dv(&vertices_[7][0]);
        glEnd();

        // side edges
        glBegin(GL_LINES);
            glVertex3dv(&vertices_[0][0]);
            glVertex3dv(&vertices_[4][0]);

            glVertex3dv(&vertices_[1][0]);
            glVertex3dv(&vertices_[5][0]);

            glVertex3dv(&vertices_[2][0]);
            glVertex3dv(&vertices_[6][0]);

            glVertex3dv(&vertices_[3][0]);
            glVertex3dv(&vertices_[7][0]);
        glEnd();

    glPopMatrix();
}

////////// drawTextured //////////
void Aquarium::drawTextured() {
    for (auto& texture : plane_textures_)
        loadTexture(texture.second);

    drawTexturedRightPlane();
    drawTexturedLeftPlane();
    drawTexturedUpperPlane();
    drawTexturedLowerPlane();
    drawTexturedBottomPlane();
}

////////// drawRightPlane //////////
void Aquarium::drawRightPlane() {
    glPushMatrix();
        glBegin(GL_QUADS);
            glVertex3dv(vertices_[2].data());
            glVertex3dv(vertices_[1].data());
            glVertex3dv(vertices_[5].data());
            glVertex3dv(vertices_[6].data());
        glEnd();
    glPopMatrix();
}

////////// drawLeftPlane //////////
void Aquarium::drawLeftPlane() {
    glPushMatrix();
        glBegin(GL_QUADS);
            glVertex3dv(vertices_[0].data());
            glVertex3dv(vertices_[3].data());
            glVertex3dv(vertices_[7].data());
            glVertex3dv(vertices_[4].data());
        glEnd();
    glPopMatrix();
}

////////// drawUpperPlane //////////
void Aquarium::drawUpperPlane() {
    glPushMatrix();
        glBegin(GL_QUADS);
            glVertex3dv(vertices_[3].data());
            glVertex3dv(vertices_[2].data());
            glVertex3dv(vertices_[6].data());
            glVertex3dv(vertices_[7].data());
        glEnd();
    glPopMatrix();
}

////////// drawLowerPlane //////////
void Aquarium::drawLowerPlane() {
    glPushMatrix();
        glBegin(GL_QUADS);
            glVertex3dv(vertices_[1].data());
            glVertex3dv(vertices_[0].data());
            glVertex3dv(vertices_[4].data());
            glVertex3dv(vertices_[5].data());
        glEnd();
    glPopMatrix();
}

////////// drawTexturedRightPlane //////////
void Aquarium::drawTexturedRightPlane() {
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, plane_textures_["right"].id_);
    glPushMatrix();
        glBegin(GL_QUADS);
            glTexCoord2d(0., 0.); glVertex3dv(vertices_[2].data());
            glTexCoord2d(1., 0.); glVertex3dv(vertices_[1].data());
            glTexCoord2d(1., 1.); glVertex3dv(vertices_[5].data());
            glTexCoord2d(0., 1.); glVertex3dv(vertices_[6].data());
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

////////// drawTexturedLeftPlane //////////
void Aquarium::drawTexturedLeftPlane() {
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, plane_textures_["left"].id_);
    glPushMatrix();
        glBegin(GL_QUADS);
            glTexCoord2d(0., 0.); glVertex3dv(vertices_[0].data());
            glTexCoord2d(1., 0.); glVertex3dv(vertices_[3].data());
            glTexCoord2d(1., 1.); glVertex3dv(vertices_[7].data());
            glTexCoord2d(0., 1.); glVertex3dv(vertices_[4].data());
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

////////// drawTexturedUpperPlane //////////
void Aquarium::drawTexturedUpperPlane() {
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, plane_textures_["upper"].id_);
    glPushMatrix();
        glBegin(GL_QUADS);
            glTexCoord2d(0., 0.); glVertex3dv(vertices_[3].data());
            glTexCoord2d(1., 0.); glVertex3dv(vertices_[2].data());
            glTexCoord2d(1., 1.); glVertex3dv(vertices_[6].data());
            glTexCoord2d(0., 1.); glVertex3dv(vertices_[7].data());
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

////////// drawTexturedLowerPlane //////////
void Aquarium::drawTexturedLowerPlane() {
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, plane_textures_["lower"].id_);
    glPushMatrix();
        glBegin(GL_QUADS);
            glTexCoord2d(0., 0.); glVertex3dv(vertices_[1].data());
            glTexCoord2d(1., 0.); glVertex3dv(vertices_[0].data());
            glTexCoord2d(1., 1.); glVertex3dv(vertices_[4].data());
            glTexCoord2d(0., 1.); glVertex3dv(vertices_[5].data());
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

////////// drawTexturedBottomPlane //////////
void Aquarium::drawTexturedBottomPlane() {
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, plane_textures_["bottom"].id_);
    glPushMatrix();
        glBegin(GL_QUADS);
            glTexCoord2d(0., 0.); glVertex3dv(vertices_[7].data());
            glTexCoord2d(1., 0.); glVertex3dv(vertices_[6].data());
            glTexCoord2d(1., 1.); glVertex3dv(vertices_[5].data());
            glTexCoord2d(0., 1.); glVertex3dv(vertices_[4].data());
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
