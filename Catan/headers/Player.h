//
// Created by andja on 9.12.25..
//

#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <string>
#include <vector>

#include "ResourceType.h"

class Edge;
class Node;

class Player {
private:
    std::string _name;
    int _victoryPoints;
    std::vector<Edge*> _roads;
    std::vector<Node*> _houses;
    std::map<ResourceType, int> _resources;
public:
    Player(std::string name) : _name(name) {}
    ~Player() = default;

    std::string getName() { return _name; }
    int getVictoryPoints() { return _victoryPoints; }

    void setVictoryPoints(int victoryPoints) { _victoryPoints = victoryPoints; }
    void setHouses(std::vector<Node*> houses) { _houses = houses; }
};


#endif //PLAYER_H