add_subdirectory(proto)
add_subdirectory(dto-translation)

# Some network components depend on Qt (Core/Network). Allow disabling them when Qt isn't available.
option(BUILD_NETWORK_QT "Build network components that require Qt" ON)
if(BUILD_NETWORK_QT)
    add_subdirectory(message-transport)
endif()
add_subdirectory(server)
add_subdirectory(client)
