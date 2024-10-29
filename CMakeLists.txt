cmake_minimum_required(VERSION 3.1...3.29)
project(
  GAU-8
  VERSION 1.0
  LANGUAGES CXX
)

add_library(gau8_ssh STATIC ./src/lib/ssh.cpp ./src/lib/connectors.hpp)

add_executable(gau8 "./src/main.cpp")

target_link_libraries(gau8 gau8_ssh ssh)
