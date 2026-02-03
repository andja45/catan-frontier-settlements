add_executable(Catan
        main.cpp
        ../appServer/main.cpp
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
add_custom_command(TARGET Catan POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:Catan>/resources
)

#set_target_properties(Catan PROPERTIES WIN32_EXECUTABLE TRUE)
