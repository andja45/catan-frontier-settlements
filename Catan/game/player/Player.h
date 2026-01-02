//
// Created by andja on 9.12.25..
//

#ifndef Catan_PLAYER_H
#define Catan_PLAYER_H

#include <map>
#include <string>
#include <vector>

#include "../board/Edge.h"
#include "../board/Node.h"
#include <player/ResourceHolder.h>

enum class DevType;
class Player : public ResourceHolder {
private:
    static int m_numPlayers;
    int m_playerId      =-1;

    std::vector<Edge*> m_roads;
    std::vector<Node *> m_houses;

    std::map<ResourceType,bool> m_has2for1Trade;

    int m_knightsUsed=0;
    int m_victoryPointsUsed=0;

    int m_totalPoints=0;

    int m_numOfRoadsLeft=15;
    int m_numOfCitiesLeft=5;
    int m_numOfSettlementsLeft=5;

    bool m_has3for1Trade = false;

public:
    Player(const std::string &name) : ResourceHolder() {}

    int getPlayerId() const { return m_playerId; }

    std::vector<Edge*> getRoads() const { return m_roads; }
    void addVictoryPoints(int points) {m_victoryPointsUsed+=points; m_totalPoints+=points;}
    void addVictoryPoint() {addVictoryPoints(1);}
    void addRoad(Edge* newroad);
    void addSettlement(Node* node);
    void addCity(Node* node);

    bool has3for1Trade() const {return m_has3for1Trade;}
    void give3for1Trade()  {m_has3for1Trade=true;}

    bool has2for1Trade(ResourceType resourceType)  {return m_has2for1Trade[resourceType];}
    void give2for1Trade(ResourceType resourceType)  {m_has2for1Trade[resourceType]=true;}

    bool hasCityLeft() const {return m_numOfCitiesLeft>0;}
    bool hasSettlementLeft() const {return m_numOfSettlementsLeft>0;}
    bool hasRoadLeft() const {return m_numOfRoadsLeft>0;}

    int getTotalPoints() const {return m_totalPoints;}
    int getVictoryPointsUsed() const {return m_victoryPointsUsed;}
    int getKnightsUsed() const {return m_knightsUsed;}
    int getNumOfRoadsLeft() const {return m_numOfRoadsLeft;}
    int getNumOfCitiesLeft() const {return m_numOfCitiesLeft;}
    int getNumOfSettlementsLeft() const {return m_numOfSettlementsLeft;}

    bool hasTrade(ResourceType resourceType) const;
    bool hasLongestRoad() const;

    ResourcePack takeRandomResources(int amount);
    ResourceType takeRandomResource();

};


#endif //Catan_PLAYER_H
