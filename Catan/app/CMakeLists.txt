

add_executable(Catan
        main.cpp
)

target_link_libraries(Catan
        PRIVATE
        catan_gui_qt
)

#set_target_properties(Catan PROPERTIES WIN32_EXECUTABLE TRUE)
