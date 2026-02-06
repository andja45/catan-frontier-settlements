//
// Created by andja on 6.1.26..
//

#include "LongestRoadRule.h"
#include <model/GameSession.h>
#include <types/TypeAliases.h>
#include "queue"

void LongestRoadRule::createRoadGraph(GameSession& session, Player p) {
    road_graph.assign(total_possible_roads, std::vector<int>());
    visited.assign(total_possible_roads, false);

    for (auto road : p.getRoads()) {
        if (road == nullptr) continue;

        int road_index = int(road->getEdgeId());
        visited[road_index] = true;

        std::vector<Edge *> neighbours = session.board().getIncidentContinuousEdges(road_index);

        for (auto neighbour : neighbours) {
            int neighbour_index = int(neighbour->getEdgeId());


            if (session.board().getEdgeOwner(neighbour_index) == p.getPlayerId()) {
                NodeId commonNode = getCommonNode(session, road_index, neighbour_index);
                PlayerId nodeOwner = session.board().getNodeOwner(commonNode);

                if (nodeOwner == types::InvalidPlayerId || nodeOwner == p.getPlayerId()) {
                    if (road_index < neighbour_index) {
                        addRoadInGraph(road_index, neighbour_index);
                    }
                }
            }
        }
    }
}

void LongestRoadRule::addRoadInGraph(EdgeId edgeId1, EdgeId edgeId2) {
    road_graph[int(edgeId1)].push_back(int(edgeId2));
    road_graph[int(edgeId2)].push_back(int(edgeId1));
}

NodeId LongestRoadRule::getCommonNode(GameSession& session, int e1, int e2) {
    auto n1 = session.board().getNodesAdjacentToEdge(e1);
    auto n2 = session.board().getNodesAdjacentToEdge(e2);
    for (auto i : n1) {
        if (i == nullptr) continue;
        for (auto j : n2) {
            if (j == nullptr) continue;
            if (i->getNodeId() == j->getNodeId())
                return i->getNodeId();
        }
    }
    return types::InvalidNodeId;
}

int LongestRoadRule::dfs(int current_road, std::vector<bool>& edge_visited) {
    edge_visited[current_road] = true;
    int max_depth = 0;

    for (int neighbour : road_graph[current_road]) {
        if (!edge_visited[neighbour]) {
            max_depth = std::max(max_depth, dfs(neighbour, edge_visited));
        }
    }

    edge_visited[current_road] = false;
    return max_depth + 1;
}


int LongestRoadRule::findLongestRoad() {
    int longest = 0;
    for (int i = 0; i < total_possible_roads; i++) {
        if (visited[i]) {
            std::vector<bool> edge_visited(total_possible_roads, false);
            longest = std::max(longest, dfs(i, edge_visited));
        }
    }
    return longest;
}

void LongestRoadRule::evaluate(GameSession& session) {
    const auto& players = session.players();

    if (players.empty()) {

        return;
    }

    PlayerId currentOwner = session.longestRoadOwner();
    int currentLongest = (currentOwner == types::InvalidPlayerId) ? 4 : 0;
    PlayerId bestPlayer = currentOwner;

    for (const auto& p : players) {
        if (!p) continue;



        createRoadGraph(session, *p);
        int length = findLongestRoad();



        p->setRoadLength(length);

        if (p->getPlayerId() == currentOwner) {
            currentLongest = std::max(4, length);
        }
    }


    for (const auto& p : players) {
        if (p->getRoadLength() > currentLongest) {
            currentLongest = p->getRoadLength();
            bestPlayer = p->getPlayerId();
        }
    }

    if (currentLongest < 5) {
        bestPlayer = types::InvalidPlayerId;
    }

    if (session.longestRoadOwner() != bestPlayer) {
        session.setLongestRoadOwner(bestPlayer);
    }
}

/*
void LongestRoadRule::evaluate(GameSession& session) {

    const auto& players = session.players();
    PlayerId currentOwner = session.longestRoadOwner();

    int currentLongest = (currentOwner == types::InvalidPlayerId) ? 4 : 0;
    PlayerId bestPlayer = currentOwner;

    for (const auto& p : players) {
         createRoadGraph(session, *p);
         int length = findLongestRoad();
         p->setRoadLength(length);

         if (p->getPlayerId() == currentOwner) {
            currentLongest = std::max(4, length);
         }
    }

    for (const auto& p : players) {
        if (p->getRoadLength() > currentLongest) {
             currentLongest = p->getRoadLength();
             bestPlayer = p->getPlayerId();
        }
    }

    if (currentLongest < 5) {
        bestPlayer = types::InvalidPlayerId;
    }

    if (session.longestRoadOwner() != bestPlayer) {
        session.setLongestRoadOwner(bestPlayer);
    }
}

*/