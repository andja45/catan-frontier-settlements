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
public:
     static net::ResourceType toProto(ResourceType type);
     static net::TileInfo serializeTile(const TileDef& t);
     static net::PortInfo serializePort(const PortDef& p);

     static net::BoardInfo serializeBoard(const Board& board);

};

#endif //CATAN_BOARDSERIALIZER_HPP