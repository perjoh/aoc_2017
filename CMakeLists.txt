cmake_minimum_required(VERSION 3.8)
project(aoc)

set(PROJECTS aoc_01 aoc_02 aoc_03 aoc_04 aoc_05 aoc_06 aoc_08 aoc_09 aoc_11 aoc_12 aoc_15 aoc_16 aoc_17 aoc_19)

foreach(target IN LISTS PROJECTS) 
	add_executable(${target} src/${target}.cpp)
	target_compile_features(${target} PRIVATE cxx_std_14)
	target_compile_options(${target} PRIVATE "/W4")
endforeach()

