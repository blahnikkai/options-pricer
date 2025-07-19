#include <bits/stdc++.h>
using namespace std;

double calculate_option_price(
    int steps, double start_ulying, double strike, double q, double u, double risk_free_rate, double timestep)
{
    vector<vector<double>> prices;
    vector<double> final_prices;
    for(int i = 0; i <= steps; i++) {
        int worst = -steps;
        double ulying = start_ulying * pow(u, worst + 2 * i);
        double val = max(ulying - strike, (double)0);
        final_prices.push_back(val);
    }
    prices.push_back(final_prices);
    for(int i = 0; i < steps; i++) {
        vector<double> price_row;
        for(int j = 0; j < steps - i; j++) {
            double d_val = prices.at(prices.size() - 1).at(j);
            double u_val = prices.at(prices.size() - 1).at(j + 1);
            double val = exp(-timestep * risk_free_rate) * (q * u_val + (1 - q) * d_val);
            price_row.push_back(val);
        }
        prices.push_back(price_row);
    }
    double calculated_price = prices.at(prices.size() - 1).at(0);
    return calculated_price;
}

int main() {
    int steps = 3;
    double start_ulying = 100;
    double strike = 100;
    double q = 0.5286;
    double u = 1.1;
    double risk_free_rate = 0.01;
    double timestep = 1;
    double option_price = calculate_option_price(steps, start_ulying, strike, q, u, risk_free_rate, timestep);
    cout << option_price << '\n';
    return 0;
}
