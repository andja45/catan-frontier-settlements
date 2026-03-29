set(protobuf_MODULE_COMPATIBLE TRUE)

find_package(protobuf CONFIG QUIET)
if(protobuf_FOUND)
    set(CATAN_USE_CONAN_PROTOBUF TRUE)
else()
    find_package(Protobuf REQUIRED)
    set(CATAN_USE_CONAN_PROTOBUF FALSE)
endif()

set(PROTO_FILES
    common.proto
    move.proto
    envelope.proto
    setup.proto
)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

add_library(catan_proto STATIC
    ${PROTO_SRCS}
    ${PROTO_HDRS}
    ProtoHeaders.hpp
)

if(CATAN_USE_CONAN_PROTOBUF)
    target_link_libraries(catan_proto PUBLIC protobuf::libprotobuf)
else()
    target_link_libraries(catan_proto PUBLIC ${Protobuf_LIBRARIES})
    target_include_directories(catan_proto PUBLIC ${Protobuf_INCLUDE_DIRS})
endif()

target_include_directories(catan_proto PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    .
)

source_group("Generated" FILES ${PROTO_SRCS} ${PROTO_HDRS})

