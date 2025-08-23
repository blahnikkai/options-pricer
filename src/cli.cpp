#include "argparse/argparse.hpp"
#include <iostream>

#include "pricing.h"

int main(int argc, char ** argv) {

    argparse::ArgumentParser arg_parser("program_name");

    double u, k, t, r, v;

    arg_parser
        .add_argument("-u", "--underlying")
        .required()
        .help("The starting price of the underlying asset")
        .store_into(u);

    arg_parser
        .add_argument("-s", "--strike")
        .required()
        .help("The strike price of the option")
        .store_into(k);

    arg_parser
        .add_argument("-t", "--time-to-expiry")
        .required()
        .help("The amount of time until the option expires")
        .store_into(t);

    arg_parser
        .add_argument("-r", "--risk-free-rate")
        .required()
        .help("The rate of return on a risk free investment")
        .store_into(r);

    arg_parser
        .add_argument("-v", "--volatility")
        .required()
        .help("The volatility of the underlying asset")
        .store_into(v);

    try {
        arg_parser.parse_args(argc, argv);
    }
    catch(const std::exception & err) {
        std::cerr << err.what() << std::endl;
        std::cerr << arg_parser;
        std::exit(1);
    }

    double black_scholes = pricing::calc_black_scholes(u, k, t, r, v);
    std::cout << black_scholes << "\n";

    return 0;
}

