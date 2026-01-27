#ifndef TOOLBARACTIONTYPE_H
#define TOOLBARACTIONTYPE_H
#include<array>
#include <string>
#include <map>
enum class ToolbarActionType {
    BuildRoad,
    BuildSettlement,
    BuildCity,
    BuyDevCard,
    PlayDevCard,
    PlayerTrade,
    BankTrade,
    RollDice,
    EndTurn
};
static const inline std::map<ToolbarActionType, std::string> nameFromToolbarActionType = {
    {ToolbarActionType::BuildRoad, "Build Road"},
    {ToolbarActionType::BuildSettlement, "Build Settlement"},
    {ToolbarActionType::BuyDevCard, "Buy Dev"},
    {ToolbarActionType::PlayDevCard, "Play Dev"},
    {ToolbarActionType::PlayerTrade, "Player Trade "},
    {ToolbarActionType::BankTrade, "Bank Trade"},
    {ToolbarActionType::EndTurn, "End Turn "},
    {ToolbarActionType::RollDice, "Roll Dice"}

};
static const std::array<ToolbarActionType, 9> ToolbarActionTypes= {
    ToolbarActionType::BuildRoad,
    ToolbarActionType::BuildSettlement,
    ToolbarActionType::BuildCity,
    ToolbarActionType::BuyDevCard,
    ToolbarActionType::PlayDevCard,
    ToolbarActionType::PlayerTrade,
    ToolbarActionType::BankTrade,
    ToolbarActionType::RollDice,
    ToolbarActionType::EndTurn
};
#endif // TOOLBARACTIONTYPE_H
