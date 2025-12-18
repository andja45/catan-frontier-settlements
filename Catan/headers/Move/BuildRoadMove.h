//
// Created by andja on 10.12.25..
//

#ifndef BUILDROADMOVE_H
#define BUILDROADMOVE_H

#include "Move.h"


class BuildRoadMove : public Move{
private:
    int m_playerId;
    int m_edgeId; // ili HexCoords + edgeIndex
public:
    BuildRoadMove(const int playerId, const int edgeId) : m_playerId(playerId), m_edgeId(edgeId) {}

    bool isValid(const GameModel&, const GameSession&) const override;
    void apply(GameModel&, GameSession&) const override;
};


#endif //BUILDROADMOVE_H