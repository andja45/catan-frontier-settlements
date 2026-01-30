//
// Created by andja on 16.1.26..
//

#ifndef CATAN_STEALCARD_H
#define CATAN_STEALCARD_H

#include <unordered_set>

#include "move/BoardMove.h"

class StealCardMove final : public BoardMove {
private:
    PlayerId m_victimPlayerId;
public:
    StealCardMove(PlayerId playerId, PlayerId victimPlayerId)
        : BoardMove(playerId), m_victimPlayerId(victimPlayerId) {}

    MoveType type() const override { return MoveType::StealCard; }
    bool isValid(const GameSession& session) const override;
    void apply(GameSession& session) const override;

    bool providesAllValid() const override { return true; }
    std::unordered_set<PlayerId> allValid(const GameSession &session) const; // for making gray players he cant chose
    PlayerId getVictimPlayerId() const {return m_victimPlayerId; }

    void setBoardElementId(int elementId) { m_victimPlayerId = static_cast<PlayerId>(elementId); }
};


#endif //CATAN_STEALCARD_H