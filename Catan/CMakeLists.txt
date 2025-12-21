cmake_minimum_required(VERSION 3.16)

project(Catan VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        widget.cpp
        widget.h
        widget.ui
        headers/Board/Edge.h
        headers/Board/Tile.h
        code/Model/Board/Edge.cpp
        headers/Board/Edge.h
        headers/Types/NodeType.h
        code/Model/Player/Player.cpp
        headers/Player/Player.h
        code/Model/Board/Board.cpp
        headers/Board/Board.h
        code/Model/Move/Move.cpp
        headers/Move/Move.h
        code/Model/Move/BuildRoadMove.cpp
        headers/Move/BuildRoadMove.h
        code/Model/Move/BuildSettlementMove.cpp
        headers/Move/BuildSettlementMove.h
        code/Model/Move/BuildCityMove.cpp
        headers/Move/BuildCityMove.h
        code/Model/Move/PlayerTradeMove.cpp
        headers/Move/PlayerTradeMove.h
        code/Model/Move/RollDiceMove.cpp
        headers/Move/RollDiceMove.h
        code/Model/Move/EndTurnMove.cpp
        headers/Move/EndTurnMove.h
        code/Model/Game/GameModel.cpp
        headers/Game/GameModel.h
        code/Model/Game/GameSession.cpp
        headers/Game/GameSession.h
        headers/Types/TypeAliases.h
        headers/Types/DevType.h
        code/Model/Board/Board.cpp
        headers/Types/Costs.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Catan
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
            headers/Board/Tile.h code/Model/Board/Tile.cpp
            headers/Types/ResourceType.h
            headers/Board/Node.h code/Model/Board/Node.cpp
            code/Model/Move/BankTradeMove.cpp
            code/Model/Game/GameModel.cpp
            code/Model/Move/SettingRobberMove.cpp
            code/AsciiView/BoardView.cpp
            code/AsciiView/BoardView.hpp
            code/AsciiView/EdgeView.cpp
            code/AsciiView/EdgeView.hpp
            code/AsciiView/TileView.hpp
            code/AsciiView/TileView.cpp
            code/AsciiView/NodeView.cpp
            code/AsciiView/NodeView.hpp
            code/AsciiView/ViewTypes.hpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET neee APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CatanSHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Catan
            ${PROJECT_SOURCES}
                headers/Types/Costs.h
                headers/Types/Costs.h
                code/Model/Move/SettingRobberMove.cpp
                headers/Move/SettingRobberMove.h
                code/Model/Move/BankTradeMove.cpp
                headers/Move/BankTradeMove.h
                code/AsciiView/BoardView.cpp
                code/AsciiView/BoardView.hpp
                code/AsciiView/EdgeView.cpp
                code/AsciiView/EdgeView.hpp
                code/AsciiView/TileView.cpp
                code/AsciiView/TileView.hpp
                code/AsciiView/TileView.hpp
                code/AsciiView/TileView.cpp
                code/AsciiView/NodeView.cpp
                code/AsciiView/NodeView.hpp
                code/AsciiView/ViewTypes.hpp
        )
    endif()
endif()

target_link_libraries(Catan PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Catan)
endif()
set_target_properties(Catan PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Catan
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Catan)
endif()
