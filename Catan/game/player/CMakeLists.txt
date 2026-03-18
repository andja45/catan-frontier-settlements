add_library(catan_player_lib
        Player.cpp
        Player.h
        Bank.cpp
)

target_include_directories(catan_player_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})