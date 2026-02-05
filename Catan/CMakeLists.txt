cmake_minimum_required(VERSION 3.16)
project(Catan VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#add_compile_options(-Wall -Wextra)

set(CMAKE_CXX_CLANG_TIDY "")
#find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)

    set(CMAKE_CXX_CLANG_TIDY
            ${CLANG_TIDY_EXE}
    )
else()
    message(WARNING "clang-tidy not found")
endif()


add_subdirectory(libs)
add_subdirectory(game)
add_subdirectory(network)
if(BUILD_CLIENT)
    add_subdirectory(gui)
    add_subdirectory(controller)
endif()

option(BUILD_CLIENT "Build the client executable" ON)
option(BUILD_SERVER "Build the server executable" ON)
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_CLIENT)
    add_subdirectory(appClient)
endif()

if(BUILD_SERVER)
    add_subdirectory(appServer)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

