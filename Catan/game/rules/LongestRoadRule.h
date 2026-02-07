//
// Created by andja on 6.1.26..
//

#ifndef CATAN_LONGESTROADRULE_H
#define CATAN_LONGESTROADRULE_H

#include <unordered_map>

#include "Rule.h"
#include "player/Player.h"

// rule in charge of keeping track, giving points and calculating who has longest road
class LongestRoadRule : public Rule {
private:

public:
    int dfsLongest(const Edge *edge, PlayerId playerId, std::unordered_set<EdgeId> &visited, std::unordered_set<NodeId> &visitedNodes);

    int computeLongestRoad(Player &p);

    void evaluate(GameSession& session) override;

};


#endif //CATAN_LONGESTROADRULE_H