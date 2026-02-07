//
// Created by andja on 10.12.25.
//

#include "RollDiceMove.h"
#include "model/GameSession.h"

void RollDiceMove::setDiceRoll(GameSession &session) {
    auto [d1,d2]= session.rollDice();
    m_diceRoll1=d1;
    m_diceRoll2=d2;
}

bool RollDiceMove::isValid(const GameSession& session) const {
    if (session.currentPlayer() != m_playerId)
        return false;
    if (!(0<m_diceRoll1&&m_diceRoll1<=6) || !(0<m_diceRoll2&&m_diceRoll2<=6))
        return false;
    if (session.localPlayer()!=m_playerId) // dice rolled remote, we roll again to confirm, everybody has same seed
    {
        auto rng=session.copyRng();
        std::uniform_int_distribution<int> m_d6{1, 6};

        int dice1 = m_d6(rng);
        int dice2 = m_d6(rng);

        if (dice1 + dice2 != getDiceRoll().first+getDiceRoll().second) {
            return false;
        }
    }
    if (session.phase() != TurnPhase::RollDice)
        return false;

    return true;
}

void RollDiceMove::apply(GameSession& session) const {
    Board& board = session.board();
    if (session.localPlayer()!=m_playerId) // we dummy roll to keep seed in sync
    {
        session.rollDice(); //we keep sync
    }

    auto diceRolls=getDiceRoll();
    const int diceRoll = diceRolls.first + diceRolls.second;
    if (diceRoll == 7) {
        session.enterDiscardCardsPhase(); // we force a phase, this isnt a sequence move
        return;
    }

    for (const Tile* tile : board.getTilesWithNumber(diceRoll)) {
        if (tile->isRobberOnTile())
            continue;

        ResourceType resource = tile->getResourceType();

        for (const Node* node : tile->getAdjacentNodes()) {
            if (!node) continue;

            const PlayerId owner = node->getOwner();
            if (owner == -1)
                continue;

            int amount = 0;
            if (node->isCity()) amount = 2;
            else if (node->isSettlement()) amount = 1;
            else std::cerr << "Has owner but isnt City or Settlement";
            if (amount>(session.bank().getResources())[resource]) // if bank doesnt have enough, we give what we can
            {
                amount=session.bank().getResources()[resource];
            }
            session.player(owner).addResource(resource, amount);
            session.gameData().addResourceRoll(resource, amount);
            session.bank().removeResource(resource, amount);
        }
    }

    session.gameData().addDiceRoll(diceRoll); // only log when roll is accepted!
}