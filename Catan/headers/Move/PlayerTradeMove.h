//
// Created by andja on 10.12.25..
//

#ifndef TRADEMOVE_H
#define TRADEMOVE_H

#include "Move.h"

class PlayerTradeMove : public Move{
private:
    int m_fromPlayer;
    int m_toPlayer;
    ResourcePack m_offer;
    ResourcePack m_request;
public:
    explicit PlayerTradeMove(int from, int to, ResourcePack offer, ResourcePack request) : m_fromPlayer(from), m_toPlayer(to), m_offer(offer), m_request(request){}

    bool isValid(const GameSession&) const override;
    void apply(GameSession&) const override;
};


#endif //TRADEMOVE_H