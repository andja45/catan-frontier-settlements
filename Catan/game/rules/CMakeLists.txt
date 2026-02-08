target_sources(catan_game PRIVATE
        Rule.h
        RulesEngine.cpp
        RulesEngine.h
        LargestArmyRule.cpp
        LargestArmyRule.h
        LongestRoadRule.cpp
        LongestRoadRule.h
        GameOverRule.cpp
        GameOverRule.h
        SkipPlayerRule.cpp
        SkipPlayerRule.hpp
)

target_include_directories(catan_game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})