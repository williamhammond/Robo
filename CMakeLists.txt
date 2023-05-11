cmake_minimum_required(VERSION 3.25)
project(Robo)

set(CMAKE_CXX_STANDARD 20)

add_executable(Robo src/Common/Engine.cpp src/Common/Engine.h src/Common/World.cpp src/Common/World.h src/Common/World.h src/Common/Clock.cpp src/Common/Clock.h src/Server/ServerMain.cpp src/Server/Server.cpp src/Server/Server.h src/Common/Clock.cpp)
