//
// Created by andja on 10.12.25..
//

#ifndef BUILDCITYMOVE_H
#define BUILDCITYMOVE_H

#include <unordered_set>

#include "move/BoardMove.h"

class BuildCityMove final : public BoardMove{
private:
    NodeId m_nodeId; // or HexCoords + nodeIndex
public:
    BuildCityMove(const PlayerId playerId, const NodeId nodeId) : BoardMove(playerId), m_nodeId(nodeId) {}

    MoveType type() const override { return MoveType::BuildCity; }
    bool isValid(const GameSession& session) const override;
    void apply(GameSession& session) const override;

    bool providesAllValid() const override { return true; }
    std::unordered_set<NodeId> allValid(const GameSession &session) const;

    NodeId getNodeId() const {return m_nodeId;}
    void setBoardElementId(int elementId) { m_nodeId = static_cast<NodeId>(elementId); }
};


#endif //BUILDCITYMOVE_H