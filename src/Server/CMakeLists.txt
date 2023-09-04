add_executable(Server ServerMain.cpp)

if (MSVC)
    target_compile_options(Server PRIVATE /W4 /WX)
else ()
    target_compile_options(Server PRIVATE -Wall -Wextra -pedantic -Wshadow -Wdouble-promotion -Wundef -Werror)
endif ()

target_link_libraries(Server PUBLIC ServerLib Common)
