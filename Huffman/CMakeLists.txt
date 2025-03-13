cmake_minimum_required(VERSION 3.30)
project(Huffman)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Huffman
    BitIO.cpp
    BitIO.hpp
    BuildHuffmanTree.cpp
    BuildHuffmanTree.hpp
    CoupleMessageProba.cpp
    CoupleMessageProba.hpp
    mainHuffman.cpp
    Node.cpp
    Node.hpp
    Table.cpp
    Table.hpp)
