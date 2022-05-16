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

////////// draw //////////
void Aquarium::draw() {
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
