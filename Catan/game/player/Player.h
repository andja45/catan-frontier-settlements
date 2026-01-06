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
    PlayerId m_playerId=-1;

    std::vector<Edge*> m_roads; //
    std::vector<Node*> m_houses; // TODO remove in the future? or keep for trades and longest road?

    //std::map<ResourceType,bool> m_has2for1Trade;

    int m_knightsUsed=0;
    int m_victoryPointsUsed=0;

    int m_totalPoints=0; //TODO calculated, kept in check by game? kept in check by setters?

    int m_numOfRoadsLeft=15;
    int m_numOfCitiesLeft=5;
    int m_numOfSettlementsLeft=5;

public:
    explicit Player(PlayerId id, std::string name) : ResourceHolder(name), m_playerId(id) {}

    PlayerId getPlayerId() const { return m_playerId; }

    std::vector<Edge*> getRoads() const { return m_roads; }
    std::vector<Node*> getHouses() const { return m_houses; }

    void addPoints(int points) {m_totalPoints+=points;}
    void removePoints(int points) {m_totalPoints-=points;}

    void addRoad(Edge* edge);
    void addSettlement(Node* node);
    void addCity(Node* node);

    bool has3for1Trade() const;
    bool has2for1Trade(ResourceType resourceType) const {return hasTrade(resourceType);}

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
    int longestRoadLength() const;

    ResourcePack takeRandomResources(int amount);
    ResourceType takeRandomResource();
};


#endif //Catan_PLAYER_H
