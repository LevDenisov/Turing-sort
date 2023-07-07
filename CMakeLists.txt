cmake_minimum_required(VERSION 3.24)
project(ExternSort)

find_library(LIBCONFIG_LIB libconfig.so /usr/lib /usr/local/lib)

include_directories(/usr/include /usr/local/include)

set(CMAKE_CXX_STANDARD 17)

add_executable(ExternSort main.cpp tape.cpp tape.h interface.cpp interface.h)
