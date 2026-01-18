//
// Created by andja on 10.12.25..
//

#ifndef BUILDSETTLEMENTMOVE_H
#define BUILDSETTLEMENTMOVE_H

#include <unordered_set>

#include "../Move.h"

class BuildSettlementMove final : public Move{
private:
    NodeId m_nodeId; // or HexCoords + nodeIndex
public:
    BuildSettlementMove(const PlayerId playerId, const NodeId nodeId) : Move(playerId), m_nodeId(nodeId) {}

    MoveType type() const override { return MoveType::BuildSettlement; }
    bool isValid(const GameSession&) const override;
    void apply(GameSession&) const override;

    bool providesAllValid() const override { return true; }
    std::unordered_set<NodeId> allValid(const GameSession& session) const;
};


#endif //BUILDSETTLEMENTMOVE_H