//
// Created by andja on 10.12.25..
//

#include "BuildCityMove.h"
#include "gamemodel/GameSession.h"


bool BuildCityMove::isValid(const GameSession& session) const {
    const Board& board = session.board();
    const Player& player = session.player(m_playerId);

    if (session.currentPlayer() != m_playerId) // TODO game should be playable even without multiplayer, but in gui we will set buttons unclickable if currplayer != localplayer cus only he can make moves on his gui, other clients send him their moves
        return false;

    if (session.phase() != TurnPhase::Main) // only happens in main
        return false;

    if (!player.hasCityLeft())
        return false;

    if (!player.hasResources(Costs::Settlement))
        return false;

    if (!board.isBuildingOwnedBy(m_playerId, m_nodeId)) // is settlement + playerid is owner
        return false;

    return true;
}

void BuildCityMove::apply(GameSession& session) const {
    Board& board = session.board();
    Player& player = session.player(m_playerId);

    player.removeResources(Costs::City);

    player.addPoints(1);

    board.placeCity(m_playerId, m_nodeId);
}