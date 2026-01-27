//
// Created by matija on 1/27/26.
//

#ifndef CATAN_NAVIGATOR_HPP
#define CATAN_NAVIGATOR_HPP
#include <memory>
#include <board/Board.h>
#include <model/GameConfig.h>

#include "AbstractApplet.hpp"

class ApplicationRoot {
public:
    ApplicationRoot();
    void showMainMenu();
    void showHost();
    void showJoin();
    void showLobbyHost(const GameConfig& config);
    void showLobbyPlayer(const GameConfig& config);
    void startGame(const GameConfig& config, std::unique_ptr<Board> board, PlayerId myId, int seed);

private:
    AbstractApplet* m_currentApplet;
};

#endif //CATAN_NAVIGATOR_HPP