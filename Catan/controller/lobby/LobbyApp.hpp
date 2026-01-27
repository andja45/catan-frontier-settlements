//
// Created by matija on 1/27/26.
//

#ifndef CATAN_LOBBYAPP_HPP
#define CATAN_LOBBYAPP_HPP
#include <model/GameConfig.h>

#include "LobbyController.hpp"
#include "../AbstractApplet.hpp"
#include "../RoleEnum.hpp"

class LobbyApp : public AbstractApplet{
public:
    explicit LobbyApp(ApplicationRoot *root, Role role);

    void start() override;

    void exit() override;

    void switchToNext() override;

    void switchToPrevious() override;

private:
    GameConfig  m_config; //model
    //view
    LobbyController m_lobbyController; //controller
};

#endif //CATAN_LOBBYAPP_HPP