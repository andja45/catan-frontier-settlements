//
// Created by andja on 17.1.26.
//

#include "PlayerTradeAcceptMove.h"
#include "gamemodel/GameSession.h"
#include "gamemodel/Trade.h"

bool PlayerTradeAcceptMove::isValid(const GameSession& session) const {
    const Trade* trade = session.getTrade(m_tradeId);
    if (!trade)
        return false;

    if (session.currentPlayer() != m_playerId) // he can only accept, he sent the trade
        return false;

    if (trade->requester() != m_playerId) // he also sent the trade
        return false;

    if (session.phase() != TurnPhase::Main)
        return false;

    if (!trade->hasAccepted(m_acceptedPlayerId))
        return false;

    const Player& requester = session.player(trade->requester());
    const Player& acceptedPlayer = session.player(m_acceptedPlayerId);

    // for safety
    if (!requester.hasResources(trade->give()))
        return false;

    if (!acceptedPlayer.hasResources(trade->receive()))
        return false;

    return true;
}

void PlayerTradeAcceptMove::apply(GameSession& session) const {
    Trade* trade = session.getTrade(m_tradeId);
    if (!trade)
        return;

    Player& requester = session.player(trade->requester());
    Player& acceptedPlayer = session.player(m_acceptedPlayerId);

    // Requester -> Accepted Player
    requester.removeResources(trade->give());
    acceptedPlayer.addResources(trade->give());

    // Accepted Player -> Requester
    acceptedPlayer.removeResources(trade->receive());
    requester.addResources(trade->receive());

    session.removeTrade(m_tradeId); // this trade is finished
}

std::unordered_set<TileId> PlayerTradeAcceptMove::allValid(const GameSession& session) const {
    std::unordered_set<PlayerId> validPlayers;

    for (PlayerId playerId : session.playerIds()) {
        if (playerId == m_playerId)
            continue;

        PlayerTradeAcceptMove testMove(*this);
        testMove.m_acceptedPlayerId = playerId;

        if (testMove.isValid(session))
            validPlayers.insert(playerId);
    }

    return validPlayers;
}
