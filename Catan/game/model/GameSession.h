//
// Created by andja on 11.12.25..
//

#ifndef Catan_GAMESESSION_H
#define Catan_GAMESESSION_H

#include <random>
#include <unordered_set>

#include "GameData.h"
#include "phase/TurnPhase.h"
#include "rules/RulesEngine.h"
#include "board/Board.h"
#include "move/Move.h"
#include "player/Player.h"
#include "types/TypeAliases.h"
#include "move/MoveType.h"
#include "move/devcard/PlayDevCardMove.h"
#include "player/Bank.h"
#include "Trade.h"
class Move;
class StealCardMove;
class PlayerTradeRequestMove;

// game session is like engine for our game that gets moves from all players from server and executes them to apply changes and keep game running
// it not only applies our rules but from other to so we keep track of which players turn is it and also which player are we


class GameSession {
private:

     int m_numOfActivePlayers;
     bool m_isOver=false;
     std::unique_ptr<Board> m_board;
     std::vector<std::unique_ptr<Player>> m_players;

     // bank
     Bank m_bank;

     // rules
     RulesEngine m_rules;
     int m_winningPints=10;

     // game data
     GameData m_gameData;

     // global info
     PlayerId m_longestRoadOwner = -1;
     PlayerId m_largestArmyOwner = -1;
     PlayerId m_winner = -1;

     int m_turnIndex = 0;
     // game should be playable even without multiplayer, but in gui we will set buttons unclickable if currplayer != localplayer cus only he can make moves on his gui, other clients send him their moves and game session executes them
     PlayerId m_currentPlayerId = -1;
     PlayerId m_localPlayerId   = -1; // who am i?

     // move logic
     int m_phaseMoveCount = 0;
     MoveType m_lastMoveType = MoveType::InvalidMoveType;
     bool m_devCardPlayedThisTurn = false;
     std::unordered_set<PlayerId> m_discardedPlayers;
     bool canStealFrom(PlayerId thiefId, PlayerId victimId) const;
     bool canStealFromAnyone(PlayerId thiefId) const;

     // phase logic
     TurnPhase m_phase = TurnPhase::InitialPlacement;

     void advancePlayerInitial();

     void advanceInitialPlacement();
     void advancePlayer();

     void setPhase(TurnPhase phase) { m_phase = phase; } // add if needed later, for now direct setting

 // trades
 std::unordered_map<TradeId, Trade> m_activeTrades;
 TradeId m_nextTradeId = 0;

#ifdef TESTING
public:
    void forcePhase(TurnPhase phase) { m_phase = phase; }
    void forceCurrentPlayer(PlayerId p) { m_currentPlayerId = p; }
private:
#endif

     void addTrade(Trade trade);
     void removeTrade(TradeId tradeId);
     Trade* getTrade(TradeId tradeId);

     friend class PlayerTradeRequestMove;
     friend class PlayerTradeResponseMove;
     friend class PlayerTradeAcceptMove;
     friend class SkipPlayerRule;

     // dice
     std::mt19937 m_rng;
     std::uniform_int_distribution<int> m_d6{1, 6};
     std::pair<int,int> m_lastDiceRoll = {1,1};

public:
    GameSession(std::vector<std::string> playerNames, PlayerId localPlayer, uint32_t seed, std::unique_ptr<Board>board, int winPoints, std::string gameName);

    GameData& gameData(){return m_gameData;}

 void advancePhaseAfterMove();
 void enterDiscardCardsPhase() { setPhase(TurnPhase::DiscardCards); }
 void enterMainPhase() { setPhase(TurnPhase::Main); }
 void enterInitialPlacementPhase() { setPhase(TurnPhase::InitialPlacement); }
 void enterRoadBuildingPhase() { setPhase(TurnPhase::RoadBuilding); }
 void enterDevCardPhase(DevCardType type);

    bool applyMove(const Move& move);
    void incrementPhaseMoveCount() { ++m_phaseMoveCount; }
    MoveType lastMoveType() const { return m_lastMoveType; }
    void markDevCardPlayedThisTurn() { m_devCardPlayedThisTurn = true; }
    bool hasPlayedDevCardThisTurn() const { return m_devCardPlayedThisTurn; }

    std::pair<int, int> rollDice();
    std::mt19937 copyRng()const {return m_rng;};
    std::mt19937& getRng() {return m_rng;};
    int getNextTradeId();
    const std::pair<int,int>* getDice() const {return &m_lastDiceRoll;}

    PlayerId localPlayer()   const { return m_localPlayerId; }
    PlayerId currentPlayer() const { return m_currentPlayerId; }

    PlayerId largestArmyOwner() const { return m_largestArmyOwner; }
    PlayerId longestRoadOwner() const { return m_longestRoadOwner; }
    PlayerId winner() const { return m_winner; }

    int winningPoints() const { return m_winningPints; }
    int numPlayers() const { return static_cast<int>(m_players.size()); }
    TurnPhase phase() const { return m_phase; }

    void setLongestRoadOwner(PlayerId playerId);
    void setLargestArmyOwner(PlayerId playerId);
    void setWinner(PlayerId playerId) { m_winner = playerId; }

    Bank& bank() { return m_bank; }
    const Bank& bank() const { return m_bank; }

    Board& board() { return *m_board; }
    const Board& board() const { return *m_board; }

    const std::vector<std::unique_ptr<Player>>& players() const { return m_players; }
    std::vector<PlayerId> playerIds() const;
    const Player& player(PlayerId id) const { return *m_players.at(id); }
    Player& player(PlayerId id) { return *m_players.at(id); }

    bool playerMustDiscard(PlayerId playerId) const;
    bool hasPlayerDiscarded(PlayerId playerId) const;
    void markPlayerDiscarded(PlayerId playerId);
    bool allRequiredPlayersDiscarded() const;

    const Trade* getTrade(TradeId tradeId) const;
    std::vector<const Trade*> activeTrades() const;

    void endGame();

    int activePlayersLeft() const { return m_numOfActivePlayers; }

    void leavePlayer(PlayerId player_id);

    void dealInitial();

    std::unordered_map<TradeId, Trade> * getTrades(){ return &m_activeTrades; }

     std::vector<Player *> getPlayers() {
         std::vector<Player *> result;
         for (auto& p : m_players) {
             result.push_back(p.get());
         }
         return result;
     }

     Bank * getBank(){return &m_bank;}
};

#endif //Catan_GAMESESSION_H