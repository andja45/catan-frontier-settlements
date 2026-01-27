//
// Created by andja on 16.1.26..
//

#ifndef CATAN_MOVETYPE_H
#define CATAN_MOVETYPE_H

enum class MoveType {
    // TURN:
    RollDice,
    EndTurn,

    // BUILD:
    BuildRoad,
    BuildSettlement,
    BuildCity,

    // ROBBER/KNIGHT:
    DiscardCards, // if more than 7 cards
    SetRobber,
    StealCard,

    // DEVCARDS:
    BuyDevCard,
    PlayDevCard,

    YearOfPlenty,
    Monopoly,

    // TRADES:
    BankTrade,
    PlayerTradeRequest,
    PlayerTradeResponse,
    PlayerTradeAccept,

    PlayerLeave,

    InvalidMoveType,
};

#endif //CATAN_MOVETYPE_H