cmake_minimum_required(VERSION 3.22)
project(OptionsPricer)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        test_pricing

        src/test_pricing.cpp
        src/pricing.cpp
        src/utils.cpp
)

add_executable(
        options_pricer_cli

        src/cli.cpp
        src/pricing.cpp
        src/utils.cpp
)
target_include_directories(options_pricer_cli PRIVATE ${CMAKE_SOURCE_DIR}/src/include)

add_executable(
        api

        src/api.cpp
        src/pricing.cpp
        src/utils.cpp
)
target_link_libraries(api PRIVATE zmq)
target_include_directories(api PRIVATE ${CMAKE_SOURCE_DIR}/src/include)

add_executable(
        test_zmq

        src/zmq_test.cpp
)
target_link_libraries(test_zmq PRIVATE zmq)
target_include_directories(test_zmq PRIVATE ${CMAKE_SOURCE_DIR}/src/include)
