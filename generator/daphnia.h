#ifndef DAPHNIA_DAPHNIA_H_20221605
#define DAPHNIA_DAPHNIA_H_20221605

#include <array>
#include <GL/freeglut.h>

class Daphnia {
public:
	Daphnia() = default;
	~Daphnia() = default;

	std::array<double, 3> getCoords();

	void setCoords(const std::array<double, 3>&);
	void setAngles(const std::array<double, 3>&);

	void draw();

private:
	std::array<double, 3> coords_{ };
	std::array<double, 3> angles_{ };
};

#endif
