//
// Created by andja on 10.12.25..
//

#include "../../../headers/Move/BuildRoadMove.h"

bool BuildRoadMove::isValid(const GameSession& session) const {
    if (!session.isPlayersTurn(m_playerId)) return false;
    if (!session.canBuild()) return false;

    GameModel& model = session.model();
    // da li je pozicija validna (pravila)
    if (!model.canPlaceRoad(m_playerId, m_edgeId)) return false;
    if (!model.hasResources(m_playerId, Costs::Road)) return false;

    return true;
}

void BuildRoadMove::apply(GameSession& session) const {
    GameModel& model = session.model();

    model.consumeResources(m_playerId, Costs::Road);
    model.placeRoad(m_playerId, m_edgeId);
}