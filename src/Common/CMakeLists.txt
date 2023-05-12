file(GLOB_RECURSE SOURCE_FILES *.cpp)
file(GLOB_RECURSE HEADER_FILES *.h)

add_library(Common ${SOURCE_FILES} ${HEADER_FILES})

if (MSVC)
    target_compile_options(Common PRIVATE /W4 /WX)
else ()
    target_compile_options(Common PRIVATE -Wall -Wextra -pedantic -Wshadow -Wdouble-promotion -Wundef -Wconversion -Werror)
endif ()

target_include_directories(Common PUBLIC ${CMAKE_CURRENT_LIST_DIR})
