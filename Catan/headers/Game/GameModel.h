//
// Created by andja on 11.12.25..
//

#ifndef Catan_GAMEMODEL_H
#define Catan_GAMEMODEL_H
#include "GameSession.h"
#include "../Board/Board.h"
#include  "../Player/Player.h"

/* provera legalnosti poteza, npr jel ima rss da kupi nesto i trenutno stanje igre */

class Move;

class GameModel {
private:
    Board m_board;
    std::vector<Player> m_players;
    GameSession m_session; // #TODO sesija dobija move od controlera i ona dalje sa move, move prima samo sesiju

    //std::vector<GameObserver*> m_observers;
    void notifyMoveApplied(const Move&);
public:
    explicit GameModel(int numPlayers);

    // === JEDINI ULAZ U MODEL ===
    bool applyMove(const Move& move);

    // === SESSION ===
    const GameSession& session() const { return m_session; }

    // === OBSERVER ===
    //void addObserver(GameObserver*);
    //void removeObserver(GameObserver*);

    // === Move-ovi koriste ===
    bool hasResources(int playerId, const ResourcePack&) const;
    void consumeResources(int playerId, const ResourcePack&);

    void placeRoad(int playerId, int edgeId);
    void placeSettlement(int playerId, int nodeId);
    void upgradeCity(int playerId, int nodeId);

    int tradeRatioFor(int playerId, ResourceType give) const;

    int rollDice();
    void distributeResources(int dice);

    // === ACCESS === #TODO sakrij ovo da ne bude public svako pristup
    Board& board() { return m_board; }
    const Board& board() const { return m_board; }
    Player& player(int id) { return m_players.at(id); }
    const Player& player(int id) const { return m_players.at(id); }

    bool canPlayerBuildSettlement(int m_player_id, int m_node_id) const;
};


#endif //Catan_GAMEMODEL_H
