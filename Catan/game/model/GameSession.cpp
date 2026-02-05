//
// Created by andja on 11.12.25.
//

#include "GameSession.h"

#include <cassert>
#include <move/Move.h>
#include <move/robber/StealCardMove.h>
#include <move/trade/PlayerTradeRequestMove.h>

GameSession::GameSession(std::vector<std::string> playerNames,
                         PlayerId localPlayer,
                         uint32_t seed,
                         std::unique_ptr<Board> board,
                         int winPoints,
                         std::string gameName)
    : m_board(std::move(board))
    , m_localPlayerId(localPlayer)
    , m_rng(seed)
    , m_gameData(gameName, playerNames) // TODO gameId will be set by client-host maybe on creation of room? roomId?
{
    const int numPlayers = static_cast<int>(playerNames.size()); // deduced from vector of players
    m_numOfActivePlayers=numPlayers;
    m_players.reserve(numPlayers);
    for (PlayerId id = 0; id < numPlayers; ++id) {
        m_players.push_back(std::make_unique<Player>(id, playerNames[id]));
    }

    m_turnIndex = 0;
    m_currentPlayerId = m_players[m_turnIndex]->getPlayerId();

    m_gameData.initialize();
}

bool GameSession::applyMove(const Move& move){
    if (!move.isValid(*this))
        return false;

    move.apply(*this);
    m_lastMoveType = move.type(); // this is where we remember last move

    m_rules.evaluate(*this); // after every move, cus someone can interrupt longest road for example
    advancePhaseAfterMove(); // only session can advance phases, move only reads them

    return true;
}

void GameSession::advancePhaseAfterMove() {
    // phase driven transitions
    switch (m_phase) {
        case TurnPhase::InitialPlacement:
            advanceInitialPlacement();
            return;

        case TurnPhase::RoadBuilding:
            incrementPhaseMoveCount();
            if (m_phaseMoveCount >= 2) {
                setPhase(TurnPhase::Main);
                m_phaseMoveCount = 0;
            }
            return;

        case TurnPhase::DiscardCards:
            if (allRequiredPlayersDiscarded()) {
                setPhase(TurnPhase::SetRobber);
                //m_discardedPlayers.clear();
            }
            return;

        default:
            break;
    }

    // move driven transitions
    switch (m_lastMoveType) {
        case MoveType::RollDice:
            setPhase(TurnPhase::Main);
            break;

        case MoveType::SetRobber:
            if (canStealFromAnyone(m_currentPlayerId)) {
                setPhase(TurnPhase::StealCard);
            } else { // we established we can place on tile no one is at, in that case every player is greyed out
                setPhase(TurnPhase::Main);
            }
            break;

        case MoveType::EndTurn:
            advancePlayer(); // session does this, not move
            m_devCardPlayedThisTurn = false; // reseting for next turn
            m_activeTrades.clear();
            m_gameData.addTurn();
            setPhase(TurnPhase::RollDice);
            break;

        default:
            break;
        }
}

void GameSession::enterDevCardPhase(DevCardType type) {
    switch (type) {
        case DevCardType::Knight:
            setPhase(TurnPhase::SetRobber);
            break;

        case DevCardType::RoadBuilding:
            setPhase(TurnPhase::RoadBuilding);
            break;

        case DevCardType::YearOfPlenty:
            setPhase(TurnPhase::YearOfPlenty);
            break;

        case DevCardType::Monopoly:
            setPhase(TurnPhase::Monopoly);
            break;

        default:
            break;
    }
}

void GameSession::advanceInitialPlacement() {
    const int playerCount = numPlayers();
    if (playerCount == 0 || m_numOfActivePlayers<=0) return;

    m_phaseMoveCount += 1;

    const int totalPlacementMoves = playerCount * 4; // *2 for both directios *2 for settlemet/road

    if (m_phaseMoveCount >= totalPlacementMoves) {
        m_phaseMoveCount = 0;
        m_turnIndex = 0;
        m_currentPlayerId = m_players[m_turnIndex]->getPlayerId(); // first player starts
        setPhase(TurnPhase::RollDice);
        return;
    }

    const int placementTurn = m_phaseMoveCount / 2; // each player gets settlement + road in one placing

    int playerIndex;
    if (placementTurn < playerCount) {
        playerIndex = placementTurn;
    }
    else {
        playerIndex = (playerCount - 1) - (placementTurn - playerCount);
    }

    m_currentPlayerId = m_players[playerIndex]->getPlayerId(); // if player needs to be changed, he changes

    if (!player(m_currentPlayerId).isActive()) {
        advancePlayer();
    }
}


void GameSession::advancePlayer() { // TODO maybe change later - add shuffling with same seed every client model uses
    if (m_players.empty() || m_numOfActivePlayers<=0) return;

    m_turnIndex = (m_turnIndex + 1) % m_players.size();
    m_currentPlayerId = m_players[m_turnIndex]->getPlayerId();

    if (!player(m_currentPlayerId).isActive()) {
        advancePlayer();
    }

}

void GameSession::addTrade(Trade trade) {
    trade.setId(m_nextTradeId++);
    m_activeTrades.emplace(trade.id(), std::move(trade));
}

void GameSession::removeTrade(TradeId tradeId) {
    m_activeTrades.erase(tradeId);
}

Trade * GameSession::getTrade(TradeId tradeId) {
    auto it = m_activeTrades.find(tradeId);
    return (it != m_activeTrades.end()) ? &it->second : nullptr;
}

int GameSession::rollDice() {
    assert(m_phase == TurnPhase::RollDice);

    int dice1 = m_d6(m_rng);
    int dice2 = m_d6(m_rng);

    return dice1 + dice2;
}

void GameSession::setLongestRoadOwner(const PlayerId playerId) {
    if (m_longestRoadOwner == playerId)
        return; // no change

    if (longestRoadOwner() != types::InvalidPlayerId){
        Player& previous = player(m_longestRoadOwner);
        previous.removePoints(2);
        previous.setLongestRoad(false);
    }

    m_longestRoadOwner = playerId;

    Player& current = player(m_longestRoadOwner);
    current.addPoints(2);
    current.setLongestRoad(true);
} // TODO add length of longestroad in player

void GameSession::setLargestArmyOwner(const PlayerId playerId) {
    if (m_largestArmyOwner == playerId)
        return; // no change

    if (largestArmyOwner() != types::InvalidPlayerId){
        Player& previous = player(m_largestArmyOwner);
        previous.removePoints(2);
        previous.setLargestArmy(false);
    }

    m_largestArmyOwner = playerId;

    Player& current = player(m_largestArmyOwner);
    current.addPoints(2);
    current.setLargestArmy(true);
}

std::vector<PlayerId> GameSession::playerIds() const {
    std::vector<PlayerId> ids;
    ids.reserve(m_players.size());
    for (const auto& p : m_players) {
        ids.push_back(p->getPlayerId());
    }
    return ids;
}

bool GameSession::playerMustDiscard(PlayerId playerId) const {
    return player(playerId).getNumOfResourceCards() > 7;
}

bool GameSession::hasPlayerDiscarded(PlayerId playerId) const {
    return m_discardedPlayers.find(playerId) != m_discardedPlayers.end();
}

void GameSession::markPlayerDiscarded(PlayerId playerId) {
    m_discardedPlayers.insert(playerId);
}

bool GameSession::allRequiredPlayersDiscarded() const {
    for (PlayerId playerId : playerIds()) {
        if (playerMustDiscard(playerId) && !hasPlayerDiscarded(playerId)) // if someone has to discard and didnt
            return false;
    }
    return true;
}

const Trade* GameSession::getTrade(TradeId tradeId) const {
    auto it = m_activeTrades.find(tradeId);
    return it != m_activeTrades.end() ? &it->second : nullptr;
}

std::vector<const Trade*> GameSession::activeTrades() const {
    std::vector<const Trade*> result;
    result.reserve(m_activeTrades.size());

    for (const auto& [id, trade] : m_activeTrades) {
        result.push_back(&trade);
    }
    return result;
}

bool GameSession::canStealFromAnyone(PlayerId thiefId) const {
   for (PlayerId victimId : playerIds()) {
        if (victimId == thiefId)
            continue;

        StealCardMove testMove(thiefId, victimId);
        if (testMove.isValid(*this))
            return true;
    }

    return false;
}

void GameSession::endGame() {
    if (m_phase == TurnPhase::GameOver)
        return;

    setPhase(TurnPhase::GameOver);
    m_gameData.markGameWon();

    for (const auto& player : m_players) {
        m_gameData.setPlayerPoints(
            player->getName(),
            player->getTotalPoints()
        );
    }

    Player& winningPlayer = player(winner());
    m_gameData.setWinningPlayer(winningPlayer.getName());

    if (m_largestArmyOwner != types::InvalidPlayerId) {
        m_gameData.setLargestArmyOwner(
            player(m_largestArmyOwner).getName()
        );
    }

    if (m_longestRoadOwner != types::InvalidPlayerId) {
        m_gameData.setLongestRoadOwner(
            player(m_longestRoadOwner).getName()
        );
    }

    m_isOver=true;
} // gui gets renderstate that behaves differently in gameover phase, onphasechanged triggers redrawing

void GameSession::leavePlayer(PlayerId player_id) {
    player(player_id).setLeft();
    m_numOfActivePlayers--;
}

