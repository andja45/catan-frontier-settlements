add_library(catan_types_lib
        NodeType.h
        TypeAliases.h
        DevCardType.h
        Costs.h
        Directions.h
        ResourceType.h
        MapType.h
        BoardType.hpp
        ToolbarActionType.h
)

target_include_directories(catan_types_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})