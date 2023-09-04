include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
        integration_tests
        IntegrationTests.cpp
)

if (WIN32)
    add_custom_command(
            TARGET integration_tests POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:integration_tests>"
            VERBATIM
    )
    add_custom_command(
            TARGET integration_tests POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:fmt::fmt>" "$<TARGET_FILE_DIR:integration_tests>"
            VERBATIM
    )
    add_custom_command(
            TARGET integration_tests POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:ServerLib>" "$<TARGET_FILE_DIR:integration_tests>"
            VERBATIM
    )
    add_custom_command(
            TARGET integration_tests POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:Common>" "$<TARGET_FILE_DIR:integration_tests>"
            VERBATIM
    )

endif ()
target_link_libraries(
        integration_tests
        PRIVATE
        GTest::gtest_main
        GTest::gtest
        ServerLib
)

if (MSVC)
    target_compile_options(integration_tests PRIVATE /W4 /WX)
else ()
    target_compile_options(integration_tests PRIVATE -Wall -Wextra -pedantic -Wshadow -Wdouble-promotion -Wundef -Werror)
endif ()

include(GoogleTest)
gtest_discover_tests(integration_tests)
