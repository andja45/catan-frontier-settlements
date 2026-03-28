find_package(Protobuf CONFIG)
if(NOT Protobuf_FOUND)
        find_package(Protobuf REQUIRED)
endif()

if(TARGET protobuf::protoc)
        set(Protobuf_PROTOC_EXECUTABLE $<TARGET_FILE:protobuf::protoc>)
endif()

set(PROTO_FILES
        common.proto
        move.proto
        envelope.proto
        setup.proto
)
set(PROTO_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

add_library(catan_proto STATIC
        ${PROTO_SRCS}
        ${PROTO_HDRS}
        ProtoHeaders.hpp
)

if(TARGET protobuf::protobuf)
        target_link_libraries(catan_proto PUBLIC protobuf::protobuf)
else()
        target_link_libraries(catan_proto PUBLIC ${Protobuf_LIBRARIES})
endif()

target_include_directories(catan_proto PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        .
        ${Protobuf_INCLUDE_DIRS}
)

source_group("Generated" FILES ${PROTO_SRCS} ${PROTO_HDRS})

