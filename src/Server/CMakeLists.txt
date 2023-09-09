add_executable(Server ServerMain.cpp)

if (MSVC)
    target_compile_options(Server PRIVATE /W4 /WX)
else ()
    target_compile_options(Server PRIVATE -Wall -Wextra -pedantic -Wshadow -Wdouble-promotion -Wundef -Werror)
endif ()

add_custom_command(
        TARGET Server POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:Server>"
        VERBATIM
)

target_link_libraries(Server PUBLIC ServerLib Common)
