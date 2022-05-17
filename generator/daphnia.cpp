#include "daphnia.h"

////////// getCoords //////////
std::array<double, 3> Daphnia::getCoords() {
	return coords_;
}

////////// setCoords //////////
void Daphnia::setCoords(const std::array<double, 3>& coords) {
	coords_ = coords;
}

////////// setAngles //////////
void Daphnia::setAngles(const std::array<double, 3>& angles) {
	angles_ = angles;
}

////////// draw //////////
void Daphnia::draw() {
	glPushMatrix();
		glTranslated(coords_[0], coords_[1], coords_[2]);
		glRotated(angles_[0], 1, 0, 0);
		glRotated(angles_[1], 0, 1, 0);
		glRotated(angles_[2], 0, 0, 1);

		glPushMatrix();
			glScalef(0.5, 0.5, 0.5);
			glScalef(1.5, 1, 1);
			glutSolidSphere(0.1, 30, 30);
		glPopMatrix();

	glPopMatrix();
}
