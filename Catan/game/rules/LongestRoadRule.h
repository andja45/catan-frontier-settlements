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

    std::unordered_map<int, std::vector<int>> createRoadGraph(GameSession &session, Player &p);

    int dfsLongest(EdgeId current, const std::unordered_map<EdgeId, std::vector<EdgeId>> &graph,
                   std::unordered_set<EdgeId> &used);

    int playerLongestRoad(GameSession &session, Player &p);


public:
    void evaluate(GameSession& session) override;

    std::pair<int, int> bfsFarthest(int start, const std::unordered_map<int, std::vector<int>> &graph,
                                    std::unordered_set<EdgeId> &visited);

    int playerLongestRoadDiameter(GameSession &session, Player &p);
};


#endif //CATAN_LONGESTROADRULE_H