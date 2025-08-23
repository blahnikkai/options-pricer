cmake_minimum_required(VERSION 3.22)
project(OptionsPricingModel)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        test_simulation

        src/test_simulation.cpp
        src/Moment.h
        src/simulation.cpp
        src/simulation.h
        src/utils.cpp
        src/utils.h
)

add_executable(
        cli

        src/cli.cpp
        src/argparse/argparse.hpp
        src/Moment.h
        src/simulation.cpp
        src/simulation.h
        src/utils.cpp
        src/utils.h
)
