cmake_minimum_required(VERSION 3.5)

project(cash LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cash main.cpp cash-register.h cashr-register.cpp csvrow.h csvrow.cpp menu.h menu.cpp
)

install(TARGETS cash
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
