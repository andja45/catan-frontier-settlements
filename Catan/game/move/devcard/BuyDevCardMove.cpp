//
// Created by andja on 16.1.26..
//

#include "BuyDevCardMove.h"
#include "model/GameSession.h"

void BuyDevCardMove::setDevCard(GameSession &session) {
    m_card = randomDevCard(session.getBank()->getDevCards(),session.getRng());
}

bool BuyDevCardMove::isValid(const GameSession& session) const {
    const Player& player = session.player(m_playerId);
    const Bank& bank = session.bank();

    if (session.currentPlayer() != m_playerId)
        return false;

    if (session.phase() != TurnPhase::Main)
        return false;

    if (!bank.hasDevCards()) // checks does bank have any devcards left
        return false;

    if (!player.hasResources(Costs::DevCard))
        return false;

    if (session.localPlayer()!=m_playerId) // we draw again to confirm remote roll
    {
        auto mt=session.copyRng();
        auto dev= randomDevCard(session.bank().getDevCards(),mt);
        return dev==m_card;
    }

    return true;
}

void BuyDevCardMove::apply(GameSession& session) const {
    Player& player = session.player(m_playerId);
    Bank& bank = session.bank();

    player.removeResources(Costs::DevCard);

    if (session.localPlayer()!=m_playerId){ // we draw again to sync with remote
        auto _ = randomDevCard(session.getBank()->getDevCards(),session.getRng());
    }

    session.getBank()->removeDevCard(m_card);
    player.addDevCard(m_card);
}

DevCardType BuyDevCardMove::randomDevCard(DevPack pack, std::mt19937& mt) {
    std::uniform_int_distribution<std::size_t> dist(0, pack.size() - 1);
    auto it = std::next(pack.begin(), dist(mt));

    while (it->second == 0){
        it=std::next(pack.begin(), dist(mt));
    }
    return it->first;
}