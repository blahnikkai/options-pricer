#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include "pricing.h"
#include "utils.h"
#include "Moment.h"

using std::vector, std::sqrt, std::max, std::cout;
using std::random_device, std::mt19937, std::normal_distribution;

// time values (time_to_expiry, risk_free_rate, volatility) should be in the same units
double pricing::calc_black_scholes(
    double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol, bool call
) {

    double d1 = (log(start_ulying / strike) + time_to_expiry * (risk_free_rate + 0.5 * pow(vol, 2))) / (vol * sqrt(time_to_expiry));
    double d2 = d1 - vol * sqrt(time_to_expiry);
    if(!call) {
        d1 *= -1;
        d2 *= -1;
    }

    double a = start_ulying * utils::normal_cdf(d1);
    double b = strike * exp(-risk_free_rate * time_to_expiry) * utils::normal_cdf(d2);

    double bs_price = a - b;
    if(!call) {
        bs_price *= -1;
    }

    return bs_price;
}

double pricing::calc_binomial(
    double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol, int steps, bool call, bool european
) {
    // 1 for call, -1 for put, used to negate
    double put_call_negater = 2 * call - 1;
    double timestep = time_to_expiry / steps;
    // u = up move
    // d = 1 / u = down move
    double u = exp(vol * sqrt(timestep));
    double d = 1 / u;
    vector<vector<Moment>> moments;
    vector<Moment> final_moments;

    // continuous
    double lambda = exp(timestep * risk_free_rate);
    // discrete
    // double lambda = pow(1 + risk_free_rate, timestep);

    double q = (lambda * u - 1) / (u * u - 1);
    for(int i = 0; i <= steps; i++) {
        int worst = -steps;
        double underlying = start_ulying * pow(u, worst + 2 * i);
        double exercise_val = put_call_negater * (underlying - strike);
        // since exercise is optional, we would only exercise if it benefits us (positive value)
        double option_price = max(0.0, exercise_val);
        double shares = 1;
        // at expiry,
        // you will have 1 of the underlying
        // + whatever bank balance necessary to match the option price exactly (perfect hedging)
        double bank_bal = option_price - underlying;
        final_moments.push_back({option_price, underlying, shares, bank_bal});
    }
    moments.push_back(final_moments);
    for(int i = 0; i < steps; i++) {
        vector<Moment> moment_row;
        for(int j = 0; j < steps - i; j++) {
            Moment d_moment = moments.at(i).at(j);
            Moment u_moment = moments.at(i).at(j + 1);
            double option_price =  1 / lambda * (q * u_moment.option_price + (1 - q) * d_moment.option_price);
            double underlying = 1 / lambda * (q * u_moment.underlying + (1 - q) * d_moment.underlying);
            if(!european) {
                // for an American option, if the exercise value of the option is higher
                // than the value of holding the option to expiry, then exercise it immediately
                double exercise_val = put_call_negater * (underlying - strike);
                option_price = max(option_price, exercise_val);
            }
            double m = u_moment.option_price;
            double n = d_moment.option_price;
            double shares = (m - n) / (underlying * (u - d));
            double bank_bal = (u * n - m * d) / (lambda * (u - d));
            moment_row.push_back({option_price, underlying, shares, bank_bal});
        }
        moments.push_back(moment_row);
    }
    double calculated_price = moments.at(moments.size() - 1).at(0).option_price;
    return calculated_price;
}

double pricing::calc_monte_carlo(
    double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol, int steps, int num_sims, bool call)
{
    double total = 0;
    for(int i = 0; i < num_sims; i++) {
        total += simulate_one_monte_carlo_trial(start_ulying, strike, time_to_expiry, risk_free_rate, vol, steps, call);
    }
    double mean = total / num_sims;
    return mean;
}

double pricing::simulate_one_monte_carlo_trial(double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol, int steps, bool call) {
    random_device rd{};
    mt19937 gen{rd()};
    double timestep = time_to_expiry / steps;
    normal_distribution<double> norm(0, sqrt(timestep));
    double s = start_ulying;
    for(int i = 0; i < steps; i++) {
        double dw = norm(gen);
        double ds = s * (risk_free_rate * timestep + vol * dw);
        s += ds;
    }
    double exercise_val = s - strike;
    if(!call) {
        exercise_val *= -1;
    }
    double final_val = max(0.0, exercise_val);
    double discounted = final_val * exp(-risk_free_rate * time_to_expiry);
    return discounted;
}
