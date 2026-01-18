//
// Created by andja on 10.12.25..
//

#ifndef ENDTURNMOVE_H
#define ENDTURNMOVE_H

#include "../Move.h"


class EndTurnMove final : public Move{
private:
    PlayerId m_playerId;
public:
    explicit EndTurnMove(PlayerId playerId) : Move(playerId) {}

    MoveType type() const override { return MoveType::EndTurn; }
    bool isValid(const GameSession&) const override;
    void apply(GameSession&) const override;
};


#endif //ENDTURNMOVE_H