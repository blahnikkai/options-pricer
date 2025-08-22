#include <iostream>
#include "simulation.h"

void compare_prices() {

    double start_ulying = 100;
    double strike = 95;
    double time_to_expiry = 1;
    double risk_free_rate = 0.015;
    double vol = 0.32;

    double bs_price = simulation::calc_black_scholes(start_ulying, strike, time_to_expiry, risk_free_rate, vol);
    std::cout << "Black Scholes Price:\t" << bs_price << '\n';

    int bin_steps = 10'000;
    double bin_price = simulation::calc_binary(start_ulying, strike, time_to_expiry, risk_free_rate, vol, bin_steps);
    std::cout << "Binomial Tree Price:\t" << bin_price << '\n';

    int monte_carlo_steps = 100;
    int monte_carlo_num_sims = 1'000'000;
    double monte_carlo_price = simulation::calc_monte_carlo(start_ulying, strike, time_to_expiry, risk_free_rate, vol, monte_carlo_steps, monte_carlo_num_sims);
    std::cout << "Monte Carlo Price:\t" << monte_carlo_price << '\n';
}

int main() {
    compare_prices();
    return 0;
}
