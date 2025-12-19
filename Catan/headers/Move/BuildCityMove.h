//
// Created by andja on 10.12.25..
//

#ifndef BUILDCITYMOVE_H
#define BUILDCITYMOVE_H

#include "Move.h"


class BuildCityMove : public Move{
private:
    int m_playerId;
    int m_nodeId; // ili HexCoords + nodeIndex
public:
    BuildCityMove(const int playerId, const int nodeId) : m_playerId(playerId), m_nodeId(nodeId) {}

    bool isValid(const GameSession& session) const override;
    void apply(GameSession& session) const override;
};


#endif //BUILDCITYMOVE_H