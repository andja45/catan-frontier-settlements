//
// Created by andja on 11.12.25.
//

#include "GameSession.h"

#include <cassert>

GameSession::GameSession(int numPlayers,
                         PlayerId localPlayer,
                         uint32_t seed)
    : m_board(std::make_unique<Board>())
    , m_players()
    , m_localPlayerId(localPlayer)
    , m_currentPlayerId(0)
    , m_phase(TurnPhase::InitialPlacement)
    , m_rng(seed)
{
    m_players.reserve(numPlayers);

    for (PlayerId id = 0; id < numPlayers; ++id) {
        m_players.push_back(std::make_unique<Player>(id));
    }
}

bool GameSession::applyMove(const Move& move){
    if (!move.isValid(*this))
        return false;
    move.apply(*this);

    //notifyModelChanged(); notify view
    return true;
}

int GameSession::rollDice() {
    assert(m_phase == TurnPhase::RollDice);

    int dice1 = m_d6(m_rng);
    int dice2 = m_d6(m_rng);
    return dice1 + dice2;
}




// dodaj prelaz na sl igraca, ako je u initalplacement ostaje tu,
// inace ide u rolldice - ali nije tako jednostavno, treba impl i izlazak
// iz initialplacement(svi se postavili)

// TODO
// za mrezu, drugi igraci imaju model i view + trenutno stanje iz gamesession
// (za crtanje koji dugmici su sivi-unclickable), samo treba resiti negde se cuva koji
// playerid si ti -> od toga isto zavisi tvoj view(karte ime)
// startgame dugme pravi gamesession i popunjava igrace(pokupi info pre starta dobijen)