//
// Created by andja on 10.12.25..
//

#ifndef MOVE_H
#define MOVE_H

/*enum class MoveType {
    BuildRoad,
    BuildSettlement,
    BuildCity,
    SettingRobber,
    BankTrade,
    PlayerTrade,
    RollDice,
    EndTurn
};*/

#include "../types/TypeAliases.h"
#include "../types/Costs.h"
#include "../types/ResourceType.h"
#include "../gamemodel/GameModel.h"
#include "../gamemodel/GameSession.h"

class Move {
public:
    virtual ~Move() = default;

    virtual bool isValid(const GameSession&) const = 0;
    virtual void apply(GameSession&) const = 0;
};


#endif //MOVE_H