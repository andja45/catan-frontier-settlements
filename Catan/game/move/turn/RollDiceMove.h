//
// Created by andja on 10.12.25..
//

#ifndef ROLLDICEMOVE_H
#define ROLLDICEMOVE_H

#include "../Move.h"


class RollDiceMove final : public Move{
private:
    int m_diceRoll1;
    int m_diceRoll2;
public:
    explicit RollDiceMove(PlayerId playerId) : Move(playerId) {
        m_diceRoll1 = 0;
        m_diceRoll2 = 0;
    }
    // RollDiceMove move = 3; jer ne zelimo da moze to, samo je generisanje dozvoljeno
    void setDiceRoll(std::pair<int,int> roll) { m_diceRoll1 = roll.first; m_diceRoll2 = roll.first; }
    std::pair<int,int> getDiceRoll() const { return {m_diceRoll1, m_diceRoll2}; }
    MoveType type() const override { return MoveType::RollDice; }
    bool isValid(const GameSession&) const override;
    void apply(GameSession&) const override;
};


#endif //ROLLDICEMOVE_H