add_executable(Client ClientMain.cpp)

if (MSVC)
    target_compile_options(Client PRIVATE /W4 /WX)
else ()
    target_compile_options(Client PRIVATE -Wall -Wextra -pedantic -Wshadow -Wdouble-promotion -Wundef -Werror)
endif ()

target_link_libraries(Client PUBLIC ClientLib Common)
