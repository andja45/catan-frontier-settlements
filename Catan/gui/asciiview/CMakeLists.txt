add_library(catan_ascii_view
        board/BoardView.cpp
        board/BoardView.hpp
        board/EdgeView.cpp
        board/EdgeView.hpp
        board/TileView.hpp
        board/TileView.cpp
        board/NodeView.cpp
        board/NodeView.hpp
        drawing/AsciiTypes.hpp
        drawing/AsciiCanvas.cpp
        drawing/AsciiCanvas.hpp
        drawing/AsciiDrawable.cpp
        drawing/AsciiDrawable.hpp
        drawing/AsciiComposite.cpp
        drawing/AsciiComposite.hpp
        drawing/AsciiWindow.cpp
        drawing/AsciiWindow.hpp
        drawing/AsciiTheme.hpp
        drawing/AsciiTextDrawable.cpp
        drawing/AsciiTextDrawable.hpp
        player/ResourceHolderView.cpp
        player/ResourceHolderView.hpp
        player/ResourceHolderHiddenView.cpp
        player/ResourceHolderHiddenView.hpp
        player/PlayerSideBar.cpp
        player/PlayerSideBar.hpp
        toolbar/ButtonsToolbar.cpp
        toolbar/ButtonsToolbar.hpp
        AsciiGameWindow.cpp
        AsciiGameWindow.hpp
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
