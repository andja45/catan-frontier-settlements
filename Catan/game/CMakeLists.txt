add_library(catan_game INTERFACE)

add_subdirectory(board)
add_subdirectory(model)
add_subdirectory(move)
add_subdirectory(phase)
add_subdirectory(player)
add_subdirectory(rules)
add_subdirectory(types)

target_include_directories(catan_game
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(catan_game
        INTERFACE
        catan_libs
        catan_board_lib
        catan_model_lib
        catan_move_lib
        catan_phase_lib
        catan_player_lib
        catan_rules_lib
        catan_types_lib
)

target_compile_features(catan_game INTERFACE cxx_std_17)
