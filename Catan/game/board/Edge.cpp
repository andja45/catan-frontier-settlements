//
// Created by matija on 1/16/26.
//

#include "Edge.h"

bool Edge::hasTrade() const {
    for (auto n:getNodes()) {
        if (n->isTrade()) return true;
    }
    return false;
}

TradeType Edge::getTradeType() {
    if (!hasTrade()) return TradeType::None;
    for (auto n:getNodes()) {
        return n->getTradeResource();
    }
}

EdgeDirection Edge::getDirection() {
    return m_edgeCoords.direction();
}
