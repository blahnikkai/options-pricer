#pragma once

namespace simulation {
    double calc_black_scholes(
        double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol
    );

    double calc_binary(
        double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol, int steps
    );

    double calc_monte_carlo(
        double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol, int steps, int num_sims
    );

    double simulate_one_monte_carlo_trial(
        double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol, int steps
    );
}
