//
// Created by andja on 28.1.26..
//

#ifndef CATAN_CHOOSEPLAYERRENDERSTATE_H
#define CATAN_CHOOSEPLAYERRENDERSTATE_H
#include <vector>
#include <player/Player.h>

class ChoosePlayerRenderState {
private:
    std::vector<PlayerId> m_players;
public:
    void setPlayers(const std::vector<PlayerId> &players) {
        m_players = players;
    }
    std::vector<PlayerId> getPlayers() const { return m_players; }
};


#endif //CATAN_CHOOSEPLAYERRENDERSTATE_H