//
// Created by matija on 1/27/26.
//

#ifndef CATAN_PLAYERLEAVE_HPP
#define CATAN_PLAYERLEAVE_HPP
#include <move/Move.h>

class PlayerLeave:public Move{
private:
public:
    explicit PlayerLeave(PlayerId playerId) : Move(playerId) {}

    MoveType type() const override {return MoveType::PlayerLeave;}
    bool isValid(const GameSession &) const override;
    void apply(GameSession &) const override;
};

#endif //CATAN_PLAYERLEAVE_HPP