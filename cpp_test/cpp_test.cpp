#include <iostream>
#include <random>

std::piecewise_linear_distribution<double> triangular_distribution(double min, double peak, double max) {
    std::vector<double> x{ min, peak, max };
    std::vector<double> y{ 0, 1, 0 };
    return std::piecewise_linear_distribution<double>(x.begin(), x.end(), y.begin());
}

int main() {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::piecewise_linear_distribution<double> dist = triangular_distribution(-0.005, 0, 0.005);
    for (int i{}; i < 10; ++i)
        std::cout << dist(gen) << std::endl;
}