#ifndef DAPHNIA_DAPHNIA_H_20221605
#define DAPHNIA_DAPHNIA_H_20221605

#include <array>

#include "texture.h"
#include "utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <GL/freeglut.h>

class Daphnia {
public:
	Daphnia() = default;
	~Daphnia() = default;

	std::array<double, 3> getCoords();
	std::array<double, 3> getCoords() const;
	std::array<double, 3> getAngles();
	std::array<double, 3> getAngles() const;
	double getLength();
	std::array<double, 3> getDirection();
	std::array<double, 3> getDirection() const;
	std::string getTextureSourceFilename();

	void setCoords(const std::array<double, 3>& coords);
	void setAngles(const std::array<double, 3>& angles);
	void setDirection(const std::array<double, 3>& direction);
	void setScale(const double& scale);
	void setTextureFilename(const std::string& filename);
	void setTextureSourceFilename(const std::string& filename);

	std::array<double, 3> calcDirection();

	void drawSimplified(const std::array<double, 4>& color4d);
	void drawComplicated(
		const std::array<double, 4>& color_inner,
		const std::array<double, 4>& color_outer,
		const std::array<double, 4>& color_head);
	void drawTextured();
	void drawSimplifiedReflection(bool horizontal,
		const std::array<double, 3>& coords,
		const std::array<double, 4>& color4d) const;
	void drawComplicatedReflection(bool horizontal,
		const std::array<double, 3>& coords,
		const std::array<double, 4>& color_inner,
		const std::array<double, 4>& color_outer,
		const std::array<double, 4>& color_head) const;

private:
	std::array<double, 3> coords_{ 0, 0, 0 };
	std::array<double, 3> angles_{ 0, 0, 0 }; //yaw, pitch, roll
	std::array<double, 3> direction_{ 1, 0, 0 };

	glm::dmat4 rotation_{ };

	const std::array<double, 3> default_direction_{ 1, 0, 0 };

	GLUquadric* sphere_{ gluNewQuadric() };
	Texture texture_{ };
	std::string texture_source_filename_{  };

	double scale_{ 0.5 }; // 0.5 - 0.8
	double scale_body_{ 1.5 };
	double scale_head_{ 0.7 };

	double length_ratio_{ 2.0 };
	double head_ratio_{ 1.6 };

	double radius_{ 0.1 };
};

#endif
