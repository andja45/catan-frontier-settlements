//
// Created by andja on 17.12.25.
//

#include "../../../headers/Move/SettingRobberMove.h"

bool SettingRobberMove::isValid(const GameSession& session) const{
    if (!session.isPlayersTurn(m_playerId))
        return false;
    if (!session.canPlaceRobber())
        return false;

    const GameModel& model = session.model();
    // po pravilima igre
    if (!model.canPlaceRobber(m_tileId))
        return false;
    if (m_victimPlayerId != -1) {
        if (!model.canStealFrom(m_playerId, m_victimPlayerId))
            return false;
    }

    return true;
}

void SettingRobberMove::apply(GameSession& session) const{
    GameModel& model = session.model();
    model.placeRobber(m_tileId);
    if (m_victimPlayerId != -1) {
        model.stealRandomResource(m_playerId, m_victimPlayerId);
    }

    session.enterMainPhase();
}