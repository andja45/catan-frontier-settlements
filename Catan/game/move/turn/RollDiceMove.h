//
// Created by andja on 10.12.25..
//

#ifndef ROLLDICEMOVE_H
#define ROLLDICEMOVE_H

#include "../Move.h"
#include "../../../network/dto-translation/move/MoveProtoSerializer.h"

class RollDiceMove final : public Move{
friend MoveProtoSerializer;
friend MoveProtoFactory;
private:
    int m_diceRoll1;
    int m_diceRoll2;
public:
    explicit RollDiceMove(PlayerId playerId) : Move(playerId) {
        m_diceRoll1 = 0;
        m_diceRoll2 = 0;
    }
    void setDiceRoll(int d1, int d2){m_diceRoll1=d1; m_diceRoll2=d2;}
    void setDiceRoll(GameSession& session);
    std::pair<int,int> getDiceRoll() const { return {m_diceRoll1, m_diceRoll2}; }
    MoveType type() const override { return MoveType::RollDice; }
    bool isValid(const GameSession&) const override;
    void apply(GameSession&) const override;
};


#endif //ROLLDICEMOVE_H