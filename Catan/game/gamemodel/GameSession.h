//
// Created by andja on 11.12.25..
//

#ifndef Catan_GAMESESSION_H
#define Catan_GAMESESSION_H

#include <random>

#include "board/Board.h"
#include "move/Move.h"
#include "player/Player.h"
#include "types/TypeAliases.h"

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

 int m_turnIndex = 0;
 PlayerId m_currentPlayerId = m_players[m_turnIndex]->getPlayerId();
 PlayerId m_localPlayerId   = -1; // ko sam ja?
 TurnPhase m_phase = TurnPhase::InitialPlacement;

 // phase logic
 int m_initialPlacementsCount = 0;
 bool m_initialPlacementsReverse = false;
 void advanceInitialPlacement();
 void advancePlayer();
 void setPhase(TurnPhase phase) { m_phase = phase; } // add if needed later, for now direct setting

 // dice
 std::mt19937 m_rng; // TODO dice can be with client-host
 std::uniform_int_distribution<int> m_d6{1, 6};
public:
 GameSession(int numPlayers, PlayerId localPlayer, uint32_t seed);

 InitialPlacementStep initialPlacementStep() const;
 void advancePhaseAfterMove(const Move &move);
 void enterRobberPhase() { setPhase(TurnPhase::Robber); }

 bool applyMove(const Move& move);
 int rollDice();

 PlayerId localPlayer()   const { return m_localPlayerId; }
 PlayerId currentPlayer() const { return m_currentPlayerId; }
 int numPlayers() const { return static_cast<int>(m_players.size()); }
 TurnPhase phase() const { return m_phase; }

 Board& board() { return *m_board; }
 const Board& board() const { return *m_board; }

 const Player& player(PlayerId id) const { return *m_players.at(id); }
 Player& player(PlayerId id) { return *m_players.at(id); }

 void updateLongestRoad();
};

// TODO treba da se doda, tj vodi racuna o poenima, longestroad/vitez/devcards poeni isto!

#endif //Catan_GAMESESSION_H