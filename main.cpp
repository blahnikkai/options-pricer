#include <bits/stdc++.h>
using namespace std;

struct Moment {
    double option_price;
    double underlying;
    double shares;
    double bank_bal;
};

// time values (time_to_expiry, risk_free_rate, volatility) should be in the same units
double calc_binomial(
    double start_ulying, double strike, double time_to_expiry, double risk_free_rate, double vol, int steps)
{
    double timestep = time_to_expiry / steps;
    double u = exp(vol * timestep);
    double d = 1 / u;
    vector<vector<Moment>> moments;
    vector<Moment> final_moments;
    // continuous
    // double lambda = exp(timestep * risk_free_rate);
    // discrete
    double lambda = pow(1 + risk_free_rate, timestep);
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

int main() {
    double start_ulying = 100;
    double strike = 100;
    double time_to_expiry = 3;
    double risk_free_rate = 0.01;

    int steps = 3;
    double u = 1.1;
    double vol = calc_vol_from_desired_u(u, time_to_expiry / steps);

    double option_price = calc_binomial(start_ulying, strike, time_to_expiry, risk_free_rate, vol, steps);
    cout << option_price << '\n';
    return 0;
}
