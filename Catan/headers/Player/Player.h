//
// Created by andja on 9.12.25..
//

#ifndef Catan_PLAYER_H
#define Catan_PLAYER_H

#include <map>
#include <string>
#include <vector>

#include "../Board/Edge.h"
#include "../Board/Node.h"

enum class DevType;

class Player {
private:
    static int _numPlayers;
    int _playerId =-1;

    std::vector<Edge*> _roads;
    std::vector<Node *> _houses;

    std::map<ResourceType,bool> _hasTrade;

    std::map<ResourceType, int> _resources;
    std::map<DevType, int> _devCards;

    int _knightsUsed=0;
    int _victoryPointsUsed=0;
    bool _hasLongestRoad=false;

    int _numOfRoadsLeft=15;
    int _numOfCitiesLeft=5;
    int _numOfSettlementsLeft=5;
public:
    Player(int id) : _playerId(id) {}

    int getPlayerId() const { return _playerId; }
    
    std::vector<Edge*> getRoads() const { return _roads; }
    void takeResource(ResourceType resourceType, int amount);
    void addResource(ResourceType resourceType, int amount);
    void addDevCard(DevType devType);
    void useDevCard(DevType devType);
    void addVictoryPoints(int points);
    void addRoad(Edge* road);
    void addHouse(Node* house);

    bool has3for1Trade() const;
    bool hasTrade(ResourceType resourceType) const;
    bool hasLongestRoad() const;
    bool hasCityLeft() const;
    bool hasSettlementLeft() const;
    bool hasRoadLeft() const;
    bool hasResource(ResourceType resourceType, int amount) const; // za gamemodel treba

    int getTotalPoints() const;
    int getVictoryPointsUsed() const;
    int getKnightsUsed() const;
    int getNumOfDevCards() const;
    int getNumOfResourceCards() const;
    int getNumOfRoadsLeft() const;
    int getNumOfCitiesLeft() const;
    int getNumOfSettlementsLeft() const;

    void setLongestRoad(bool longestRoad) {_hasLongestRoad=longestRoad;}
    ResourceType takeRandomResource();
};


#endif //Catan_PLAYER_H
