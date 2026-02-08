target_sources(catan_game PRIVATE
        GameSession.cpp
        GameSession.h
        GameData.cpp
        GameData.h
        GameConfig.cpp
        GameConfig.h
        Trade.h
)

target_include_directories(catan_game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})