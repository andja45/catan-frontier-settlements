find_package(Protobuf REQUIRED)

set(PROTO_FILES
        common.proto
        move.proto
        envelope.proto
        setup.proto
)
set(PROTO_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)


protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES}
#        PROTOC_OUT_DIR ${PROTO_GEN_DIR}
)

add_library(catan_proto STATIC
        ${PROTO_SRCS}
        ${PROTO_HDRS}
)

target_link_libraries(catan_proto PUBLIC protobuf::libprotobuf)

target_include_directories(catan_proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)


source_group("Generated" FILES ${PROTO_SRCS} ${PROTO_HDRS})

