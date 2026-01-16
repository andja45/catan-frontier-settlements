//
// Created by andja on 6.1.26..
//

#include "LargestArmyRule.h"
#include "gamemodel/GameSession.h"

void LargestArmyRule::evaluate(GameSession& session){
    const auto& players = session.players();

    PlayerId bestPlayer = session.largestArmyOwner();
    int bestKnights = 0;
    if (bestPlayer != types::InvalidPlayer)
        bestKnights = session.player(bestPlayer).getKnightsUsed();

    for (const auto& p : players) {
        const int knights = p->getKnightsUsed();
        if (knights >= 3 &&
            knights > bestKnights) { // we try to beat the old best player, or find one if they dont exist

            bestKnights = knights;
            bestPlayer = p->getPlayerId();
            }
    }

    if (bestPlayer == types::InvalidPlayer)
        return;

    if (session.largestArmyOwner() != bestPlayer) {
        session.setLargestArmyOwner(bestPlayer);
    }
}