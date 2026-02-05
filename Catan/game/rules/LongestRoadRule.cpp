//
// Created by andja on 6.1.26..
//

#include "LongestRoadRule.h"
#include <model/GameSession.h>
#include <types/TypeAliases.h>
#include "queue"

void LongestRoadRule::createRoadGraph(GameSession& session, Player p) {
    visited.resize(total_possible_roads, false);
    road_graph.reserve(total_possible_roads);

    for (auto road : p.getRoads())
    {
        int road_index = int(road->getEdgeId());

        if (visited[road_index]) {
            continue;
        }

        visited[road_index] = true;

        std::vector<Edge *> neighbours = session.board().getIncidentContinuousEdges(road_index);

        for (auto neighbour : neighbours)
        {
            addRoadInGraph(road_index, neighbour->getEdgeId());
            visited[neighbour->getEdgeId()] = true;
        }

    }
}

void LongestRoadRule::addRoadInGraph(EdgeId edgeId1, EdgeId edgeId2) {
    road_graph[int(edgeId1)].push_back(int(edgeId2));
    road_graph[int(edgeId2)].push_back(int(edgeId1));
}

int LongestRoadRule::findLongestRoad() {
    std::vector<std::pair<int, int>> distance;
    distance.resize(total_possible_roads, {-1, -1});
    std::queue<int> order;

    for (int i = 0; i < total_possible_roads; i++) {
        if (visited[i] && distance[i].first == -1) {

            order.push(i);
            distance[i].first = 0;
            distance[i].second = i;

            while (!order.empty()) {
                int temp = order.front();

                for (int j = 0; j < road_graph[temp].size(); j++) {
                    if (distance[road_graph[temp][j]].first == -1) {
                        order.push(road_graph[temp][j]);
                        distance[road_graph[temp][j]].first = distance[temp].first + 1;
                        distance[road_graph[temp][j]].second = i;
                    }
                }

                order.pop();
            }
        }
    }

    std::map<int, std::pair<int, int>> maximums;

    for (int i = 0; i < total_possible_roads; i++) {
        if (!maximums.count(distance[i].second) || distance[i].first > maximums[distance[i].second].first) {
            maximums[distance[i].second].first = distance[i].first;
            maximums[distance[i].second].second = i;
        }
    }

    for (int i = 0; i < total_possible_roads; i++) {
        if (maximums[distance[i].second].second == i) {

            order.push(i);
            distance[i].first = 0;

            while (!order.empty()) {
                int temp = order.front();

                for (int j = 0; j < road_graph[temp].size(); j++) {
                    if (distance[road_graph[temp][j]].first == -1) {
                        order.push(road_graph[temp][j]);
                        distance[road_graph[temp][j]].first = distance[temp].first + 1;
                    }
                }

                order.pop();
            }
        }
    }

    int longestRoad = 0;

    for (int i = 0; i < total_possible_roads; i++) {
        if (distance[i].first > longestRoad) {
            longestRoad = distance[i].first;
        }
    }

    return longestRoad;
}

void LongestRoadRule::evaluate(GameSession& session) {
    const auto& players = session.players();

    PlayerId bestPlayer = session.longestRoadOwner();

    int currentLongest = -1;

    for (const auto& p : players) {
        createRoadGraph(session, *p);

        int longestRoad = findLongestRoad();

        if (longestRoad > currentLongest) {
            currentLongest = longestRoad;
            bestPlayer = p->getKnightsUsed();
        }
        p->setRoadLength(longestRoad);
    }

    if (bestPlayer == types::InvalidPlayerId)
        return;

    if (session.longestRoadOwner() != bestPlayer) {
        session.setLongestRoadOwner(bestPlayer);
    }
}
