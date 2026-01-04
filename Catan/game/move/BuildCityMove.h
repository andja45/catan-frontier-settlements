//
// Created by andja on 10.12.25..
//

#ifndef BUILDCITYMOVE_H
#define BUILDCITYMOVE_H

#include "Move.h"


class BuildCityMove : public Move{
private:
    PlayerId m_playerId;
    NodeId m_nodeId; // or HexCoords + nodeIndex
public:
    BuildCityMove(const PlayerId playerId, const NodeId nodeId) : m_playerId(playerId), m_nodeId(nodeId) {}

    MoveType type() const override { return MoveType::BuildCity; }
    bool isValid(const GameSession& session) const override;
    void apply(GameSession& session) const override;
};


#endif //BUILDCITYMOVE_H