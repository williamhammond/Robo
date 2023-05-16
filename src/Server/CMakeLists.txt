file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)

add_executable(Server ${SOURCE_FILES} ${HEADER_FILES})

if (MSVC)
    target_compile_options(Server PRIVATE /W4 /WX)
else ()
    target_compile_options(Server PRIVATE -Wall -Wextra -pedantic -Wshadow -Wdouble-promotion -Wundef -Wconversion -Werror)
endif ()

target_link_libraries(Server PRIVATE Common)

# See https://github.com/libsdl-org/SDL/issues/6399 for why this exists
if (WIN32)
    add_custom_command(
            TARGET Server POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:Server>"
            VERBATIM
    )
endif ()
