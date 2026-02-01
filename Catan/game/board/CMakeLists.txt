target_sources(catan_game PRIVATE
        Board.cpp
        Board.h
        Edge.cpp
        Edge.h
        Tile.h
        # coords
        coords/AxialCoords.cpp
        coords/AxialCoords.hpp
        coords/ElementCoords.hpp
        coords/EdgeCoords.cpp
        coords/EdgeCoords.hpp
        coords/NodeCoords.cpp
        coords/NodeCoords.hpp
        # factory
        factory/AbstractBoardCreator.hpp
        factory/BoardCatalog.hpp
        factory/RandomStandardMapCreator.cpp
        factory/RandomStandardMapCreator.hpp
        factory/RandomExtendedMapCreator.cpp
        factory/RandomExtendedMapCreator.hpp
        factory/LoadFromFileCreator.cpp
        factory/LoadFromFileCreator.hpp
        # serialization
        serialization/BoardLayoutToJson.cpp
        serialization/BoardLayoutToJson.hpp
        serialization/AbstractBoardSerializer.hpp
)

target_include_directories(catan_game PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/coords
        ${CMAKE_CURRENT_SOURCE_DIR}/factory
        ${CMAKE_CURRENT_SOURCE_DIR}/serialization
)