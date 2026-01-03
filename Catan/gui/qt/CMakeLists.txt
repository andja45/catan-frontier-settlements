find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


add_library(catan_gui_qt
        widget.cpp
        widget.h
        ui/widget.ui
        QBoard.cpp
        QBoard.h
)

target_include_directories(catan_gui_qt
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(catan_gui_qt
        PUBLIC
        catan_game
        Qt${QT_VERSION_MAJOR}::Widgets
)

set_target_properties(catan_gui_qt PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
)
