#pragma once

namespace pricing {
    double calc_black_scholes(
        double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol, bool call = true
    );

    double calc_binomial(
        double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol, int steps, bool call = true, bool european = true
    );

    double calc_monte_carlo(
        double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol, int steps, int num_sims, bool call = true
    );

    double simulate_one_monte_carlo_trial(
        double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol, int steps, bool call
    );
}
