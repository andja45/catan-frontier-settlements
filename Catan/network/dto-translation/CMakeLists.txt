add_library(catan_dto
        board/BoardSerializer.hpp
        board/BoardFactory.cpp
        board/BoardFactory.hpp
        board/BoardSerializer.cpp
        move/MoveProtoSerializer.cpp
        move/MoveProtoSerializer.h
        move/MoveProtoFactory.cpp
        move/MoveProtoFactory.h
        config/ConfigSerializer.cpp
        config/ConfigSerializer.hpp
        config/ConfigFactory.cpp
        config/ConfigFactory.hpp
)
target_link_libraries(catan_dto PUBLIC catan_proto)
target_link_libraries(catan_dto PUBLIC catan_game)

target_include_directories(catan_dto
        PUBLIC
        .
)
