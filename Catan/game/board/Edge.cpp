//
// Created by matija on 1/16/26.
//

#include "Edge.h"

bool Edge::hasTrade() const {
    if (getStart()->hasTrade()&&getEnd()->hasTrade()) return true;
    return false;
}

TradeType Edge::getTradeType() {
    if (!hasTrade()) return TradeType::None;
    for (auto n:getNodes()) {
        return n->getTradeResource();
    }
    return ResourceType::None;
}

EdgeDirection Edge::getDirection() {
    return m_edgeCoords.direction();
}
