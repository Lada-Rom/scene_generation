#ifndef GENERATOR_GENERATOR_H_20221305
#define GENERATOR_GENERATOR_H_20221305

#include <string>
#include <nlohmann/json.hpp>

class Generator {
public:
	Generator() = default;
	~Generator() = default;

private:
	const std::string generator_params{ "../../data/json/generator_params.json" };
};

#endif
