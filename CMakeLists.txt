cmake_minimum_required(VERSION 4.1)
project(EnigmaAndBreaker)

set(CMAKE_CXX_STANDARD 20)

add_executable(EnigmaAndBreaker main.cpp
        Enigma.cpp
        Enigma.h
        EnigmaBreaker.cpp
        EnigmaBreaker.h)
