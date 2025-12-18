//
// Created by andja on 11.12.25..
//

#ifndef Catan_GAMESESSION_H
#define Catan_GAMESESSION_H

 /*loopovi igranje igraca/bota i provere vict points road vitezi..*/

enum class TurnPhase {
 RollDice,
 Main, // regularan potez, build/trade/kupuje devcards, nakon rolldice
 Robber,
 InitialPlacement,
 End
};

class GameSession {
private:
 int m_currentPlayer = -1;
 int m_numPlayers = -1;
 TurnPhase m_phase = TurnPhase::InitialPlacement;
public:
 explicit GameSession(int numPlayers) : m_numPlayers(numPlayers) {}

 // --- Info ---
 int currentPlayer() const { return m_currentPlayer; }
 TurnPhase phase() const { return m_phase; }
 bool isPlayersTurn(int playerId) const { return playerId == m_currentPlayer; }

 // --- Guards (koristi ih Move::isValid) ---
 bool canRollDice() const;
 bool canBuild() const;
 bool canTrade() const;
 bool canEndTurn() const;

 // --- Transitions ---
 void enterRobberPhase();
 void endTurn();
};


#endif //Catan_GAMESESSION_H