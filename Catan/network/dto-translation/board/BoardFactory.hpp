//
// Created by matija on 1/24/26.
//

#ifndef CATAN_BOARDFACTORY_HPP
#define CATAN_BOARDFACTORY_HPP

#include "ProtoHeaders.hpp"
#include <board/Board.h>

class BoardFactory {
private:
    static TileDef makeTile(const net::BoardInfo::TileInfo& proto);
    static PortDef makePort(const net::BoardInfo::PortInfo& proto);
public:
    static ResourceType fromProto(net::ResourceType r);
    static std::unique_ptr<Board> fromProto(const net::BoardInfo& proto);
};

#endif //CATAN_BOARDFACTORY_HPP