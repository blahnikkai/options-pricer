cmake_minimum_required(VERSION 3.22)
project(OptionsPricingModel)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        test_pricing

        src/test_pricing.cpp
        src/Moment.h
        src/pricing.cpp
        src/pricing.h
        src/utils.cpp
        src/utils.h
)

add_executable(
        options_pricer_cli

        src/cli.cpp
        src/argparse/argparse.hpp
        src/Moment.h
        src/pricing.cpp
        src/pricing.h
        src/utils.cpp
        src/utils.h
)
