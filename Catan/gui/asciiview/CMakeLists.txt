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
        player/PlayerOverview.cpp
        player/PlayerOverview.hpp
        drawing/AsciiCanvas.cpp
        drawing/AsciiCanvas.hpp
        player/PlayersOverview.cpp
        player/PlayersOverview.hpp
        player/MyCardsOverview.cpp
        player/MyCardsOverview.hpp
        player/BankOverview.cpp
        player/BankOverview.hpp
        drawing/AsciiDrawable.cpp
        drawing/AsciiDrawable.hpp
        drawing/AsciiComposite.cpp
        drawing/AsciiComposite.hpp
        drawing/AsciiWindow.cpp
        drawing/AsciiWindow.hpp
        drawing/AsciiTheme.hpp
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
