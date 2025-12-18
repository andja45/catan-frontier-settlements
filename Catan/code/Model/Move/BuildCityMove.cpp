//
// Created by andja on 10.12.25..
//

#include "../../../headers/Move/BuildCityMove.h"

bool BuildCityMove::isValid(const GameModel& model, const GameSession& session) const {
    if (!session.isPlayersTurn(m_playerId)) return false;
    if (!session.canBuild()) return false;

    // da li je pozicija validna (pravila)
    if (!model.canPlayerBuildCity(m_playerId, m_nodeId)) return false; // TODO dodaj u model
        /*      tu
        if (node->getOwner() == nullptr) return false;
        if (node->getOwner()->getPlayerId() != _playerId) return false;
        if (node->getNodeType() != NodeType::Settlement) return false;
        */

    if (!model.hasResources(m_playerId, Costs::City)) return false;

    return true;
}

void BuildCityMove::apply(GameModel& model, GameSession&) const {
    model.consumeResources(m_playerId, Costs::City);
    model.placeCity(m_playerId, m_nodeId);
}