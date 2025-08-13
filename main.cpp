#include <bits/stdc++.h>
using namespace std;

struct Moment {
    double option_price;
    double underlying;
    double shares;
    double bank_bal;
};

// time values (time_to_expiry, risk_free_rate, volatility) should be in the same units
double calc_bin(
    double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol, int steps
) {
    double timestep = time_to_expiry / steps;
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
        double option_price = max(underlying - strike, 0.0);
        double shares = 1;
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

double calc_vol_from_desired_u(double u, double timestep) {
    return log(u) / sqrt(timestep);
}

double normal_cdf(double x) {
    return 0.5 * erfc(-x * sqrt(0.5));
}

double calc_bs(
    double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol
) {
    double d1 = (log(start_ulying / strike) + time_to_expiry * (risk_free_rate + 0.5 * pow(vol, 2))) / (vol * sqrt(time_to_expiry));
    double a = start_ulying * normal_cdf(d1);
    double b = exp(-risk_free_rate * time_to_expiry) * strike * normal_cdf(d1 - vol * sqrt(time_to_expiry));
    double bs_price = a - b;
    return bs_price;
}

double simulate_monte_carlo(double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol, int steps) {
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
    double final_val = max(0.0, s - strike);
    double discounted = final_val * exp(-risk_free_rate * time_to_expiry);
    return discounted;
}

double calc_monte_carlo(double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol, int steps, int num_sims) {
    double total = 0;
    for(int i = 0; i < num_sims; i++) {
        total += simulate_monte_carlo(start_ulying, strike, time_to_expiry, risk_free_rate, vol, steps);
    }
    double mean = total / num_sims;
    return mean;
}

void compare_prices() {
    double start_ulying = 100;
    double strike = 95;
    double time_to_expiry = 1;
    double risk_free_rate = 0.015;
    double vol = 0.32;

    double bs_price = calc_bs(start_ulying, strike, time_to_expiry, risk_free_rate, vol);
    cout << "Black Scholes Price:\t" << bs_price << '\n';

    int bin_steps = 10'000;
    double bin_price = calc_bin(start_ulying, strike, time_to_expiry, risk_free_rate, vol, bin_steps);
    cout << "Binomial Tree Price:\t" << bin_price << '\n';

    int monte_carlo_steps = 100;
    int monte_carlo_num_sims = 1'000'000;
    double monte_carlo_price = calc_monte_carlo(start_ulying, strike, time_to_expiry, risk_free_rate, vol, monte_carlo_steps, monte_carlo_num_sims);
    cout << "Monte Carlo Price:\t" << monte_carlo_price << '\n';
}

int main() {
    compare_prices();
    return 0;
}
