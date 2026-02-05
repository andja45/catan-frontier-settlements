//
// Created by matija on 1/27/26.
//

#ifndef CATAN_GAMEAPP_HPP
#define CATAN_GAMEAPP_HPP
#include <common/AbstractApplet.hpp>
#include <game/GameController.hpp>
#include <model/GameConfig.h>
#include <model/GameSession.h>
#include <screens/game/GameWindow.h>

class GameController;

class GameApp : public AbstractApplet {
public:

    explicit GameApp(const GameConfig& config, std::unique_ptr<Board> board, PlayerId myId, int seed,ApplicationRoot *root);

    void show() override{m_gameWindow->show();}
    void hide() override{m_gameWindow->show();}

    ~GameApp() override {
        m_gameWindow->deleteLater();
        m_controller->deleteLater();
    }

private:
    GameController* m_controller;
    std::unique_ptr<GameSession> m_gameSession;
    GameWindow* m_gameWindow;
};

#endif //CATAN_GAMEAPP_HPP