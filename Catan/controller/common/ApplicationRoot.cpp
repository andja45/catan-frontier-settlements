//
// Created by matija on 1/27/26.
//

#include "ApplicationRoot.hpp"

#include <game/GameApp.hpp>
#include <history/HistoryApp.hpp>
#include <host-join/HostJoinApp.hpp>
#include <lobby/LobbyApp.hpp>
#include <menu/MenuApp.hpp>

ApplicationRoot::ApplicationRoot() {
    m_menu=new MenuApp(this);
    m_netService=new NetworkService();
    m_msgService=new PopupService();
}

void ApplicationRoot::showMainMenu() {
    clearPrevious();
    m_menu->show();
    m_netService->disconnect();

}

void ApplicationRoot::showLobbyHost() {
    clearPrevious();
    m_menu->hide();
    m_currentApplet = new LobbyApp(this,RoleType::Host);
    m_currentApplet->show();
}

void ApplicationRoot::showLobbyPlayer() {
    clearPrevious();
    m_menu->hide();
    m_currentApplet = new LobbyApp(this,RoleType::Player);
    m_currentApplet->show();

}

void ApplicationRoot::showHostPopup() {
    clearPrevious();
    m_currentApplet = new HostJoinApp(this,m_menu->getView(),RoleType::Host);
    m_currentApplet->show();
}

void ApplicationRoot::showJoinPopup() {
    clearPrevious();
    m_currentApplet= new HostJoinApp(this,m_menu->getView(),RoleType::Player);
    m_currentApplet->show();
}

void ApplicationRoot::startGame(const GameConfig &config, std::unique_ptr<Board> board, PlayerId myId, int seed) {
    clearPrevious();
    m_currentApplet=new GameApp(config,std::move(board),myId,seed,this);
    m_currentApplet->show();
}

void ApplicationRoot::showHistory() {
    clearPrevious();
    m_currentApplet=new HistoryApp(this);
    m_currentApplet->show();
}

void ApplicationRoot::closeApp() {
    clearPrevious();
    m_menu->hide();
}

void ApplicationRoot::startApp() {
    clearPrevious();
    m_menu->show();
}

void ApplicationRoot::clearPrevious() {
    if (m_currentApplet) {
        m_currentApplet->hide();
        delete m_currentApplet;
        m_currentApplet=nullptr;
    }
}

ApplicationRoot::~ApplicationRoot() {
    if (m_currentApplet) delete m_currentApplet;
    if (m_netService) delete m_netService;
    if (m_menu) delete m_menu;
    if (m_msgService) delete m_msgService;
}


