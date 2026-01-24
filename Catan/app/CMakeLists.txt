

add_executable(Catan
        main.cpp
)

target_link_libraries(Catan
        PRIVATE
        catan_gui_qt
        catan_ascii_view
        catan_networkmoves
)

#set_target_properties(Catan PROPERTIES WIN32_EXECUTABLE TRUE)
