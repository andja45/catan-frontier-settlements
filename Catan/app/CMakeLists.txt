add_executable(Catan
        main.cpp
        ../controller/host-join/HostJoinController.cpp
        ../controller/host-join/HostJoinController.hpp
        ../controller/host-join/RoleType.hpp
)

target_link_libraries(Catan
        PRIVATE
        catan_gui_qt
        catan_ascii_view
        catan_network_client
        catan_network_server
        catan_game

)
qt_add_resources(Catan resources/resource.qrc)


#set_target_properties(Catan PROPERTIES WIN32_EXECUTABLE TRUE)
