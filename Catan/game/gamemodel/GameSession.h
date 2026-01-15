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
// game session is like engine for our game that gets moves from all players from server and executes them to apply changes and keep game running
 // it not only applies our rules but from other to so we keep track of which players turn is it and also which player are we


// TODO add moves activateDevCard move gets dev card type, check if player has it and uses it (decrements counter, applies effect or enters new phase in separate move per dev card)
// TODO add building bridges phase, and reuse initial phase counter (change name to phase counter), it receives build road move and ends after building two roads
// TODO add monopoly and resource phase as well (because we are not accepting those moves unless we paid and activated dev card), add moves for monopoly and resource
// alternative: merge robber move and steal card for knight and dice, merge activate and choose resource for monopoly and resource
// for multi-step moves (monopoly and resources) is valid is false until both get chosen, only then do action buttons becomes clickable and move sent
// keep bridge phase to reuse build road logic?

// TODO add trade request and accept moves

enum class TurnPhase {
 RollDice, // Start of turn, awaits dice roll
 Main, // regularan potez, build/trade/kupuje devcards, nakon rolldice
 Robber, // Phase choosing tile to move the robber and choosing player to steal a card // TODO consider breaking into robberChooseTile and choosePlayer for easier isValid highlighting purposes
 DiscardCards, // Game enters in this phase when 7 is rolled and waits for all players to send their discard cards moves
 InitialPlacement, // Special phase for initial placement turns (every player chooses one road and settlement) //TODO consider breaking into two phases and ditch enum
 End // mozda nije faza?
}; // drzi to stanje sve dok ne dobije signal da je uradjeno nesto sto ga menja
   // tj to je stanje koje ceka, ne ono koje sledi

enum class InitialPlacementStep {
 PlaceSettlement,
 PlaceRoad
};

// TODO game session should keep track of pending trade requests, add TradeRequest class with playerId owner, trade pack give and trade pack receive, and it stores players that accepted trade so owner can choose
// TODO when game gets offer move it saves trade request, this gets displayed to other players so they can choose to accept, when others accept its saved in trade struct and when owner does and it matches then trade happens according to stored data
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
 // game should be playable even without multiplayer, but in gui we will set buttons unclickable if currplayer != localplayer cus only he can make moves on his gui, other clients send him their moves and game session executes them
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