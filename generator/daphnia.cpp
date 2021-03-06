#include "daphnia.h"

////////// getCoords //////////
std::array<double, 3> Daphnia::getCoords() {
	return coords_;
}

////////// getCoords //////////
std::array<double, 3> Daphnia::getCoords() const {
	return coords_;
}

////////// getAngles //////////
std::array<double, 3> Daphnia::getAngles() {
	return angles_;
}

////////// getAngles //////////
std::array<double, 3> Daphnia::getAngles() const {
	return angles_;
}

////////// getLength //////////
double Daphnia::getLength() {
	return radius_ * length_ratio_ * scale_;
}

////////// getDirection //////////
std::array<double, 3> Daphnia::getDirection() {
	return direction_;
}

////////// getDirection //////////
std::array<double, 3> Daphnia::getDirection() const {
	return direction_;
}

////////// getTextureSourceFilename //////////
std::string Daphnia::getTextureSourceFilename() {
	return texture_source_filename_;
}

////////// setCoords //////////
void Daphnia::setCoords(const std::array<double, 3>& coords) {
	coords_ = coords;
}

////////// setAngles //////////
void Daphnia::setAngles(const std::array<double, 3>& angles) {
	angles_ = angles;
	rotation_ = glm::eulerAngleYXZ(
		glm::radians(angles[0]), glm::radians(angles[1]), glm::radians(angles[2]));
}

////////// setDirection //////////
void Daphnia::setDirection(const std::array<double, 3>& direction) {
	direction_ = direction;
}

////////// setScale //////////
void Daphnia::setScale(const double& scale) {
	scale_ = scale;
}

////////// setTextureFilename //////////
void Daphnia::setTextureFilename(const std::string& filename) {
	texture_.filename_ = filename;
}

////////// setTextureSourceFilename //////////
void Daphnia::setTextureSourceFilename(const std::string& filename) {
	texture_source_filename_ = filename;
}

////////// calcDirection //////////
std::array<double, 3> Daphnia::calcDirection() {
	glm::dvec4 direction_default = glm::make_vec4(default_direction_.data());
	direction_default[3] = 1.;
	glm::dvec4 direction_4d = rotation_ * direction_default;
	direction_4d /= direction_4d.w;
	glm::dvec3 direction_3d = glm::normalize(
		glm::dvec3{ direction_4d.x, direction_4d.y, direction_4d.z });
	return { direction_3d.x, direction_3d.y, direction_3d.z };
}

////////// drawSimplified //////////
void Daphnia::drawSimplified(const std::array<double, 4>& color4d, bool direction) {
	glPushMatrix();
	glEnable(GL_BLEND);

		glTranslated(coords_[0], coords_[1], coords_[2]);

		if (direction) {
			glColor3d(1, 0, 0);
			glBegin(GL_LINES);
			glVertex3d(0, 0, 0);
			glVertex3d(direction_[0], direction_[1], direction_[2]);
			glEnd();
		}

		glMultMatrixd(&rotation_[0][0]);

		glColor4dv(color4d.data());
		glPushMatrix();
			glScaled(scale_, scale_, scale_);
			glScaled(length_ratio_, 1., 1.);
			glRotated(90, 0, 1, 0);
			glutSolidSphere(radius_, 30, 30);
		glPopMatrix();
	glDisable(GL_BLEND);
	glPopMatrix();
}

////////// drawComplicated //////////
void Daphnia::drawComplicated(
	const std::array<double, 4>& color_inner,
	const std::array<double, 4>& color_outer,
	const std::array<double, 4>& color_head) {

	glPushMatrix();
	glEnable(GL_BLEND);
		glTranslated(coords_[0], coords_[1], coords_[2]);
		glMultMatrixd(&rotation_[0][0]);
		glScaled(scale_, scale_, scale_);

		glPushMatrix();
			glPushMatrix();
				glColor4dv(color_head.data());
				glTranslated(
					0.75 * radius_ * scale_body_ * length_ratio_,
					radius_ * scale_body_ - radius_, 0);
				glRotated(-40, 0, 0, 1);
				glScaled(scale_head_, scale_head_, scale_head_);
				glScaled(head_ratio_, 1., 1.);
				glutSolidSphere(radius_, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glColor4dv(color_outer.data());
				glScaled(scale_body_, scale_body_, scale_body_);
				glScaled(length_ratio_, 1., 1.);
				glutSolidSphere(radius_, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glColor4dv(color_inner.data());
				glTranslated(0, radius_ * scale_body_ - radius_, 0);
				glScaled(length_ratio_, 1., 1.);
				glutSolidSphere(radius_, 30, 30);
			glPopMatrix();
		glPopMatrix();
	glDisable(GL_BLEND);
	glPopMatrix();
}

////////// drawTextured //////////
void Daphnia::drawTextured(const std::array<double, 4> color) {
	glPushMatrix();
		glTranslated(coords_[0], coords_[1], coords_[2]);

		//glColor4d(1., 0., 0., 1.);
		//glBegin(GL_LINES);
		//glVertex3d(0., 0., 0.);
		//glVertex3d(direction_[0], direction_[1], direction_[2]);
		//glEnd();

		glMultMatrixd(&rotation_[0][0]);

		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);

		loadTexture(texture_);
		gluQuadricDrawStyle(sphere_, GLU_FILL);
		glBindTexture(GL_TEXTURE_2D, texture_.id_);
		gluQuadricTexture(sphere_, GL_TRUE);
		gluQuadricNormals(sphere_, GLU_SMOOTH);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glColor4dv(color.data());
		glPushMatrix();
			glScaled(scale_, scale_, scale_);
			glScaled(length_ratio_, 1., 1.);
			glRotated(90, 0, 1, 0);
			gluSphere(sphere_, radius_, 32, 32);
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	glPopMatrix();
}

////////// drawSimplifiedReflection //////////
void Daphnia::drawSimplifiedReflection(bool horizontal,
	const std::array<double, 3>& coords,
	const std::array<double, 4>& color4d) const {
	glPushMatrix();
	glEnable(GL_BLEND);

		glColor4dv(color4d.data());
		if (horizontal)
			glScaled(-1, 1, 1);
		else
			glScaled(1, -1, 1);
		glTranslated(coords[0], coords[1], coords[2]);
		glMultMatrixd(&rotation_[0][0]);

		glPushMatrix();
			glScaled(scale_, scale_, scale_);
			glScaled(length_ratio_, 1., 1.);
			glRotated(90, 0, 1, 0);
			glutSolidSphere(radius_, 30, 30);
		glPopMatrix();

	glDisable(GL_BLEND);
	glPopMatrix();
}

////////// drawComplicatedReflection //////////
void Daphnia::drawComplicatedReflection(bool horizontal,
	const std::array<double, 3>& coords,
	const std::array<double, 4>& color_inner,
	const std::array<double, 4>& color_outer,
	const std::array<double, 4>& color_head) const {
	glPushMatrix();
	glEnable(GL_BLEND);

		if (horizontal)
			glScaled(-1, 1, 1);
		else
			glScaled(1, -1, 1);
		glTranslated(coords[0], coords[1], coords[2]);
		glMultMatrixd(&rotation_[0][0]);
		glScaled(scale_, scale_, scale_);

		glPushMatrix();
			glPushMatrix();
				glColor4dv(color_head.data());
				glTranslated(
					0.75 * radius_ * scale_body_ * length_ratio_,
					radius_ * scale_body_ - radius_, 0);
				glRotated(-40, 0, 0, 1);
				glScaled(scale_head_, scale_head_, scale_head_);
				glScaled(head_ratio_, 1., 1.);
				glutSolidSphere(radius_, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glColor4dv(color_outer.data());
				glScaled(scale_body_, scale_body_, scale_body_);
				glScaled(length_ratio_, 1., 1.);
				glutSolidSphere(radius_, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glColor4dv(color_inner.data());
				glTranslated(0, radius_ * scale_body_ - radius_, 0);
				glScaled(length_ratio_, 1., 1.);
				glutSolidSphere(radius_, 30, 30);
			glPopMatrix();
		glPopMatrix();

	glDisable(GL_BLEND);
	glPopMatrix();
}
