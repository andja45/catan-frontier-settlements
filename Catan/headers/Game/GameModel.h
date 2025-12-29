//
// Created by andja on 11.12.25..
//

#ifndef Catan_GAMEMODEL_H
#define Catan_GAMEMODEL_H
#include "../Board/Board.h"
#include  "../Player/Player.h"

/* provera legalnosti poteza, npr jel ima rss da kupi nesto i trenutno stanje igre */

class Move;

class GameModel {
private:
    Board m_board;
    std::vector<Player> m_players;

    //std::vector<GameObserver*> m_observers; // controller ce biti u toj listi da registruje izmene internih stanja
    void notifyModelChanged();
public:
    explicit GameModel(int numPlayers);

    // === OBSERVER ===
    //void addObserver(GameObserver*);
    //void removeObserver(GameObserver*);

    bool hasResource(int playerId, ResourceType type, int amount) const;
    bool hasResources(int playerId, const ResourcePack&) const;

    void consumeResource(int playerId, ResourceType type, int amount);
    void consumeResources(int playerId, const ResourcePack&);
    void transferResource(int playerId, ResourceType type, int amount);
    void transferResources(int from, int to, const ResourcePack& pack);
    void stealRandomResource(int thiefId, int victimId);

    bool canPlaceRoad(int player_id, int edge_id) const;
    bool canPlaceSettlement(int player_id, int node_id, bool isInitialPlacement = false) const;
    bool canPlaceCity(int player_id, int node_id) const;
    bool canPlaceRobber(int tileId) const;
    bool canStealFrom(int thiefId, int victimId) const;

    void placeRoad(int playerId, int edgeId);
    void placeSettlement(int playerId, int nodeId);
    void placeCity(int playerId, int nodeId);
    void placeRobber(int tileId);

    int tradeRatioFor(int playerId, ResourceType give) const;
    int rollDice();
    void distributeResources(int dice);
};


#endif //Catan_GAMEMODEL_H
