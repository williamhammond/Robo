if (WIN32)
    add_executable(Client WIN32 ClientMain.cpp)
else ()
    add_executable(Client ClientMain.cpp)
endif ()

if (MSVC)
    target_compile_options(Client PRIVATE /W4 /WX)
else ()
    target_compile_options(Client PRIVATE -Wall -Wextra -pedantic -Wshadow -Wdouble-promotion -Wundef -Werror)
endif ()

add_custom_command(
        TARGET Client POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:Client>"
        VERBATIM
)

target_link_libraries(Client PUBLIC ClientLib Common SDL2::SDL2)
