#include "daphnia.h"

////////// getCoords //////////
std::array<double, 3> Daphnia::getCoords() {
	return coords_;
}

////////// getCoords //////////
std::array<double, 3> Daphnia::getCoords() const {
	return coords_;
}

////////// getLength //////////
double Daphnia::getLength() {
	return radius_ * length_ratio_ * scale_;
}

////////// setCoords //////////
void Daphnia::setCoords(const std::array<double, 3>& coords) {
	coords_ = coords;
}

////////// setAngles //////////
void Daphnia::setAngles(const std::array<double, 3>& angles) {
	angles_ = angles;
}

////////// setScale //////////
void Daphnia::setScale(const double& scale) {
	scale_ = scale;
}

////////// drawSimplified //////////
void Daphnia::drawSimplified(const std::array<double, 4>& color4d) {
	glPushMatrix();

		glColor4dv(color4d.data());
		glTranslated(coords_[0], coords_[1], coords_[2]);
		glRotated(angles_[0], 1, 0, 0);
		glRotated(angles_[1], 0, 1, 0);
		glRotated(angles_[2], 0, 0, 1);

		glPushMatrix();
			glScaled(scale_, scale_, scale_);
			glScaled(length_ratio_, 1., 1.);
			glutSolidSphere(radius_, 30, 30);
		glPopMatrix();

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
		glRotated(angles_[0], 1, 0, 0);
		glRotated(angles_[1], 0, 1, 0);
		glRotated(angles_[2], 0, 0, 1);

		glPushMatrix();
			glScaled(scale_, scale_, scale_);
			glScaled(length_ratio_, 1., 1.);
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
		glRotated(angles_[0], 1, 0, 0);
		glRotated(angles_[1], 0, 1, 0);
		glRotated(angles_[2], 0, 0, 1);
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
