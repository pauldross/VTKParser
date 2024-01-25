cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(parser)


add_executable(parser MACOSX_BUNDLE parser.cpp
        depend/VTKParser.h
        depend/VTKParser.cpp
        depend/VTKParser.cpp
        depend/VTKParser.h
)

