 //
// Created by andja on 10.12.25..
//

#ifndef BUILDSETTLEMENTMOVE_H
#define BUILDSETTLEMENTMOVE_H

#include <unordered_set>

#include "move/BoardMove.h"

class BuildSettlementMove final : public BoardMove{
private:
    NodeId m_nodeId; // or HexCoords + nodeIndex
public:
    BuildSettlementMove(const PlayerId playerId, const NodeId nodeId) : BoardMove(playerId), m_nodeId(nodeId) {}

    MoveType type() const override { return MoveType::BuildSettlement; }
    bool isValid(const GameSession&) const override;
    void apply(GameSession&) const override;

    bool providesAllValid() const override { return true; }
    std::unordered_set<NodeId> allValid(const GameSession& session) const;

    NodeId getNodeId() const { return m_nodeId; }
    void setBoardElementId(int elementId) { m_nodeId = static_cast<NodeId>(elementId); }
};


#endif //BUILDSETTLEMENTMOVE_H