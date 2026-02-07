//
// Created by andja on 6.1.26..
//

#include "LongestRoadRule.h"

#include <unordered_map>
#include <unordered_set>
#include <model/GameSession.h>

#include "queue"

int LongestRoadRule::dfsLongest(const Edge* edge,PlayerId playerId,std::unordered_set<EdgeId>& visited, std::unordered_set<NodeId>& visitedNodes)
{
    visited.insert(edge->getEdgeId());
    int best = 0;

    for (const Node* node : edge->getNodes())
    {
        if (visitedNodes.count(node->getNodeId())!=0)
            continue;
        visitedNodes.insert(node->getNodeId());
        if (!node->isEmpty() && node->getOwner() != playerId)
            continue;

        for (const Edge* next : node->getIncidentEdges())
        {
            if (!next) continue;
            if (next->getOwner() != playerId) continue;
            if (visited.count(next->getEdgeId())) continue;

            best = std::max(best, dfsLongest(next, playerId, visited, visitedNodes));
        }
        visitedNodes.erase(node->getNodeId());
    }

    visited.erase(edge->getEdgeId());
    return best + 1;
}

int LongestRoadRule::computeLongestRoad(Player& p)
{
    int longest = 0;

    for (const Edge* edge : p.getRoads())
    {
        std::unordered_set<EdgeId> visited;
        std::unordered_set<NodeId> visitedNodes;
        longest = std::max(longest, dfsLongest(edge, p.getPlayerId(), visited, visitedNodes));
    }

    return longest;
}


void LongestRoadRule::evaluate(GameSession &session) {
    const auto &players = session.players();

    PlayerId bestPlayer = session.longestRoadOwner();
    int currentLongest  = (bestPlayer != types::InvalidPlayerId) ? session.player(bestPlayer).getRoadLength() : -1;

    for (const auto &p: players) {
        int longestRoad = computeLongestRoad(*p);
        p->setRoadLength(longestRoad);
        if (longestRoad >= 5 && longestRoad > currentLongest) {
            currentLongest = longestRoad;
            bestPlayer     = p->getPlayerId();
        }
    }

    if (bestPlayer == types::InvalidPlayerId)
        return;

    if (session.longestRoadOwner() != bestPlayer) {
        session.setLongestRoadOwner(bestPlayer);
    }
}
