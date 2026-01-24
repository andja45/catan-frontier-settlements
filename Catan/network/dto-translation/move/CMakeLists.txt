

add_library(catan_networkmoves
        MoveProtoFactory.cpp
        MoveProtoFactory.h
        MoveProtoSerializer.cpp
        MoveProtoSerializer.h
)
target_link_libraries(catan_networkmoves PUBLIC catan_proto)
target_link_libraries(catan_networkmoves PUBLIC catan_game)

target_include_directories(catan_networkmoves
        PUBLIC
        .
)
