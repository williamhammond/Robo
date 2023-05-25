file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)

add_executable(Client ${SOURCE_FILES} ${HEADER_FILES})
if (MSVC)
    target_compile_options(Client PRIVATE /W4 /WX)
else ()
    target_compile_options(Client PRIVATE -Wall -Wextra -pedantic -Wshadow -Wdouble-promotion -Wundef -Werror)
endif ()

set(platform_dependent_libs "")
if (WIN32)
    if (MSVC)
        list(APPEND platform_dependent_libs "ws2_32")
    else ()
        list(APPEND platform_dependent_libs "-lws2_32")
    endif ()
endif ()
target_link_libraries(Client PRIVATE Common spdlog ${platform_dependent_libs})

# See https://github.com/libsdl-org/SDL/issues/6399 for why this exists
if (WIN32)
    add_custom_command(
            TARGET Client POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:Client>"
            VERBATIM
    )
    add_custom_command(
            TARGET Client POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:fmt::fmt>" "$<TARGET_FILE_DIR:Client>"
            VERBATIM
    )
endif ()
