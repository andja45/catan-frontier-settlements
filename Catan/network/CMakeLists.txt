cmake_minimum_required(VERSION 3.10)
project(CatanNetwork)

# 1. Pronalaženje Protobuf paketa
find_package(Protobuf REQUIRED)

# 2. Definisanje putanja do .proto fajlova
# Pretpostavljamo da su moves.proto i common.proto u istom ovom folderu
set(PROTO_FILES
        proto/common.proto
        proto/moves.proto
)

# 3. Komanda koja automatski generiše .pb.cc i .pb.h fajlove tokom build-a
# PROTO_SRCS će sadržati listu .cc fajlova, a PROTO_HDRS listu .h fajlova
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

# 4. Kreiranje biblioteke od tvojih fajlova i generisanog koda
add_library(network_lib STATIC
        MoveProtoFactory.cpp
        MoveProtoFactory.h
        MoveProtoSerializer.cpp
        MoveProtoSerializer.h
        ${PROTO_SRCS}
        ${PROTO_HDRS}
)

# 5. Povezivanje sa eksternom Protobuf bibliotekom
# PUBLIC znači da će svako ko koristi network_lib automatski dobiti i Protobuf headere
target_link_libraries(network_lib PUBLIC protobuf::libprotobuf)

# 6. Postavljanje include putanja
# Ovo omogućava da radi #include "MoveProtoFactory.h" bez komplikovanih putanja
target_include_directories(network_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Opciono: Ako želiš da se generisani fajlovi vide u IDE-u (CLion)
source_group("Generated" FILES ${PROTO_SRCS} ${PROTO_HDRS})