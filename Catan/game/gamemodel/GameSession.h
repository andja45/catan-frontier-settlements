//
// Created by andja on 11.12.25..
//

#ifndef Catan_GAMESESSION_H
#define Catan_GAMESESSION_H

#include <random>

#include "rules/RulesEngine.h"
#include "board/Board.h"
#include "player/Player.h"
#include "types/TypeAliases.h"
class Move;

/*loopovi igranje igraca/bota i provere vict points road vitezi..*/

enum class TurnPhase {
 RollDice,
 Main, // regularan potez, build/trade/kupuje devcards, nakon rolldice
 Robber, // mozda nije faza?
 DiscardCards, //
 InitialPlacement,
 End // mozda nije faza?
}; // drzi to stanje sve dok ne dobije signal da je uradjeno nesto sto ga menja
   // tj to je stanje koje ceka, ne ono koje sledi

enum class InitialPlacementStep {
 PlaceSettlement,
 PlaceRoad
};

class GameSession {
private:
 std::unique_ptr<Board> m_board;
 std::vector<std::unique_ptr<Player>> m_players;

 // rules
 RulesEngine m_rules;
 int m_winningPints = 10; // TODO this will be read from gameConfig client-hosts sends

 // global info
 PlayerId m_longestRoadOwner = -1;
 PlayerId m_largestArmyOwner = -1;
 PlayerId m_winner = -1;

 int m_turnIndex = 0;
 PlayerId m_currentPlayerId = -1;
 PlayerId m_localPlayerId   = -1; // who am i?

 // phase logic
 TurnPhase m_phase = TurnPhase::InitialPlacement;
 int m_initialPlacementsCount = 0;
 bool m_initialPlacementsReverse = false;
 void advanceInitialPlacement();
 void advancePlayer();
 void setPhase(TurnPhase phase) { m_phase = phase; } // add if needed later, for now direct setting

 // dice
 std::mt19937 m_rng; // TODO dice can be with client-host
 std::uniform_int_distribution<int> m_d6{1, 6};
public:
 GameSession(int numPlayers, std::vector<std::string> playerNames, PlayerId localPlayer, uint32_t seed);

 InitialPlacementStep initialPlacementStep() const;
 void advancePhaseAfterMove(const Move &move);
 void enterRobberPhase() { setPhase(TurnPhase::Robber); }

 bool applyMove(const Move& move);
 int rollDice();
 void endGame(); // TODO implement

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

 Board& board() { return *m_board; }
 const Board& board() const { return *m_board; }

 const std::vector<std::unique_ptr<Player>>& players() const { return m_players; }
 const Player& player(PlayerId id) const { return *m_players.at(id); }
 Player& player(PlayerId id) { return *m_players.at(id); }
};

#endif //Catan_GAMESESSION_H