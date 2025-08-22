#include <cmath>
#include "utils.h"

double utils::normal_cdf(double x) {
    return 0.5 * erfc(-x * std::sqrt(0.5));
}

double utils::u_to_vol(double u, double timestep) {
    return std::log(u) / std::sqrt(timestep);
}

