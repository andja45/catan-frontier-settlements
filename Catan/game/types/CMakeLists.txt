add_library(catan_types_lib INTERFACE
        NodeType.h
        TypeAliases.h
        DevCardType.h
        Costs.h
        Directions.h
        ResourceType.h
        MapType.h
        BoardType.h
        ToolbarActionType.h
)

target_include_directories(catan_types_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)