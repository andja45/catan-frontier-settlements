//
// Created by andja on 17.1.26..
//

#ifndef CATAN_TRADE_H
#define CATAN_TRADE_H

#include "types/TypeAliases.h"
#include <unordered_set>

class Trade {
private:
    TradeId m_id = -1;
    PlayerId m_requester = types::InvalidPlayer;

    ResourcePack m_give;
    ResourcePack m_receive;

    std::unordered_set<PlayerId> m_respondedPlayers;

    void setId(TradeId tradeId) { m_id = tradeId; }
    friend  class GameSession;
public:
    Trade(PlayerId requester, ResourcePack give, ResourcePack receive)
        : m_requester(requester), m_give(give), m_receive(receive) {}

    TradeId id() const { return m_id; }
    PlayerId requester() const { return m_requester; }

    const ResourcePack& give() const { return m_give; }
    const ResourcePack& receive() const { return m_receive; }

    bool hasAccepted(PlayerId playerId) const {
        return m_respondedPlayers.find(playerId) != m_respondedPlayers.end();
    }

    bool canRespond(PlayerId player) const { // so he cant accept twice
        return player != m_requester && !hasAccepted(player);
    }

    void markResponded(PlayerId player) {
        if (!canRespond(player))
            return;
        m_respondedPlayers.insert(player);
    }
};

#endif //CATAN_TRADE_H