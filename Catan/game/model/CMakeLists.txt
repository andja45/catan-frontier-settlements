add_library(catan_model_lib
        GameSession.cpp
        GameSession.h
        GameData.cpp
        GameData.h
        GameConfig.cpp
        GameConfig.h
        Trade.h
)

target_include_directories(catan_model_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(catan_model_lib
        PUBLIC
        catan_libs
        catan_rules_lib
        PRIVATE
        catan_types_lib
)