cmake_minimum_required(VERSION 3.5)
project(aoc)

set(PROJECTS aoc_01 
             aoc_02 
             aoc_03 
             aoc_04 
             aoc_05 
             aoc_06 
             aoc_07 
             aoc_08 
             aoc_09 
             aoc_10 
             aoc_11 
             aoc_12 
             aoc_13 
             aoc_14 
             aoc_15 
             aoc_16 
             aoc_17 
             aoc_18 
             aoc_19 
             aoc_20 
             aoc_21 
             aoc_22 
             aoc_23 
             aoc_24 
             aoc_25)

foreach(target IN LISTS PROJECTS) 
    add_executable(${target} src/${target}.cpp)
    target_compile_features(${target} PRIVATE cxx_lambdas cxx_trailing_return_types cxx_auto_type cxx_return_type_deduction) 
endforeach()

