//
// Created by andja on 6.1.26..
//

#include "LongestRoadRule.h"

#include <unordered_map>
#include <unordered_set>
#include <model/GameSession.h>

#include "queue"

std::unordered_map<int,std::vector<int>>  LongestRoadRule::createRoadGraph(GameSession& session, Player& p) {
    std::unordered_map<int,std::unordered_set<int>> roadGraph;
    auto& board = session.board();

    const auto& roads = p.getRoads();
    std::unordered_set<bool> visited;

    for (auto road : roads) {
        EdgeId id = road->getEdgeId();

        for (Edge* adj : board.getIncidentContinuousEdges(id)) {
            roadGraph[id].insert(adj->getEdgeId());
            roadGraph[adj->getEdgeId()].insert(id);
        }
    }

    std::unordered_map<int, std::vector<int>> result;
    result.reserve(roadGraph.size());

    for (auto& [k, s] : roadGraph) {
        result.emplace(k, std::vector<int>(s.begin(), s.end()));
    }
    return result;
}

int LongestRoadRule::dfsLongest(EdgeId current, const std::unordered_map<EdgeId, std::vector<EdgeId>>& graph,std::unordered_set<EdgeId>& used) {
    used.insert(current);

    int best = 0;
    auto it = graph.find(current);
    if (it != graph.end()) {
        for (EdgeId next : it->second) {
            if (used.count(next) == 0) {
                best = std::max(best, dfsLongest(next, graph, used));
            }
        }
    }

    used.erase(current);
    return best + 1;
}


int LongestRoadRule::playerLongestRoad(GameSession& session, Player& p) {
    const auto& graph=createRoadGraph(session,p);

    int longest = 0;
    std::unordered_set<EdgeId> used;

    // dfs from every edge
    for (auto& [edgeId, _] : graph) {
        longest = std::max(longest, dfsLongest(edgeId, graph, used));
    }

    return longest;
}


void LongestRoadRule::evaluate(GameSession& session) {
    const auto& players = session.players();

    PlayerId bestPlayer = session.longestRoadOwner();
    int currentLongest = (bestPlayer != types::InvalidPlayerId)? session.player(bestPlayer).getRoadLength(): -1;

    for (const auto& p : players) {

        int longestRoad = playerLongestRoad(session, *p);
        p->setRoadLength(longestRoad);
        if (longestRoad >= 5 && longestRoad > currentLongest) {
            currentLongest = longestRoad;
            bestPlayer = p->getPlayerId();
        }

    }

    if (bestPlayer == types::InvalidPlayerId)
        return;

    if (session.longestRoadOwner() != bestPlayer) {
        session.setLongestRoadOwner(bestPlayer);
    }
}


std::pair<int, int> LongestRoadRule::bfsFarthest(int start, const std::unordered_map<int,std::vector<int>>& graph, std::unordered_set<EdgeId>& visited) {
    std::queue<std::pair<int,int>> q;
    q.push({start, 0});
    std::unordered_set<EdgeId> localVisited;

    int farNode = start;
    int maxDist = 0;

    while(!q.empty()) {
        auto [node, dist] = q.front();
        q.pop();

        if(localVisited.find(node)!=localVisited.end()) continue;
        localVisited.insert(node);

        if(dist > maxDist) {
            maxDist = dist;
            farNode = node;
        }

        for(int neighbor : graph.at(node)) {
            if(localVisited.count(neighbor)==0)
                q.push({neighbor, dist + 1});
        }
    }

    // visit all nodes in main graph
    for(int node : localVisited) {
        visited.insert(node);
    }

    return {farNode, maxDist};
}

int LongestRoadRule::playerLongestRoadDiameter(GameSession& session, Player& p) {
    const auto& roads=p.getRoads();
    const auto& board=session.board();
    const auto& graph=createRoadGraph(session,p);

    std::unordered_set<EdgeId> visited;
    int maxDiameter=0;

    for(auto [edgeId,_]:graph) {
        if(visited.count(edgeId)==0) {
            // one leaf
            auto [farNode, _] = bfsFarthest(edgeId, graph, visited);
            // to other leaf to find diameter
            auto [otherEnd, diameter] = bfsFarthest(farNode, graph, visited);
            maxDiameter=std::max(maxDiameter,diameter);
        }
    }
    return maxDiameter;
}
