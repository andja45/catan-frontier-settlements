cmake_minimum_required(VERSION 3.16)
project(CatanTests LANGUAGES CXX)

include(FetchContent)

# Prefer an installed Catch2 v3, but fall back to fetching.
find_package(Catch2 3 CONFIG QUIET)

if(NOT TARGET Catch2::Catch2WithMain)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.3.2
    )
    FetchContent_MakeAvailable(Catch2)
endif()

find_package(Catch2 3 CONFIG QUIET)

add_executable(catan_tests
        DiscardCardsMoveTest.cpp
        SetRobberMoveTest.cpp
        StealCardMoveTest.cpp
)

target_link_libraries(catan_tests PRIVATE
        catan_game
        Catch2::Catch2WithMain
)

enable_testing()
add_test(NAME catan_tests COMMAND catan_tests)
