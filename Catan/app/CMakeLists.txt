

add_executable(Catan
        main.cpp
        ../controller/renderstate/RenderState.h
        ../controller/renderstate/BoardRenderState.cpp
        ../controller/renderstate/BoardRenderState.h
        ../controller/renderstate/ButtonsRenderState.cpp
        ../controller/renderstate/ButtonsRenderState.h
)

target_link_libraries(Catan
        PRIVATE
        catan_gui_qt
)

#set_target_properties(Catan PROPERTIES WIN32_EXECUTABLE TRUE)
