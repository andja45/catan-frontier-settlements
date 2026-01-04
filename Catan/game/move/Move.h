//
// Created by andja on 10.12.25..
//

#ifndef MOVE_H
#define MOVE_H

enum class MoveType {
    RollDice,
    EndTurn,

    BuildRoad,
    BuildSettlement,
    BuildCity,

    SetRobber,
    StealCard, // in one move?
    DiscardCards, // if more than 7 cards

    BankTrade,
    PlayerTrade,

    BuyDevCard,
    PlayDevCard
};

#include "../types/TypeAliases.h"
#include "../types/Costs.h"
#include "../types/ResourceType.h"

class GameSession;

class Move {
public:
    virtual ~Move() = default;

    virtual MoveType type() const = 0;
    virtual bool isValid(const GameSession&) const = 0;
    virtual void apply(GameSession&) const = 0;
};


#endif //MOVE_H