//
// Created by andja on 10.12.25..
//

#include "../../../headers/Move/BuildSettlementMove.h"

#include "../../../headers/Game/GameModel.h"

bool BuildSettlementMove::isValid(const GameSession& session) const {
    if (!session.isPlayersTurn(m_playerId)) return false;
    if (!session.canBuild()) return false;

    const GameModel& model = session.model();
    // da li je pozicija validna (pravila)
    if (!model.canPlaceSettlement(m_playerId, m_nodeId, session.isInitialPlacement())) return false; // TODO dodaj u model
    if (!model.hasResources(m_playerId, Costs::Settlement)) return false;

    return true;
}

void BuildSettlementMove::apply(GameSession& session) const {
    GameModel& model = session.model();

    model.consumeResources(m_playerId, Costs::Settlement);
    model.placeSettlement(m_playerId, m_nodeId);
}