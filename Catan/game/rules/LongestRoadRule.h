//
// Created by andja on 6.1.26..
//

#ifndef CATAN_LONGESTROADRULE_H
#define CATAN_LONGESTROADRULE_H


#include "Rule.h"
#include "player/Player.h"

// rule in charge of keeping track, giving points and calculating who has longest road
class LongestRoadRule : public Rule {
private:
    std::vector<bool> visited;
    int total_possible_roads = 110;
    std::vector<std::vector<int>> road_graph;
    void addRoadInGraph(EdgeId edgeId1, EdgeId edgeId2);
    void createRoadGraph(GameSession& session, Player p);
    int findLongestRoad();
public:
    void evaluate(GameSession& session) override;
};


#endif //CATAN_LONGESTROADRULE_H