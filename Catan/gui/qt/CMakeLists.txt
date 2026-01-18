find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Charts)


add_library(catan_gui_qt
        windows/game/widget.cpp
        windows/game/widget.h
        windows/game/widget.ui
        windows/game/board/QBoard.cpp
        windows/game/board/QBoard.h
        windows/game/board/QTile.h
        windows/game/board/QTile.cpp
        windows/game/board/QNode.h windows/game/board/QNode.cpp
        windows/history/GameHistory.h windows/history/GameHistory.cpp
        windows/history/GameDetailsView.h windows/history/GameDetailsView.cpp
        windows/game/sidebar/QRightSideBar.cpp
        windows/game/sidebar/QRightSideBar.h
        windows/history/GameHistory.h windows/history/GameHistory.cpp
        windows/history/GameDetailsView.h windows/history/GameDetailsView.cpp
        common/GameTheme.h common/GameTheme.cpp
        windows/host_join/GameJoinView.h windows/host_join/GameJoinView.cpp
        windows/host_join/GameHostView.h windows/host_join/GameHostView.cpp
        windows/host_join/PlayerListItem.h windows/host_join/PlayerListItem.cpp
        windows/host_join/AddBotListItem.h windows/host_join/AddBotListItem.cpp
        windows/game/board/QEdge.h windows/game/board/QEdge.cpp
)

target_include_directories(catan_gui_qt
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(catan_gui_qt
        PUBLIC
        catan_game
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Charts

)

set_target_properties(catan_gui_qt PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
)
