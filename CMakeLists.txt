cmake_minimum_required(VERSION 3.28)
project(BinaryOptionsPricingModel)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        BinaryOptionsPricingModel

        src/main.cpp
        src/argparse/argparse.hpp
        src/Moment.h
        src/simulation.cpp
        src/simulation.h
        src/utils.cpp
        src/utils.h
)
