

add_executable(Catan
        main.cpp
        ../controller/renderstate/RenderState.h
        ../controller/renderstate/BoardRenderState.cpp
        ../controller/renderstate/BoardRenderState.h
        ../controller/renderstate/ToolbarRenderState.cpp
        ../controller/renderstate/ToolbarRenderState.h
        ../network/MoveProtoFactory.cpp
        ../network/MoveProtoFactory.h
        ../network/MoveProtoSerializer.h
        ../network/MoveProtoSerializer.cpp
)

target_link_libraries(Catan
        PRIVATE
        catan_gui_qt
        catan_ascii_view
)

#set_target_properties(Catan PROPERTIES WIN32_EXECUTABLE TRUE)
