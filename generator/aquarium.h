#ifndef AQUARIUM_AQUARIUM_H_20221405
#define AQUARIUM_AQUARIUM_H_20221405

#include <array>

class Aquarium {
public:
	Aquarium() = default;
	~Aquarium() = default;

private:
	std::array<double, 3> size_{ 18.1, 15.0, 2.1 };
	const std::array<double, 3> default_size_{ 16.5, 14.5, 2.5};
};

#endif
