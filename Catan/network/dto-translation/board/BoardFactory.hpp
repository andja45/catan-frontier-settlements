//
// Created by matija on 1/24/26.
//

#ifndef CATAN_BOARDFACTORY_HPP
#define CATAN_BOARDFACTORY_HPP

#include "ProtoHeaders.hpp"
#include <board/Board.h>

class BoardFactory {
    static ResourceType fromProto(net::ResourceType r);
    static TileDef makeTile(const net::TileInfo& proto);
    static PortDef makePort(const net::PortInfo& proto);
    static std::unique_ptr<Board> makeBoard(const net::BoardInfo& proto);
};

#endif //CATAN_BOARDFACTORY_HPP