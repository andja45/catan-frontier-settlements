//
// Created by andja on 10.12.25..
//

#include "../../../headers/Move/BuildSettlementMove.h"

#include "../../../headers/Game/GameModel.h"

bool BuildSettlementMove::isValid(const GameModel& model, const GameSession& session) const {
    if (!session.isPlayersTurn(m_playerId)) return false;
    if (!session.canBuild()) return false;

    // da li je pozicija validna (pravila)
    if (!model.canPlayerBuildSettlement(m_playerId, m_nodeId)) return false; // TODO dodaj u model
    if (!model.hasResources(m_playerId, Costs::Settlement)) return false;

    return true;
}

void BuildSettlementMove::apply(GameModel& model, GameSession&) const {
    model.consumeResources(m_playerId, Costs::Settlement);
    model.placeSettlement(m_playerId, m_nodeId); // TODO dodaj u model
}