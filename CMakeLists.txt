cmake_minimum_required(VERSION 3.22)

project(Robo
        VERSION 0.1
        DESCRIPTION "Test project for game networking"
        LANGUAGES CXX)

set(CMAKE_BINARY_DIR build/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
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
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.0.0)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.11.0)
FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.26.5)

if (NOT sdl2_POPULATED)
    FetchContent_Populate(SDL2)
    message(STATUS "Catch source dir: ${sdl2_SOURCE_DIR}")
    message(STATUS "Catch binary dir: ${sdl2_BINARY_DIR}")
    add_subdirectory(${sdl2_SOURCE_DIR} ${sdl2_BINARY_DIR})
endif ()

if (NOT fmt_POPULATED)
    FetchContent_Populate(fmt)
    message(STATUS "Catch source dir: ${fmt_SOURCE_DIR}")
    message(STATUS "Catch binary dir: ${fmt_BINARY_DIR}")
    add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR})
endif ()

FetchContent_MakeAvailable(spdlog fmt SDL2)

add_subdirectory(src/Common)
add_subdirectory(src/Server)
add_subdirectory(src/Client)

set(CMAKE_INSTALL_PREFIX "./build/install")
if (UNIX)
    include(GNUInstallDirs)
else ()
    if (WIN32)
        set(CMAKE_INSTALL_LIBDIR "lib")
        set(CMAKE_INSTALL_DATADIR "share")
        set(CMAKE_INSTALL_INCLUDEDIR "include")
        set(CMAKE_INSTALL_BINDIR "bin")
        message(STATUS "Setting installation destination on Windows to: ${CMAKE_INSTALL_PREFIX} ")
        if (MSVC)
            link_libraries(ws2_32 wsock32)
        else ()
            link_libraries(ws2_32 -lws2_32)
        endif ()
    else ()
        message(FATAL_ERROR "System not UNIX nor WIN32 - not implemented yet")
    endif ()
endif ()
install(TARGETS Server Client Common
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
