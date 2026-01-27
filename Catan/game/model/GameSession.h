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

/*loopovi igranje igraca/bota i provere vict points road vitezi..*/
// game session is like engine for our game that gets moves from all players from server and executes them to apply changes and keep game running
 // it not only applies our rules but from other to so we keep track of which players turn is it and also which player are we


// TODO add moves activateDevCard move gets dev card type, check if player has it and uses it (decrements counter, applies effect or enters new phase in separate move per dev card)
// TODO add building bridges phase, and reuse initial phase counter (change name to phase counter), it receives build road move and ends after building two roads
// TODO add monopoly and resource phase as well (because we are not accepting those moves unless we paid and activated dev card), add moves for monopoly and resource
// alternative: merge robber move and steal card for knight and dice, merge activate and choose resource for monopoly and resource
// for multi-step moves (monopoly and resources) is valid is false until both get chosen, only then do action buttons becomes clickable and move sent
// keep bridge phase to reuse build road logic?

// TODO add trade request and accept moves



// TODO game session should keep track of pending trade requests, add TradeRequest class with playerId owner, trade pack give and trade pack receive, and it stores players that accepted trade so owner can choose
// TODO when game gets offer move it saves trade request, this gets displayed to other players so they can choose to accept, when others accept its saved in trade struct and when owner does and it matches then trade happens according to stored data
class GameSession {
private:
 std::unique_ptr<Board> m_board;
 std::vector<std::unique_ptr<Player>> m_players;

 // bank
 Bank m_bank;

 // rules
 RulesEngine m_rules;
 int m_winningPints = 10; // TODO this will be read from gameConfig client-hosts sends

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
 bool canStealFromAnyone(PlayerId thiefId) const;

 // phase logic
 TurnPhase m_phase = TurnPhase::InitialPlacement;
 void advanceInitialPlacement();
 void advancePlayer();
 void setPhase(TurnPhase phase) { m_phase = phase; } // add if needed later, for now direct setting

 // trades
 std::unordered_map<TradeId, Trade> m_activeTrades;
 TradeId m_nextTradeId = 1;

 void addTrade(Trade trade);
 void removeTrade(TradeId tradeId);
 Trade* getTrade(TradeId tradeId);

 friend class PlayerTradeRequestMove;
 friend class PlayerTradeResponseMove;
 friend class PlayerTradeAcceptMove;
//TODO DICE MOVE TWO DICES, LEAVE MOVE AND PLAYER HAS LEFT AND SKIP NEXT TU
 // dice
 std::mt19937 m_rng; // TODO dice can be with client-host
 std::uniform_int_distribution<int> m_d6{1, 6};
public:
 GameSession(std::vector<std::string> playerNames, PlayerId localPlayer, uint32_t seed, std::unique_ptr<Board>board, int winPoints, std::string gameName);

 void advancePhaseAfterMove();
 void enterDiscardCardsPhase() { setPhase(TurnPhase::DiscardCards); }
 void enterDevCardPhase(DevCardType type);

 bool applyMove(const Move& move);
 void incrementPhaseMoveCount() { ++m_phaseMoveCount; }
 MoveType lastMoveType() const { return m_lastMoveType; }
 void markDevCardPlayedThisTurn() { m_devCardPlayedThisTurn = true; }
 bool hasPlayedDevCardThisTurn() const { return m_devCardPlayedThisTurn; }
 int rollDice();

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

 GameData& gameData();

 Bank& bank() { return m_bank; }
 const Bank& bank() const { return m_bank; }

 Board& board() { return *m_board; }
 const Board& board() const { return *m_board; }

 const std::vector<std::unique_ptr<Player>>& players() const { return m_players; }
 std::vector<PlayerId> playerIds() const;
 const Player& player(PlayerId id) const { return *m_players.at(id); }
 Player& player(PlayerId id) { return *m_players.at(id); }

 bool playerMustDiscard(PlayerId playerId) const; // TODO this is a rule maybe, but i think no, happens during one move
 bool hasPlayerDiscarded(PlayerId playerId) const;
 void markPlayerDiscarded(PlayerId playerId);
 bool allRequiredPlayersDiscarded() const;

 const Trade* getTrade(TradeId tradeId) const;
 std::vector<const Trade*> activeTrades() const; // TODO not sure if both public and if both needed

 void endGame(); // TODO implement + fill some gameData fields there(awards, everyones points) maybe add endGame phase and all is grey? in function enter that phase
};

#endif //Catan_GAMESESSION_H