find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Charts)


add_library(catan_gui_qt
        widget.cpp
        widget.h
        ui/widget.ui
        QBoard.cpp
        QBoard.h
        QTile.h
        QTile.cpp
        QNode.h QNode.cpp
        GameHistory.h GameHistory.cpp
        GameDetailsView.h GameDetailsView.cpp
        QRightSideBar.cpp
        QRightSideBar.h
        GameHistory.h GameHistory.cpp
        GameDetailsView.h GameDetailsView.cpp
        GameTheme.h GameTheme.cpp
        host_join/GameJoinView.h host_join/GameJoinView.cpp
        host_join/GameHostView.h host_join/GameHostView.cpp
        host_join/PlayerListItem.h host_join/PlayerListItem.cpp
        host_join/AddBotListItem.h host_join/AddBotListItem.cpp
        QEdge.h QEdge.cpp
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
