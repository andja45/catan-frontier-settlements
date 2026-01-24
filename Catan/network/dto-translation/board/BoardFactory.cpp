//
// Created by matija on 1/24/26.
//

#include "BoardFactory.hpp"

ResourceType BoardFactory::fromProto(net::ResourceType r) {
    return static_cast<ResourceType>(r);
}

TileDef BoardFactory::makeTile(const net::TileInfo &proto) {
    return TileDef{
        .q = proto.q(),
        .r = proto.r(),
        .res = fromProto(proto.resource()),
        .number = proto.tile_number()
    };
}

PortDef BoardFactory::makePort(const net::PortInfo &proto) {
    return PortDef{
        .q = proto.q(),
        .r = proto.r(),
        .i = proto.i(),
        .tradeType = fromProto(proto.resource())
    };
}

std::unique_ptr<Board> BoardFactory::makeBoard(const net::BoardInfo &proto) {

    std::vector<TileDef> tiles;
    tiles.reserve(proto.tiles_size());

    for (const auto& t : proto.tiles()) {
        tiles.push_back(makeTile(t));
    }

    std::vector<PortDef> ports;
    ports.reserve(proto.ports_size());

    for (const auto& p : proto.ports()) {
        ports.push_back(makePort(p));
    }

    auto board= std::make_unique<Board>();
    board->initializeBoard(tiles);
    board->loadTrades(ports);
    return std::move(board);
}
