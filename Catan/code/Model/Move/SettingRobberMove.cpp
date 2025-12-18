//
// Created by andja on 17.12.25..
//

#include "../../../headers/Move/SettingRobberMove.h"

bool SettingRobberMove::isValid(const GameModel& model, const GameSession& session) const{
    if (!session.isPlayersTurn(m_playerId))
        return false;

    if (!session.canPlaceRobber)
        return false;

    const Tile* tile = model.board().getTileById(m_tileId);
    if (!tile)
        return false;

    if (tile->isRobberOnTile())
        return false;

    if (m_victimPlayerId != -1) {
        const Player& victim = model.player(m_victimPlayerId);

        if (m_victimPlayerId == m_playerId)
            return false;

        if (victim.getNumOfResourceCards() == 0)
            return false;
    }

    return true;
}

void SettingRobberMove::apply(GameModel& model, GameSession& session) const{
    model.board().moveRobberTo(m_tileId);

    if (m_victimPlayerId != -1) {
        Player& victim = model.player(m_victimPlayerId);
        Player& thief  = model.player(m_playerId);

        ResourceType stolen = victim.takeRandomResource();
        thief.addResource(stolen, 1);
    }

    session.enterMainPhase();
}
