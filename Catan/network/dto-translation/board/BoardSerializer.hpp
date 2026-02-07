//
// Created by matija on 1/24/26.
//

#ifndef CATAN_BOARDSERIALIZER_HPP
#define CATAN_BOARDSERIALIZER_HPP

#include <board/Board.h>

#include "move.pb.h"
#include "setup.pb.h"
#include "common.pb.h"
#include "envelope.pb.h"

class BoardSerializer {
private:
     static net::BoardInfo::PortInfo serializePort(const PortDef& p);

     static net::BoardInfo::TileInfo serializeTile(const TileDef& t);

public:
     static net::ResourceType toProto(ResourceType type);
     static net::BoardInfo toProto(const Board& board);

};

#endif //CATAN_BOARDSERIALIZER_HPP