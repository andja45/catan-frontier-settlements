//
// Created by andja on 10.12.25..
//

#include "../../../headers/Move/BuildRoadMove.h"

bool BuildRoadMove::isValid(const GameModel& model, const GameSession& session) const {
    if (!session.isPlayersTurn(m_playerId)) return false;
    if (!session.canBuild()) return false;

    // da li je pozicija validna (pravila)
    if (!model.canPlayerBuildRoad(m_playerId, m_edgeId)) return false; // TODO dodaj u model
    if (!model.hasResources(m_playerId, Costs::Road)) return false;

    return true;
}

void BuildRoadMove::apply(GameModel& model, GameSession&) const {
    model.consumeResources(m_playerId, Costs::Road);
    model.placeRoad(m_playerId, m_edgeId); // TODO dodaj u model
}