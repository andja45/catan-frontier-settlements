//
// Created by andja on 17.12.25.
//

#include "SetRobberMove.h"
#include "model/GameSession.h"

bool SetRobberMove::isValid(const GameSession& session) const {
    const Board& board = session.board();

    if (session.currentPlayer() != m_playerId)
        return false;

    if (session.phase() != TurnPhase::SetRobber)
        return false;

    if (m_tileId == types::InvalidTileId) // it has to be a valid tile TODO maybe later on set that -700,800 tile id is invalid, we just check if id -1
        return false;

    if (board.isRobberOnTile(m_tileId)) // cant set on same tile
        return false;

    return true;
}

void SetRobberMove::apply(GameSession& session) const {
    Board& board = session.board();

    board.placeRobber(m_tileId);
}

std::unordered_set<TileId> SetRobberMove::allValid(const GameSession& session) const {
    std::unordered_set<TileId> validTiles;

    const Board& board = session.board();
    for (TileId tileId : board.tileIds()) {
        SetRobberMove testMove(*this);
        testMove.m_tileId = tileId;

        if (testMove.isValid(session))
            validTiles.insert(tileId);
    }

    return validTiles;
} // all board but lastrobbertile will light up
