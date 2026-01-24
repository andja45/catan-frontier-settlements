//
// Created by jovan on 19/01/2026.
//

#ifndef CATAN_MOVEPROTOFACTORY_H
#define CATAN_MOVEPROTOFACTORY_H

#include <memory>
#include "move/Move.h"

namespace catan {
    class Move;
}

class MoveProtoFactory {
public:
    static std::unique_ptr<Move> fromProto(const catan::Move& proto);
};


#endif //CATAN_MOVEPROTOFACTORY_H