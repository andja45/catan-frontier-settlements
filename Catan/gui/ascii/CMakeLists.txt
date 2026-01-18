add_library(catan_ascii_view
        BoardView.cpp
        BoardView.hpp
        EdgeView.cpp
        EdgeView.hpp
        TileView.hpp
        TileView.cpp
        NodeView.cpp
        NodeView.hpp
        ViewTypes.hpp
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
