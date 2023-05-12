cmake_minimum_required(VERSION 3.25)

project(Robo
        VERSION 0.1
        DESCRIPTION "Test project for game networking"
        LANGUAGES CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    # Ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    include(CTest)
endif ()

include(FetchContent)
FetchContent_Declare(
        fmtlib
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 5.3.0)
FetchContent_MakeAvailable(fmtlib)

add_subdirectory(src/Common)
add_subdirectory(src/Server)
