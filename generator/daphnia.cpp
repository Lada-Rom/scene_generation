#include "daphnia.h"

////////// getCoords //////////
std::array<double, 3> Daphnia::getCoords() {
	return coords_;
}

////////// getLength //////////
double Daphnia::getLength() {
	return radius_ * length_scale_ * scale_;
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

////////// draw //////////
void Daphnia::draw() {
	glPushMatrix();
		glTranslated(coords_[0], coords_[1], coords_[2]);
		glRotated(angles_[0], 1, 0, 0);
		glRotated(angles_[1], 0, 1, 0);
		glRotated(angles_[2], 0, 0, 1);

		glPushMatrix();
			glScaled(scale_, scale_, scale_);
			glScaled(length_scale_, 1., 1.);
			glutSolidSphere(radius_, 30, 30);
		glPopMatrix();

	glPopMatrix();
}
