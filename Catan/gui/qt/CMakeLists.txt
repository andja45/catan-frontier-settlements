find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Multimedia)

add_library(catan_gui_qt
        screens/game/GameWindow.cpp
        screens/game/GameWindow.h
        screens/game/board/QBoard.cpp
        screens/game/board/QBoard.h
        screens/game/board/elements/QTile.h
        screens/game/board/elements/QTile.cpp
        screens/game/board/elements/QNode.h screens/game/board/elements/QNode.cpp
        screens/history/GameHistory.h screens/history/GameHistory.cpp
        screens/history/GameDetailsView.h screens/history/GameDetailsView.cpp
        screens/history/GameHistory.h screens/history/GameHistory.cpp
        screens/history/GameDetailsView.h screens/history/GameDetailsView.cpp
        common/theme/GameTheme.h common/theme/GameTheme.cpp
        screens/connect/HostJoinView.h screens/connect/HostJoinView.cpp
        screens/lobby/LobbyView.h screens/lobby/LobbyView.cpp
        screens/lobby/PlayerListItem.h screens/lobby/PlayerListItem.cpp
        screens/lobby/AddBotListItem.h screens/lobby/AddBotListItem.cpp
        screens/game/board/elements/QEdge.h screens/game/board/elements/QEdge.cpp
        common/cards/QCard.h common/cards/QCard.cpp
        common/cards/QCardRow.h common/cards/QCardRow.cpp
        common/cards/CardSpec.h
        common/panels/FloatingPanel.h common/panels/FloatingPanel.cpp
        screens/game/toolbar/BoardToolbar.h screens/game/toolbar/BoardToolbar.cpp
        common/panels/CostPopup.h common/panels/CostPopup.cpp
        common/cards/QCountBadge.h common/cards/QCountBadge.cpp
        screens/game/action-popups/popups/StealCardPopup.h screens/game/action-popups/popups/StealCardPopup.cpp
        common/player/PlayerDot.h common/player/PlayerDot.cpp
        screens/game/toolbar/tool-popups/ChooseDevCardPopup.h screens/game/toolbar/tool-popups/ChooseDevCardPopup.cpp
        screens/game/action-popups/popups/MonopolyPopup.h screens/game/action-popups/popups/MonopolyPopup.cpp
        screens/game/toolbar/tool-popups/RequestPlayerTradePopup.h screens/game/toolbar/tool-popups/RequestPlayerTradePopup.cpp
        screens/game/toolbar/tool-popups/RequestBankTradePopup.h screens/game/toolbar/tool-popups/RequestBankTradePopup.cpp
        screens/game/trade-pupups/popups/RespondPlayerTradePopup.h screens/game/trade-pupups/popups/RespondPlayerTradePopup.cpp
        screens/game/toolbar/buttons/DiceWidget.h screens/game/toolbar/buttons/DiceWidget.cpp
        screens/game/action-popups/popups/YearOfPlentyPopup.cpp
        screens/game/action-popups/popups/YearOfPlentyPopup.h
        screens/game/action-popups/popups/DiscardPopup.cpp
        screens/game/action-popups/popups/DiscardPopup.h
        screens/mainmenu/MainMenu.cpp
        screens/mainmenu/MainMenu.h
        resources/resources.qrc
        common/AudioManager.cpp
        common/AudioManager.h
        screens/game/trade-pupups/TradeOverlay.cpp
        screens/game/trade-pupups/TradeOverlay.hpp
        common/player/PlayerView.cpp
        common/player/PlayerView.hpp
        common/player/PlayerDetailedView.cpp
        common/player/PlayerDetailedView.hpp
        common/chat/Chat.cpp
        common/chat/Chat.hpp
        screens/game/sidebar/PlayersOverlay.cpp
        screens/game/sidebar/PlayersOverlay.hpp
        common/panels/FloatingButton.cpp
        common/panels/FloatingButton.hpp
        screens/game/trade-pupups/popups/AcceptPlayerTradePopup.cpp
        screens/game/trade-pupups/popups/AcceptPlayerTradePopup.hpp
        screens/game/action-popups/ActionManager.cpp
        screens/game/action-popups/ActionManager.hpp
        screens/game/game-overlay/GameOverlay.cpp
        screens/game/game-overlay/GameOverlay.hpp
)

target_include_directories(catan_gui_qt
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(catan_gui_qt
        PUBLIC
        catan_game
        catan_controller
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Charts
        Qt${QT_VERSION_MAJOR}::Multimedia
)

set_target_properties(catan_gui_qt PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
)
