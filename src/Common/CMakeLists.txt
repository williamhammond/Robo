file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)

add_library(Common ${SOURCE_FILES} ${HEADER_FILES})

if (MSVC)
    target_compile_options(Common PRIVATE /W4 /WX)
else ()
    target_compile_options(Common PRIVATE -Wall -Wextra -pedantic -Wshadow -Wdouble-promotion -Wundef -Werror)
endif ()

target_link_libraries(Common PRIVATE spdlog SDL2::SDL2 fmt)
target_include_directories(Common PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${sdl2_SOURCE_DIR}/include ${fmt_SOURCE_DIR}/include)
