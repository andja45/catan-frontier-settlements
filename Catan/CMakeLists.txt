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
add_subdirectory(gui)
add_subdirectory(app)
add_subdirectory(network)
add_subdirectory(controller)

#set_target_properties(catan_gui_qt PROPERTIES
#        CXX_CLANG_TIDY ""
#)
#set_source_files_properties(
#        gui/qt/ui/widget.cpp
#        PROPERTIES
#        SKIP_LINTING ON
#)