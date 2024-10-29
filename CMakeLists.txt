cmake_minimum_required(VERSION 3.1...3.29)
project(
  GAU-8
  VERSION 1.0
  LANGUAGES CXX
)

add_library(gau8_ssh STATIC ./src/lib/ssh.cpp ./src/lib/connectors.hpp)

add_library(gau8_license STATIC ./src/lib/license.cpp ./src/lib/connectors.hpp)

add_library(useful_funx STATIC ./src/lib/useful.hpp ./src/lib/useful.cpp)

add_library(athreads STATIC ./src/lib/athreads.cpp ./src/lib/athreads.hpp)

add_executable(gau8 "./src/main.cpp")

target_link_libraries(gau8 gau8_ssh gau8_license useful_funx athreads ssh curl)
