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

#include "move/trade/BankTradeMove.h"

enum class DevCardType;
class Player : public ResourceHolder {
private:
    PlayerId m_playerId=-1;

    std::vector<Edge*> m_roads;
    std::vector<Node*> m_buildings; // TODO remove in the future? or keep for trades and longest road?

    std::vector<std::vector<int>> road_graph;

    //std::map<ResourceType,bool> m_has2for1Trade;

    int m_knightsUsed=0;
    int m_victoryPointsUsed=0;

    int m_totalPoints=0; //kept in check by game as part of game logic and rules, do not calculate it here!

    int m_numOfRoadsLeft=15;
    int m_numOfCitiesLeft=5;
    int m_numOfSettlementsLeft=5;

    bool m_isActive=true;

public:
    explicit Player(PlayerId id, std::string name) : ResourceHolder(name), m_playerId(id) {}

    PlayerId getPlayerId() const { return m_playerId; }

    std::vector<Edge*> getRoads() const { return m_roads; }
    std::vector<Node*> getBuildings() const { return m_buildings; }
    Node* getLastBuildingBuilt() const {return m_buildings.back();}

    void addPoints(int points) {m_totalPoints+=points;}
    void removePoints(int points) {m_totalPoints-=points;}

    void addRoad(Edge* edge);
    void addSettlement(Node* node);
    void addCity();

    void addRoadInGraph(EdgeId edgeId1, EdgeId edgeId2);

    bool has3for1Trade() const;
    bool has2for1Trade(ResourceType resourceType) const {return hasTrade(resourceType);}

    bool hasCityLeft() const {return m_numOfCitiesLeft>0;}
    bool hasSettlementLeft() const {return m_numOfSettlementsLeft>0;}
    bool hasRoadLeft() const {return m_numOfRoadsLeft>0;}
    bool hasBuildings() const {return m_buildings.size()>0;}

    int getTotalPoints() const {return m_totalPoints;}

    int getVictoryPointsUsed() const {return m_victoryPointsUsed;}
    int getKnightsUsed() const {return m_knightsUsed;}
    int getNumOfRoadsLeft() const {return m_numOfRoadsLeft;}
    int getNumOfCitiesLeft() const {return m_numOfCitiesLeft;}
    int getNumOfSettlementsLeft() const {return m_numOfSettlementsLeft;}

    bool hasTrade(ResourceType resourceType) const;

    ResourcePack takeRandomResources(int amount); // for robber discard and steal card
    ResourceType takeRandomResource();

    int minBankTradeRatio(ResourceType resource) const;

    bool isActive() const {return m_isActive;}
    void setLeft() {m_isActive=false;}
};


#endif //Catan_PLAYER_H
