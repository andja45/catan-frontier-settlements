//
// Created by andja on 11.12.25..
//

#ifndef Catan_GAMESESSION_H
#define Catan_GAMESESSION_H
#include "GameModel.h"

/*loopovi igranje igraca/bota i provere vict points road vitezi..*/

enum class TurnPhase {
 RollDice,
 Main, // regularan potez, build/trade/kupuje devcards, nakon rolldice
 Robber,
 InitialPlacement,
 End
}; // drzi to stanje sve dok ne dobije signal da je uradjeno nesto sto ga menja
   // tj to je stanje koje ceka, ne ono koje sledi

class GameSession {
private:
 GameModel m_model;
 int m_currentPlayer = -1;
 int m_numPlayers = -1;
 TurnPhase m_phase = TurnPhase::InitialPlacement;
public:
 explicit GameSession(int numPlayers) : m_model(numPlayers), m_numPlayers(numPlayers) {}

 bool applyMove(const Move& move);

 // --- Info ---
 int currentPlayer() const { return m_currentPlayer; }
 int numPlayers() const { return m_numPlayers; }
 TurnPhase phase() const { return m_phase; }
 bool isPlayersTurn(int playerId) const { return playerId == m_currentPlayer; }
 bool isInitialPlacement() const { return m_phase == TurnPhase::InitialPlacement; }

 // --- Guards (koristi ih Move::isValid) ---
 bool canRollDice() const;
 bool canBuild() const;
 bool canTrade() const;
 bool canPlaceRobber() const;
 bool canEndTurn() const;

 // --- Transitions ---
 void enterRobberPhase();
 void enterMainPhase();
 void endTurn();

 // --- Model ---
 GameModel& model() { return m_model; }
 const GameModel& model() const { return m_model; }
};

// TODO treba da se doda, tj vodi racuna o poenima, longestroad/vitez/devcards poeni isto!

#endif //Catan_GAMESESSION_H