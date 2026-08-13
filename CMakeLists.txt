cmake_minimum_required(VERSION 3.20)
project(base64 C)
add_library(base64 STATIC src/base64.c)
target_include_directories(base64 PUBLIC include)
