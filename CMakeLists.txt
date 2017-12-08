cmake_minimum_required(VERSION 3.6)
project(aoc)

add_executable(aoc_03 03/aoc_03.cpp)
target_compile_features(aoc_03 PRIVATE cxx_std_14)

add_executable(aoc_04 04/aoc_04.cpp)
target_compile_features(aoc_04 PRIVATE cxx_std_14)

add_executable(aoc_05 05/aoc_05.cpp)
target_compile_features(aoc_05 PRIVATE cxx_std_14)
