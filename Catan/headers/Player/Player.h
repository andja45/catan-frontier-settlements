//
// Created by andja on 9.12.25..
//

#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <string>
#include <vector>

#include "../Types/ResourceType.h"

class Edge;
class Node;

class Player {
private:
    std::string m_name;
    int m_victoryPoints;
    std::vector<Edge*> m_roads;
    std::vector<Node*> m_houses;
    std::map<ResourceType, int> m_resources;
public:
    Player(std::string name) : m_name(name) {}
    ~Player() = default;

    std::string getName() { return m_name; }
    int getVictoryPoints() { return m_victoryPoints; }

    void setVictoryPoints(int victoryPoints) { m_victoryPoints = victoryPoints; }
    void setHouses(std::vector<Node*> houses) { m_houses = houses; }
};


#endif //PLAYER_H
