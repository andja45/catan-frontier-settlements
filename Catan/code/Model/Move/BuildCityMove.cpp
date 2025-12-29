//
// Created by andja on 10.12.25..
//

#include "../../../headers/Move/BuildCityMove.h"

bool BuildCityMove::isValid(const GameSession& session) const {
    if (!session.isPlayersTurn(m_playerId)) return false;
    if (!session.canBuild()) return false;

    const GameModel& model = session.model();
    // da li je pozicija validna (pravila)
    if (!model.canPlaceCity(m_playerId, m_nodeId)) return false;

    if (!model.hasResources(m_playerId, Costs::City)) return false;

    return true;
}

void BuildCityMove::apply(GameSession& session) const {
    GameModel& model = session.model();

    model.consumeResources(m_playerId, Costs::City);
    model.placeCity(m_playerId, m_nodeId);
}