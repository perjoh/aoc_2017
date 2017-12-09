cmake_minimum_required(VERSION 3.6)
project(aoc)

set(PROJECTS aoc_03 aoc_04 aoc_05 aoc_08)

foreach(target IN LISTS PROJECTS) 
	add_executable(${target} src/${target}.cpp)
	target_compile_features(${target} PRIVATE cxx_std_14)
	target_compile_options(${target} PUBLIC "/W4")
endforeach()

