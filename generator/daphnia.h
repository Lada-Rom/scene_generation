#ifndef DAPHNIA_DAPHNIA_H_20221605
#define DAPHNIA_DAPHNIA_H_20221605

#include <array>
#include <GL/freeglut.h>

class Daphnia {
public:
	Daphnia() = default;
	~Daphnia() = default;

	std::array<double, 3> getCoords();
	double getLength();

	void setCoords(const std::array<double, 3>&);
	void setAngles(const std::array<double, 3>&);
	void setScale(const double&);

	void draw();

private:
	std::array<double, 3> coords_{ };
	std::array<double, 3> angles_{ };

	double scale_{ 0.5 }; // 0.5 - 0.8
	double radius_{ 0.1 };
	double length_scale_{ 1.5 };
};

#endif
