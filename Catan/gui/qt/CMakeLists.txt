find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Charts)


add_library(catan_gui_qt
        screens/game/widget.cpp
        screens/game/widget.h
        screens/game/widget.ui
        screens/game/board/QBoard.cpp
        screens/game/board/QBoard.h
        screens/game/board/QTile.h
        screens/game/board/QTile.cpp
        screens/game/board/QNode.h screens/game/board/QNode.cpp
        screens/history/GameHistory.h screens/history/GameHistory.cpp
        screens/history/GameDetailsView.h screens/history/GameDetailsView.cpp
        screens/game/sidebar/RightOverlay.cpp
        screens/game/sidebar/RightOverlay.h
        screens/history/GameHistory.h screens/history/GameHistory.cpp
        screens/history/GameDetailsView.h screens/history/GameDetailsView.cpp
        common/GameTheme.h common/GameTheme.cpp
        screens/lobby/GameJoinView.h screens/lobby/GameJoinView.cpp
        screens/lobby/GameHostView.h screens/lobby/GameHostView.cpp
        screens/lobby/PlayerListItem.h screens/lobby/PlayerListItem.cpp
        screens/lobby/AddBotListItem.h screens/lobby/AddBotListItem.cpp
        screens/game/board/QEdge.h screens/game/board/QEdge.cpp
        components/cards/QCard.h components/cards/QCard.cpp
        components/cards/QCardRow.h components/cards/QCardRow.cpp
        components/cards/CardSpec.h
        components/panels/FloatingPanel.h components/panels/FloatingPanel.cpp
        screens/game/sidebar/BoardToolbar.h screens/game/sidebar/BoardToolbar.cpp
        components/panels/CostPopup.h components/panels/CostPopup.cpp
        components/cards/CountBadge.h components/cards/CountBadge.cpp
        components/panels/RobPlayerPopup.h components/panels/RobPlayerPopup.cpp
        components/panels/PlayerDot.h components/panels/PlayerDot.cpp
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
