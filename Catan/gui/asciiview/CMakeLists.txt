add_library(catan_ascii_view
        board/BoardView.cpp
        board/BoardView.hpp
        board/EdgeView.cpp
        board/EdgeView.hpp
        board/TileView.hpp
        board/TileView.cpp
        board/NodeView.cpp
        board/NodeView.hpp
        board/BoardTypes.hpp
        player/PlayerView.cpp
        player/PlayerView.hpp
)

target_include_directories(catan_ascii_view
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(catan_ascii_view
        PUBLIC
        catan_game
)

target_compile_features(catan_ascii_view PUBLIC cxx_std_17)
