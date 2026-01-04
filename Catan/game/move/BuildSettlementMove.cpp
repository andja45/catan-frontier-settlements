//
// Created by andja on 10.12.25..
//

#include "BuildSettlementMove.h"

#include "../gamemodel/GameModel.h"

bool BuildSettlementMove::isValid(const GameSession& session) const {
    const Board& board = session.board();
    const Player& player = session.player(m_playerId);

    if (session.currentPlayer() != m_playerId) // TODO game should be playable even without multiplayer, but in gui we will set buttons unclickable if currplayer != localplayer cus only he can make moves on his gui, other clients send him their moves
        return false;

    if (!player.hasSettlementLeft())
        return false;

    if (!player.canAfford(Costs::Settlement))
        return false;

    if (!board.isNodeFree(m_nodeId))
        return false;

    if (board.nodeTouchesAnySettlement(m_nodeId))
        return false;

    bool connected = false;
    if (session.phase() == TurnPhase::Main) {
        if (!player.canAfford(Costs::Road))
            return false;

        connected =
            board.nodeTouchesPlayerRoad(m_playerId, m_nodeId);
    }
    else if (session.phase() == TurnPhase::InitialPlacement &&
        session.initialPlacementStep() == InitialPlacementStep::PlaceRoad){ // or both phases in separate check at start and this just in else
        return true; // has no rules other than settlement distance
        }

    if (!connected)
        return false;

    return true;
}

void BuildSettlementMove::apply(GameSession& session) const {
    Board& board = session.board();
    Player& player = session.player(m_playerId);

    if (session.phase() == TurnPhase::Main) {
        player.spendResources(Costs::Settlement);
    }

    player.addVictoryPoints(1);

    Node* node = session.board().getNodeById(m_nodeId);
    player.addSettlement(node); // adds to list of that players houses and decrements numofsettlements left
    board.placeSettlement(m_playerId, m_nodeId);
}