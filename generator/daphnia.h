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

	void drawSimplified(const std::array<double, 4>& color4d);
	void drawComplicated(
		const std::array<double, 4>& color_inner,
		const std::array<double, 4>& color_outer,
		const std::array<double, 4>& color_head);
	void drawSimplifiedReflection(bool horizontal,
		const std::array<double, 3>& coords,
		const std::array<double, 4>& color4d) const;
	void drawComplicatedReflection(bool horizontal,
		const std::array<double, 3>& coords,
		const std::array<double, 4>& color_inner,
		const std::array<double, 4>& color_outer,
		const std::array<double, 4>& color_head) const;

private:
	std::array<double, 3> coords_{ };
	std::array<double, 3> angles_{ };

	double scale_{ 0.5 }; // 0.5 - 0.8
	double scale_body_{ 1.5 };
	double scale_head_{ 0.7 };

	double length_ratio_{ 1.5 };
	double head_ratio_{ 1.6 };

	double radius_{ 0.1 };
};

#endif
