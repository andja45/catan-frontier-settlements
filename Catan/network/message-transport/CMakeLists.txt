add_library(catan_transport
    MessageFramer.cpp
    MessageFramer.h
    NetworkTransport.h
    NetworkTransport.cpp
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)


target_link_libraries(catan_transport PUBLIC catan_proto)

target_link_libraries(catan_transport
        PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Network
)
target_include_directories(catan_transport
        PUBLIC
        .
)
