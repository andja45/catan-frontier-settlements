//
// Created by andja on 10.12.25.
//

#include "RollDiceMove.h"

bool RollDiceMove::isValid(const GameSession& session) const {
    if (session.currentPlayer() != m_playerId) // TODO game should be playable even without multiplayer, but in gui we will set buttons unclickable if currplayer != localplayer cus only he can make moves on his gui, other clients send him their moves
        return false;

    if (session.phase() != TurnPhase::RollDice)
        return false;

    return true;
}

void RollDiceMove::apply(GameSession& session) const {
    Board& board = session.board();

    const int diceRoll = session.rollDice();
    if (diceRoll == 7) {
        session.enterRobberPhase(); // TODO resi problem odbacivanja karata
        return;
    }

    for (const Tile* tile : board.getTilesWithNumber(diceRoll)) {
        if (tile->isRobberOnTile())
            continue;

        ResourceType resource = tile->getType();

        for (const Node* node : tile->getAdjacentNodes()) {
            if (!node) continue;

            const PlayerId owner = node->getOwner();
            if (owner == -1)
                continue;

            const int amount = node->isCity() ? 2 : 1;
            session.player(owner).addResource(resource, amount);
        }
    }
}