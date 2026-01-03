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
 InitialPlacement,
 End // mozda nije faza?
}; // drzi to stanje sve dok ne dobije signal da je uradjeno nesto sto ga menja
   // tj to je stanje koje ceka, ne ono koje sledi

class GameSession {
private:
 std::unique_ptr<Board> m_board;
 std::vector<std::unique_ptr<Player>> m_players;

 PlayerId m_localPlayerId   = -1; // ko sam ja?
 PlayerId m_currentPlayerId = -1;
 TurnPhase m_phase = TurnPhase::InitialPlacement;

 std::mt19937 m_rng;
 std::uniform_int_distribution<int> m_d6{1, 6};
public:
 GameSession(int numPlayers, PlayerId localPlayer, uint32_t seed);

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
};

// TODO treba da se doda, tj vodi racuna o poenima, longestroad/vitez/devcards poeni isto!

#endif //Catan_GAMESESSION_H