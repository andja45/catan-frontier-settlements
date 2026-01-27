//
// Created by matija on 1/24/26.
//

#include "BoardSerializer.hpp"

net::ResourceType BoardSerializer::toProto(ResourceType r) {
    return static_cast<net::ResourceType>(r);
}

net::BoardInfo::TileInfo BoardSerializer::serializeTile(const TileDef &t)  {
    net::BoardInfo::TileInfo proto;
    proto.set_q(t.q);
    proto.set_r(t.r);
    proto.set_resource(toProto(t.res));
    proto.set_tile_number(t.number);
    return proto;
}

net::BoardInfo::PortInfo BoardSerializer::serializePort(const PortDef &p)  {
    net::BoardInfo::PortInfo proto;
    proto.set_q(p.q);
    proto.set_r(p.r);
    proto.set_i(p.i);
    proto.set_resource(toProto(static_cast<ResourceType>(p.tradeType)));
    return proto;
}

net::BoardInfo BoardSerializer::toProto(const Board &board) {
    net::BoardInfo proto;
    // serialize tiles
    for (const auto&t: board.getTileDefs()) {
        *proto.add_tiles() = serializeTile(t);

    }
    // serialize ports
    for (const auto& p : board.getPortDefs()) {
        *proto.add_ports() = serializePort(p);
    }
    return proto;
}
