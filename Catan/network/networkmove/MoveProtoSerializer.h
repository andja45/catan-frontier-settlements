//
// Created by jovan on 20/01/2026.
//

#ifndef CATAN_MOVEPROTOSERIALIZER_H
#define CATAN_MOVEPROTOSERIALIZER_H

#include "common.pb.h"
#include "move.pb.h"
#include "move/Move.h"


class MoveProtoSerializer {
public:
    static catan::Move toProto(const Move& move);
};

#endif //CATAN_MOVEPROTOSERIALIZER_H