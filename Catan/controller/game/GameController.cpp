//
// Created by matija on 1/24/26.
//

#include "GameController.hpp"

#include <utility>
#include <QDebug>
#include <common/PathService.hpp>
#include <screens/game/action-popups/popups/DiscardPopup.h>
#include <screens/game/action-popups/popups/YearOfPlentyPopup.h>

#include "move/BoardMove.h"
#include "move/build/BuildCityMove.h"
#include "move/build/BuildRoadMove.h"
#include "move/build/BuildSettlementMove.h"
#include "move/devcard/BuyDevCardMove.h"
#include "move/devcard/MonopolyMove.h"
#include "move/devcard/YearOfPlentyMove.h"
#include "move/robber/DiscardCardsMove.h"
#include "move/robber/SetRobberMove.h"
#include "move/robber/StealCardMove.h"
#include "move/trade/PlayerTradeAcceptMove.h"
#include "move/trade/PlayerTradeRequestMove.h"
#include "move/trade/PlayerTradeResponseMove.h"
#include "move/turn/EndTurnMove.h"
#include "move/turn/PlayerLeaveMove.hpp"
#include "move/turn/RollDiceMove.h"
#include "move/MoveToString.h"

GameController::GameController(GameSession& session, NetworkTransport* transport, GameWindow &gameWindow, QObject* parent)
    : QObject(parent), m_session(session), m_gameWindow(gameWindow)
    {
    m_localPlayerId = session.localPlayer();
    m_adapter = new GameNetworkAdapter(this);
    m_adapter->setTransport(transport);

    connectElements();
    updateState();
    updateView();
    m_gameWindow.getOverlay()->show();
    m_adapter->sendReady();

}

bool GameController::isLocalPlayersTurn(const char* action) const {
    if (m_session.currentPlayer() == m_localPlayerId)
        return true;

    qDebug() << action << ": not local player's turn";
    return false;
}

void GameController::updateState(){ // filling renderstates and tools
    m_boardRenderState.clear();
    m_toolbarRenderState.clear();


    if (m_session.localPlayer()==m_session.currentPlayer()) {
        emit gameOverlay(GameOverlayType::Hidden);
    }
    else {
        emit gameOverlay(GameOverlayType::Empty);
    }

    if (m_session.phase()==TurnPhase::DiscardCards) {
        if (!m_session.hasPlayerDiscarded(m_localPlayerId) && m_session.playerMustDiscard(m_localPlayerId))
            emit setDiscard();
        else
            emit gameOverlay(GameOverlayType::Waiting);
    }

    if (m_session.localPlayer()==m_session.currentPlayer()) {
        updateActiveToolOnPhase();
        m_toolbarRenderState.updateFromPhase(m_session.phase());
        if (m_session.hasPlayedDevCardThisTurn()) {
            m_toolbarRenderState.disableDev();
        }
        if (m_activeTool && m_activeTool->providesAllValid()) {
            m_boardRenderState.setHighlighted(m_activeTool->allValid(m_session),
                m_activeTool->type());
        }

        if (m_session.phase()==TurnPhase::StealCard) {
            emit setChoosePlayer(m_activeTool->allValid(m_session));
        }
    }

    if (m_session.phase()==TurnPhase::GameOver) {
        onGameOver();
    }

}

void GameController::setActiveTool(std::unique_ptr<Move> tool){
    m_activeTool = std::move(tool);

}

void GameController::clearActiveTool(){
    m_activeTool.reset();
    m_activeTool=nullptr;
}

void GameController::connectElements() {

    auto qboard= m_gameWindow.getBoard();
    auto qtoolbar= m_gameWindow.getToolbar();
    auto qchat = m_gameWindow.getChat();
    auto qtradeOverlay = m_gameWindow.getTradeOverlay();
    auto qactionManager = m_gameWindow.getActionManager();
    auto qgameOverlay = m_gameWindow.getOverlay();
    auto qplayersOverlay = m_gameWindow.getPlayersOverlay();

    // ADAPTER
    // receive move from adapter
    connect(m_adapter,&GameNetworkAdapter::startGame,this,&GameController::onGameStart);
    connect(m_adapter,&GameNetworkAdapter::remoteMoveReceived,
            this, &GameController::onMoveReceived);
    // error from adapter
    connect(m_adapter,&GameNetworkAdapter::errored,this,[this](const std::string& error) {
        m_adapter->sendMove( new PlayerLeaveMove(m_session.localPlayer()));
        emit gameClosed();
    });
    // disconnect
    connect(m_adapter,&GameNetworkAdapter::disconnected,this,[this]() {
        m_adapter->sendMove( new PlayerLeaveMove(m_session.localPlayer()));
        emit gameClosed();
    });
    // receive message from adapter
    connect(m_adapter,&GameNetworkAdapter::remoteMessageReceived,
            qchat, [qchat](std::string a,std::string m) {
                qchat->addChatMessage(QString::fromStdString(a), QString::fromStdString(m));
            });
    // send leave to adapter after game closes
    connect(&m_gameWindow,&GameWindow::closed,this,[this]() {
        m_adapter->sendMove( new PlayerLeaveMove(m_session.localPlayer()));
        emit gameClosed();
    });
    // send message from chat to adapter
    connect(qchat,&Chat::chatSendRequested,this, [this](const QString& msg) {
        m_adapter->sendMessage(m_session.player(m_session.localPlayer()).getName(),msg.toStdString());
    });

    // TOOLBAR
    // build buttons
    connect(qtoolbar,&BoardToolbar::buildRoadClicked,
            this,&GameController::onBuildRoadClicked);
    connect(qtoolbar,&BoardToolbar::buildSettlementClicked,
            this,&GameController::onBuildSettlementClicked);
    connect(qtoolbar,&BoardToolbar::buildCityClicked,
            this,&GameController::onBuildCityClicked);

    // other buttons
    connect(qtoolbar,&BoardToolbar::buyDevCardRequested,
            this,&GameController::onBuyDevCardClicked);
    connect(qtoolbar,&BoardToolbar::endTurnRequested,
            this,&GameController::onEndTurnClicked);
    connect(qtoolbar,&BoardToolbar::diceRolled,
            this,&GameController::onRollDiceClicked);
    connect(qtoolbar,&BoardToolbar::devCardChosen,
            this,&GameController::onUseDevCardClicked);

    // popup buttons
    connect(qtoolbar,&BoardToolbar::bankTradeRequested,this,[this](TradeOffer offer) {
        onBankTradeSent(offer.give.begin()->first, offer.receive.begin()->first);
    });
    connect(qtoolbar,&BoardToolbar::playerTradeRequested,this,[this](const TradeOffer& offer) {
        onPlayerTradeRequestSent(offer.give,offer.receive);
    });
    connect(qtoolbar,&BoardToolbar::devCardChosen,this,&GameController::onUseDevCardClicked);

    // BOARD
    connect(qboard,&QBoard::elementClicked,this,&GameController::onBoardElementClicked);
    connect(this,&GameController::buildPlaced,qboard,&QBoard::onPlayBuildFeedback);

    // ACTION POPUPS
    connect(qactionManager,&ActionManager::monopolyResourceChosen,this,&GameController::onMonopolyResourceChosen);
    connect(qactionManager,&ActionManager::yearOfPlentySubmitted,this,[this](const YearOfPlentyChoice& c) {
        onYearOfPlentyResourcesChosen((c.receive.begin()->first),(++c.receive.begin())->first);
    });
    connect(qactionManager,&ActionManager::playerChosenToSteal,this,&GameController::onStealCardPlayerChosen);
    connect(qactionManager,&ActionManager::discardConfirmed,this,[this](const DiscardChoice& choice) {
        onDiscardCardsSent({choice.discard});
    });

    // TRADE
    // accept
    connect(qtradeOverlay,&TradeOverlay::tradeAccepted,this,&GameController::onPlayerTradeAcceptSent);
    // respond
    connect(qtradeOverlay,&TradeOverlay::tradeRespondedPositive,this,[this](PlayerId pid,TradeId tid) {
        onPlayerTradeResponseSent(tid,true);
    });
    connect(qtradeOverlay,&TradeOverlay::tradeRespondedNegative,this,[this](PlayerId pid,TradeId tid) {
        onPlayerTradeResponseSent(tid,false);
    });

    // GAME OVERLAY
    connect(this,&GameController::gameOverlay,qgameOverlay,&GameOverlay::overlay);

    // UPDATES TO VIEW
    connect(this,&GameController::updateToolbar,qtoolbar,&BoardToolbar::updateState);
    connect(this,&GameController::updateBoard,qboard,&QBoard::update);
    connect(this,&GameController::updateActivePlayer,qplayersOverlay,&PlayersOverlay::update);
    connect(this,&GameController::update,qtradeOverlay,&TradeOverlay::refresh);

    connect(this,&GameController::setDiscard,qactionManager,&ActionManager::openDiscardPopup);
    connect(this,&GameController::setChoosePlayer,qactionManager,&ActionManager::setStealCandidates);
    connect(this,&GameController::setDiscard,qactionManager,&ActionManager::openDiscardPopup);

}

void GameController::updateActiveToolOnPhase(){
    switch (m_session.phase()) { // these are activetools that become active when force phase change, not clicking
        case TurnPhase::SetRobber:
            setActiveTool(std::make_unique<SetRobberMove>(m_localPlayerId, types::InvalidTileId));
            break;

        case TurnPhase::StealCard:
            setActiveTool(std::make_unique<StealCardMove>(m_localPlayerId, types::InvalidPlayerId));
            break;

        case TurnPhase::RoadBuilding: // remove this if we want player to click on road himself (uncomment required toolbarrenderstate code)
            setActiveTool(std::make_unique<BuildRoadMove>(m_localPlayerId, types::InvalidEdgeId));
            break;

        case TurnPhase::InitialPlacement:
            if (m_session.lastMoveType() == MoveType::InvalidMoveType || // for first move of the game
                m_session.lastMoveType() == MoveType::BuildRoad)
                setActiveTool(std::make_unique<BuildSettlementMove>(m_localPlayerId, types::InvalidNodeId));
            else if (m_session.lastMoveType() == MoveType::BuildSettlement)
                setActiveTool(std::make_unique<BuildRoadMove>(m_localPlayerId, types::InvalidEdgeId));
            break;

        default:
            break;
    }
}

void GameController::onBuildRoadClicked(){
    if (!isLocalPlayersTurn("BuildRoad")) return;

    setActiveTool(std::make_unique<BuildRoadMove>(m_localPlayerId, types::InvalidEdgeId));
    updateState();
    updateView();
}

void GameController::onBuildSettlementClicked(){
    if (!isLocalPlayersTurn("BuildSettlement")) return;

    setActiveTool(std::make_unique<BuildSettlementMove>(m_localPlayerId, types::InvalidNodeId));
    updateState();
    updateView();
}

void GameController::onBuildCityClicked() {
    if (!isLocalPlayersTurn("BuildCity")) return;

    setActiveTool(std::make_unique<BuildCityMove>(m_localPlayerId, types::InvalidNodeId));
    updateState();
    updateView();
}

void GameController::onBoardElementClicked(int elementId){
    if (!isLocalPlayersTurn("BoardElementClicked")) return;

    if (!m_activeTool) return;

    auto* boardMove = dynamic_cast<BoardMove*>(m_activeTool.get());
    if (!boardMove) return;

    boardMove->setBoardElementId(elementId);
    if (sendMove(m_activeTool.get()))
        emit buildPlaced(); // shake only after successful placement
}

void GameController::onBuyDevCardClicked() {
    if (!isLocalPlayersTurn("BuyDev")) return;

    auto move = std::make_unique<BuyDevCardMove>(m_localPlayerId);
    move->setDevCard(m_session);

    sendMove(move.get());
}

void GameController::onUseDevCardClicked(DevCardType cardType) {
    if (!isLocalPlayersTurn("UseDev")) return;

    auto move = std::make_unique<PlayDevCardMove>(m_localPlayerId, cardType);

    sendMove(move.get());
}

void GameController::onStealCardPlayerChosen(PlayerId victimId) {
    if (!isLocalPlayersTurn("StealCard")) return;

    if (!m_activeTool) return;

    auto* move = dynamic_cast<StealCardMove*>(m_activeTool.get());
    if (!move) return;

    move->setVictimPlayerId(victimId);
    sendMove(m_activeTool.get());
}

void GameController::onMonopolyResourceChosen(ResourceType resource) {
    if (!isLocalPlayersTurn("Monopoly")) return;

    auto move = std::make_unique<MonopolyMove>(m_localPlayerId, resource);

    sendMove(move.get());
}

void GameController::onYearOfPlentyResourcesChosen(ResourceType resource1, ResourceType resource2) {
    if (!isLocalPlayersTurn("YearOfPlenty")) return;

    auto move = std::make_unique<YearOfPlentyMove>(m_localPlayerId, resource1, resource2);

    sendMove(move.get());
}

void GameController::onDiscardCardsSent(const ResourcePack &discarded) {
    auto move = std::make_unique<DiscardCardsMove>(m_localPlayerId, discarded);
    sendMove(move.get());
}

void GameController::onPlayerTradeRequestSent(const ResourcePack &give, const ResourcePack &receive) {
    if (!isLocalPlayersTurn("PlayerTradeRequest")) return;

    std::unique_ptr<PlayerTradeRequestMove> move = std::make_unique<PlayerTradeRequestMove>(m_localPlayerId, give, receive);
    move->setTradeId(m_session.getNextTradeId());
    sendMove(move.get());
}

void GameController::onPlayerTradeResponseSent(TradeId tid,bool response) {
    // doesnt need to be local player!!!
    auto move = std::make_unique<PlayerTradeResponseMove>(m_localPlayerId,tid,response);

    sendMove(move.get());
}

void GameController::onPlayerTradeAcceptSent(TradeId tradeId, PlayerId acceptedPlayerId) {
    // doesnt need to be local player!!!
    if (!isLocalPlayersTurn("PlayerTradeAccept")) return;
    auto move = std::make_unique<PlayerTradeAcceptMove>(m_localPlayerId, acceptedPlayerId,tradeId);

    sendMove(move.get());
}

void GameController::onBankTradeSent(ResourceType give, ResourceType receive) {
    if (!isLocalPlayersTurn("BankTrade")) return;

    auto move = std::make_unique<BankTradeMove>(m_localPlayerId, give, receive);

    sendMove(move.get());
}

void GameController::onRollDiceClicked() {
    if (!isLocalPlayersTurn("RollDice")) return;
    if (m_session.phase()!=TurnPhase::RollDice) return;

    std::unique_ptr<RollDiceMove> move = std::make_unique<RollDiceMove>(m_localPlayerId);
    move->setDiceRoll(m_session);
    sendMove(move.get());
}

void GameController::onEndTurnClicked() {
    if (!isLocalPlayersTurn("EndTurn")) return;

    m_boardRenderState.clear();
    m_toolbarRenderState.clear(); // will remain cleared for local player until his next turn

    auto move = std::make_unique<EndTurnMove>(m_localPlayerId);

    sendMove(move.get());
}

void GameController::onMoveReceived(Move* receivedMove){
    std::unique_ptr<Move> move(receivedMove);
    if (receivedMove->getPlayerId()==m_localPlayerId) { // we skip moves from us, this is fallback server already does this
        return;
    }

    const std::string logLine = moveToString(*move, &m_session);

    if (!move) {
        onError("Received empty move!");
    }

    if (!m_session.applyMove(*move)) {
        onError("Server Received invalid move!");
        return;
    }

    if (auto* qchat = m_gameWindow.getChat()) {
        qchat->addChatMessage("", QString::fromStdString(logLine), true);
    }

    if (m_session.currentPlayer()==m_localPlayerId) {
        updateState();
    }
    updateView(); // renderstates will be cleared since im not active player rn, this will only make view redraw for me since someone else played
    updateState();
}

void GameController::updateView() {
    emit updateActivePlayer(m_session.currentPlayer());
    emit updateToolbar(m_toolbarRenderState);
    emit updateBoard(m_boardRenderState);
    emit update();
}

void GameController::onGameStart() {
    m_gameWindow.getOverlay()->hide();
    updateState();
    updateView();
}

bool GameController::sendMove(const Move *move) {
    if (!m_session.applyMove(*move)) { // we apply local moves ourselves
        qDebug()  <<"Tried to send invalid move!";
        return false;
    }

    if (auto* qchat = m_gameWindow.getChat()) {
        qchat->addChatMessage("", QString::fromStdString(moveToString(*move, &m_session)), true);
    }

    m_adapter->sendMove(move);
    clearActiveTool(); // tool isnt active anymore after sending move (even sequential moves are made of multiple moves)
    updateState();
    updateView();
    return true;
}

void GameController::onError(const std::string& error) {
    qDebug() << error.c_str();
    auto leaveMove = std::make_unique<PlayerLeaveMove>(m_session.localPlayer());
    sendMove(leaveMove.get());
    emit gameClosed();
}

void GameController::onGameOver() {
    if (m_session.winner()==m_localPlayerId){
        emit gameOverlay(GameOverlayType::GameWon);
        emit gameWon();
    }
    else {
        emit gameOverlay(GameOverlayType::GameOver);
        emit gameOver();
    }
    auto path=PathService::instance().resourceDir().toStdString();
    auto data = m_session.gameData();
    data.writeToFile(path);
}
