//
// Created by matija on 1/27/26.
//

#ifndef CATAN_LOBBYAPP_HPP
#define CATAN_LOBBYAPP_HPP
#include <model/GameConfig.h>
#include <screens/lobby/LobbyView.h>

#include "LobbyController.hpp"
#include "../common/AbstractApplet.hpp"
#include <QObject>

class LobbyApp : public QObject, public AbstractApplet{
    Q_OBJECT
public:
    explicit LobbyApp(ApplicationRoot *root, RoleType role);

    ~LobbyApp() override{
        m_lobbyView->deleteLater();
        m_lobbyController->deleteLater();
    }

    void show() override {m_lobbyView->show();}
    void hide() override {m_lobbyView->hide();}

private:
    //GameConfig  m_config; //model
    LobbyView* m_lobbyView;
    LobbyController* m_lobbyController; //controller
};

#endif //CATAN_LOBBYAPP_HPP