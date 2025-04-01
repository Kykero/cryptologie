cmake_minimum_required(VERSION 3.30)
project(LZW)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(LZW
    BitIO.cpp
    BitIO.hpp
    Dictionnaire.cpp
    Dictionnaire.hpp
    Lzw.cpp
    Lzw.hpp
    main.cpp
    SequenceMessages.cpp
    SequenceMessages.hpp)
