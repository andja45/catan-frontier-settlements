target_sources(catan_game PRIVATE
        Player.cpp
        Player.h
        Bank.cpp
)

target_include_directories(catan_game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})