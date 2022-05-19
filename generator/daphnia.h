#ifndef DAPHNIA_DAPHNIA_H_20221605
#define DAPHNIA_DAPHNIA_H_20221605

#include <array>
#include <GL/freeglut.h>

class Daphnia {
public:
	Daphnia() = default;
	~Daphnia() = default;

	std::array<double, 3> getCoords();
	std::array<double, 3> getCoords() const;
	double getLength();

	void setCoords(const std::array<double, 3>&);
	void setAngles(const std::array<double, 3>&);
	void setScale(const double&);

	void draw(const std::array<double, 4>& color4d);
	void drawReflection(bool horizontal,
		const std::array<double, 3>& coords,
		const std::array<double, 4>& color4d) const;

private:
	std::array<double, 3> coords_{ };
	std::array<double, 3> angles_{ };

	double scale_{ 0.5 }; // 0.5 - 0.8
	double radius_{ 0.1 };
	double length_scale_{ 1.5 };
};

#endif
