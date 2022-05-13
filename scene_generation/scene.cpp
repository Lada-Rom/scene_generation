#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS

#include "scene.h"


void Scene::initializeGLUT() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    //glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void Scene::displayContent() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glClear(GL_COLOR_BUFFER_BIT);

    // camera pose
    glLoadIdentity();
    gluLookAt(eye_[0], eye_[1], eye_[2], center_[0], center_[1], center_[2], up_[0], up_[1], up_[2]);
    //glRotatef(180, 0, 0, 1);
    //gluLookAt(0.0, 0.0, camera_initial_z_, 0.0, 0.0, 0.0, 0.0, 1.0, 0);

    // background image
    if (draw_texture_) {
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBindTexture(GL_TEXTURE_2D, texture_id_);
        
        glRotatef(180, 0, 0, 1);
        glTranslatef(0.0, 0.0, -aquarium_.getDepth());
        double scale = 0.489;
        glScalef(scale, scale, 1);
        //std::cout << viewport[2] << " " << texture_height_ << " " << texture_height_ / viewport[2] << std::endl;
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0); glVertex3f(-texture_height_ * image_width_ / image_height_,  texture_height_, 0.0);
            glTexCoord2f(0.0, 1.0); glVertex3f(-texture_height_ * image_width_ / image_height_, -texture_height_, 0.0);
            glTexCoord2f(1.0, 1.0); glVertex3f( texture_height_ * image_width_ / image_height_, -texture_height_, 0.0);
            glTexCoord2f(1.0, 0.0); glVertex3f( texture_height_ * image_width_ / image_height_,  texture_height_, 0.0);
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    }

    // content
    glPushMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glRotatef(pitch_, 1, 0, 0);
        glRotatef(roll_, 0, 1, 0);
        glRotatef(yaw_, 0, 0, 1);
        glTranslatef(camera_x_pose_, 0, 0);
        glTranslatef(0, camera_y_pose_, 0);
        glTranslatef(0, 0, -camera_distance_);
        if (draw_aquarium_)
            glColor3f(1, 0, 0.5);
        else
            glColor3f(1, 1, 1);
        aquarium_.draw(false, true);             // aquarium
        glColor3f(0, 0, 0);
        for (Daphnia& daphnia : daphnias_) {    // daphnias
            daphnia.draw();
        }
    glPopMatrix();
    
    // refreshing camera angle
    if (camera_new_angle_ != camera_prev_angle_) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(camera_new_angle_, (GLfloat)viewport[2] / (GLfloat)viewport[3], z_near_, z_far_);
        glMatrixMode(GL_MODELVIEW);
        camera_prev_angle_ = camera_new_angle_;
        float k = std::tan(0.5 * camera_new_angle_ * pi / 180);
        float z = camera_initial_z_ - z_far_;
        float b = camera_initial_z_ * std::tan(0.5 * camera_new_angle_ * pi / 180);
        texture_height_ = -k * z + b;
    }

    //static int count = 0;
    //if (count == 0) {
    //    std::string filename = "../data/" + std::to_string(count) + ".png";
    //    saveImage(viewport[2], viewport[3], filename);
    //}

    glutSwapBuffers();
    glutPostRedisplay();
}

void Scene::reshapeContent(unsigned int width, unsigned height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera_new_angle_, (GLfloat)width / (GLfloat)height, z_near_, z_far_);
    glMatrixMode(GL_MODELVIEW);
    float k = std::tan(0.5 * camera_new_angle_ * pi / 180);
    float z = camera_initial_z_ - z_far_;
    float b = camera_initial_z_ * std::tan(0.5 * camera_new_angle_ * pi / 180);
    texture_height_ = -k * z + b;
}

void Scene::keyboardControl(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        pitch_ += 1;
        break;
    case 's':
        pitch_ -= 1;
        break;
    case 'a':
        roll_ += 1;
        break;
    case 'd':
        roll_ -= 1;
        break;
    case 'q':
        yaw_ -= 1;
        break;
    case 'e':
        yaw_ += 1;
        break;
    case 't':
        camera_y_pose_ += 0.1;
        break;
    case 'g':
        camera_y_pose_ -= 0.1;
        break;
    case 'f':
        camera_x_pose_ -= 0.1;
        break;
    case 'h':
        camera_x_pose_ += 0.1;
        break;
    case (char)32:
        std::cout << " yaw, pitch, roll: " << yaw_ << " " << pitch_ << " " << roll_ << std::endl;
        std::cout << " camera x, y, z pose: " << camera_x_pose_ << " " << camera_y_pose_ << " " << camera_distance_ << std::endl;
        std::cout << " camera view angle: " << camera_new_angle_ << std::endl;
        std::cout << std::endl;
    default:
        break;
    }
}

void Scene::arrowControl(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        camera_distance_ -= 0.1;
        break;
    case GLUT_KEY_DOWN:
        camera_distance_ += 0.1;
        break;
    case GLUT_KEY_LEFT:
        camera_new_angle_ -= 1;
        break;
    case GLUT_KEY_RIGHT:
        camera_new_angle_ += 1;
        break;
    default:
        break;
    }
}

void Scene::generateDaphnias(size_t quantity, float radius) {
    float width = aquarium_.getWidth();
    float height = aquarium_.getHeight();
    float depth = aquarium_.getDepth();

    std::uniform_real_distribution<> x_distribution(-0.5 * width + radius, 0.5 * width - radius);
    std::uniform_real_distribution<> y_distribution(-0.5 * height + radius, 0.5 * height - radius);
    std::uniform_real_distribution<> z_distribution(-2 * depth + radius, 0 - radius);
    //std::cout << width << " " << height << " " << depth << std::endl;

    float x;
    float y;
    float z;
    for (size_t i{ }; i < quantity; ++i) {
        x = x_distribution(random_device_);
        y = y_distribution(random_device_);
        z = z_distribution(random_device_);
        //std::cout << x << " " << y << " " << z << std::endl;
        Daphnia new_daphnia{ x, y, z, radius };
        new_daphnia.setRange(0.5 * width, 0.5 * height, 0.5 * depth);
        daphnias_.push_back(new_daphnia);
    }
}

void Scene::loadTexture(const std::string& filename) {
    unsigned char *data = stbi_load(filename.c_str(), &image_width_, &image_height_, &image_comp_, 3);
    if (!data)
        return;

    //create a texture
    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image_width_, image_height_, GL_RGB, GL_UNSIGNED_BYTE, data);
    //texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    free(data);
}

void Scene::saveImage(int width, int height, std::string& filepath) {
    unsigned char* data = (unsigned char*)malloc((int)(width * height * (3)));
    glReadPixels(0, 0, width, height,GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_flip_vertically_on_write(true);
    stbi_write_png(filepath.c_str(), width, height, 3, data, 0);
}

std::ostream& operator<<(std::ostream& ostrm, const glm::mat3& rhs) {
    ostrm << "{ { " << rhs[0][0] << " " << rhs[0][1] << " " << rhs[0][2] << " },\n"
          << "  { " << rhs[1][0] << " " << rhs[1][1] << " " << rhs[1][2] << " },\n"
          << "  { " << rhs[2][0] << " " << rhs[2][1] << " " << rhs[2][2] << " } }";
    return ostrm;
}

void Scene::calcLookAtArray() {
    glm::mat3 rotation_i = glm::inverse(rotation_);
    glm::mat3 rotation_t = glm::transpose(rotation_i);
    //std::cout << rotation << "\n" << std::endl;
    //std::cout << glm::transpose(rotation) << "\n" << std::endl;
    //std::cout << glm::inverse(rotation) << "\n" << std::endl;
    //std::cout << glm::inverse(glm::transpose(rotation)) << "\n" << std::endl;
    //std::cout << glm::transpose(glm::inverse(rotation)) << "\n" << std::endl;

    //eye
    eye_ = translation_;
    //eye_[2] = -eye_[2];
    std::cout << eye_[0] << " " << eye_[1] << " " << eye_[2] << std::endl;

    //center
    glm::vec3 view = {0, 0, eye_[2]};
    center_[0] = view[0] / view[2] * (-eye_[2]) + eye_[0];
    center_[1] = view[1] / view[2] * (-eye_[2]) + eye_[1];
    center_[2] = 0;
    std::cout << center_[0] << " " << center_[1] << " " << center_[2] << std::endl;

    //up
    up_ = glm::normalize(rotation_t * up_);
    std::cout << up_[0] << " " << up_[1] << " " << up_[2] << std::endl;
}