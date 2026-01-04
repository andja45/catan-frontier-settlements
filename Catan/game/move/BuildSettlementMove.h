//
// Created by andja on 10.12.25..
//

#ifndef BUILDSETTLEMENTMOVE_H
#define BUILDSETTLEMENTMOVE_H

#include "Move.h"


class BuildSettlementMove final : public Move{
private:
    PlayerId m_playerId;
    NodeId m_nodeId; // or HexCoords + nodeIndex
public:
    BuildSettlementMove(const PlayerId playerId, const NodeId nodeId) : m_playerId(playerId), m_nodeId(nodeId) {}

    MoveType type() const override { return MoveType::BuildSettlement; }
    bool isValid(const GameSession&) const override;
    void apply(GameSession&) const override;
};


#endif //BUILDSETTLEMENTMOVE_H