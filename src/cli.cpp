#include "argparse/argparse.hpp"
#include <iostream>

#include "pricing.h"

int main(int argc, char ** argv) {

    argparse::ArgumentParser arg_parser("program_name");

    double u, k, t, r, v;

    arg_parser
        .add_argument("-u", "--underlying")
        .required()
        .help("The starting price of the underlying asset.")
        .store_into(u);

    arg_parser
        .add_argument("-s", "--strike")
        .required()
        .help("The strike price of the option.")
        .store_into(k);

    arg_parser
        .add_argument("-t", "--time-to-expiry")
        .required()
        .help("The amount of time until the option expires.")
        .store_into(t);

    arg_parser
        .add_argument("-r", "--risk-free-rate")
        .required()
        .help("The rate of return as a percent per unit time on a risk free investment.")
        .store_into(r);

    arg_parser
        .add_argument("-v", "--volatility")
        .required()
        .help("The volatility of the underlying asset with time horizon equal to a unit time.")
        .store_into(v);

    int binomial_steps;
    arg_parser
        .add_argument("-b", "--binomial-steps")
        .help("The number of steps in the binomial tree pricing model.")
        .store_into(binomial_steps);

    std::vector<int> monte_carlo_args;
    arg_parser
        .add_argument("-m", "--monte-carlo")
        .nargs(2)
        .help("Takes two arguments, the number of steps in a monte carlo simulation and the number of simulations to perform.")
        .store_into(monte_carlo_args);

    bool put;
    arg_parser
        .add_argument("-p", "--put")
        .flag()
        .help("Flag to select a long put contract")
        .store_into(put);

    bool american;
    arg_parser
        .add_argument("-a", "--american")
        .flag()
        .help("Flag to select an American option, which can be exercised at any time. This extra optionality makes them more expensive.")
        .store_into(american);

    try {
        arg_parser.parse_args(argc, argv);
    }
    catch(const std::exception & err) {
        std::cerr << err.what() << std::endl;
        std::cerr << arg_parser;
        std::exit(1);
    }

    std::string option_country = american ? "American" : "European";
    std::string put_or_call = put ? "put" : "call";
    std::cout << "Calculating the price of one " << option_country << " " << put_or_call << " with the following parameters\n";
    std::cout << "\tunderlying price = " << u << "\n";
    std::cout << "\tstrike price = " << k << "\n";
    std::cout << "\ttime to expiry = " << t << "\n";
    std::cout << "\trisk free rate = " << r << " (" << (r * 100) << "%)\n";
    std::cout << "\tvolatility = " << v << " (" << v * 100 << "%)\n";

    if(!american) {
        std::cout << "Calculating the exact price with the Black-Scholes formula\n";
        double black_scholes_price = pricing::calc_black_scholes(u, k, t, r, v, !put);
        std::cout << "Black Scholes Price:\t" << black_scholes_price << '\n';
    }

    if(arg_parser.is_used("-b")) {
        std::cout << "Approximating the price with a binomial tree with " << binomial_steps << " steps.\n";
        double binomial_price = pricing::calc_binomial(u, k, t, r, v, binomial_steps, !put, !american);
        std::cout << "Binomial Tree Price:\t" << binomial_price << '\n';
    }

    if(!american && arg_parser.is_used("-m")) {
        std::cout << "Approximating the price with " << monte_carlo_args.at(1) << " Monte Carlo simulations of " << monte_carlo_args.at(0) << " steps each.\n";
        double monte_carlo_price = pricing::calc_monte_carlo(u, k, t, r, v, monte_carlo_args.at(0), monte_carlo_args.at(1), !put);
        std::cout << "Monte Carlo Price:\t" << monte_carlo_price << '\n';
    }

    return 0;
}

