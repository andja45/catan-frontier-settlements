//
// Created by andja on 10.12.25.
//

#include "BuildRoadMove.h"
#include "gamemodel/GameSession.h"

bool BuildRoadMove::isValid(const GameSession& session) const {
    const Board& board = session.board();
    const Player& player = session.player(m_playerId);

    if (session.currentPlayer() != m_playerId) // TODO game should be playable even without multiplayer, but in gui we will set buttons unclickable if currplayer != localplayer cus only he can make moves on his gui, other clients send him their moves
        return false;

    if (!player.hasRoadLeft())
        return false;

    if (!board.isEdgeFree(m_edgeId))
        return false;

    // connected check, if phase is correct
    bool connected = false;
    if (session.phase() == TurnPhase::Main) {
        if (!player.canAfford(Costs::Road))
            return false;

        connected =
            board.edgeTouchesPlayerHouse(m_playerId, m_edgeId) ||
            board.edgeTouchesPlayerRoad(m_playerId, m_edgeId);
    }
    else if (session.phase() == TurnPhase::InitialPlacement &&
        session.initialPlacementStep() == InitialPlacementStep::PlaceRoad){ // or both phases in separate check at start and this just in else
        const auto& houses = player.getHouses();
        if (houses.empty())
            return false;

        Node* lastSettlement = houses.back(); // TODO or just function that remembers lastbuiltSettlement, if we do computeLongestRoad in board (update will be in session)
        connected = board.edgeTouchesPlayerSettlement(lastSettlement->getOwner(), m_edgeId);
    }

    if (!connected)
        return false;

    return true;
}

void BuildRoadMove::apply(GameSession& session) const {
    Board& board = session.board();
    Player& player = session.player(m_playerId);

    if (session.phase() == TurnPhase::Main) {
        player.spendResources(Costs::Road);
    }

    Edge* edge= session.board().getEdgeById(m_edgeId);
    player.addRoad(edge); // adds to list of that players edges and decrements numofroads left
    board.placeRoad(m_playerId, m_edgeId);

    session.updateLongestRoad(); // only then can longestroad be changed
}