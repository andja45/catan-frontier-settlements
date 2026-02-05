//
// Created by matija on 1/31/26.
//

#include "ActionManager.hpp"

#include <iostream>
#include <QWidget>

#include "popups/DiscardPopup.h"
#include "popups/StealCardPopup.h"
#include "popups/MonopolyPopup.h"
#include "popups/YearOfPlentyPopup.h"
#include <QVector>



ActionManager::ActionManager(std::vector<Player*> players,PlayerId local, Bank *bank, QWidget *parent, QWidget *anchor)
    : QObject(parent), m_bank(bank), m_player(players[local]), m_players(players), m_localId(local),
      m_popupParent(parent),
      m_anchorWidget(anchor ? anchor : parent) {

}

void ActionManager::setStealCandidates(const std::unordered_set<PlayerId>& set) {
    openStealCardPopup(std::vector<PlayerId>(set.begin(), set.end()));
}

void ActionManager::openActionPopup(DevCardType type) {
    if (type==DevCardType::Monopoly) openMonopolyPopup();
    else if (type==DevCardType::YearOfPlenty) openYearOfPlentyPopup();
}

void ActionManager::setAnchorWidget(QWidget* anchorWidget) {
    m_anchorWidget = anchorWidget;
}


void ActionManager::openDiscardPopup() {
    auto* popup = ensureDiscardPopup();
    if (!popup) return;

    hideAllExcept(popup);
    popup->restart();
    //popup->openAtGlobal(m_player,m_bank , globalPos.isNull() ? defaultGlobalPos() : globalPos);
    popup->openAtGlobal(m_player,m_bank,{0,0});
    popup->raise();
}

void ActionManager::openStealCardPopup(const std::vector<PlayerId>& candidates) {
    std::vector<Player*> players;
    for (auto id : candidates) players.push_back(m_players[id]);
    auto* popup = ensureStealCardPopup();
    if (!popup) return;

    popup->setCandidates(QVector<Player*>(players.begin(), players.end()));

    hideAllExcept(popup);
    //popup->openAtGlobal(globalPos.isNull() ? defaultGlobalPos() : globalPos);
    popup->openAtGlobal({0,0});
    popup->raise();
}

void ActionManager::openMonopolyPopup() {
    auto* popup = ensureMonopolyPopup();
    if (!popup) return;

    hideAllExcept(popup);
    //popup->openAtGlobal(globalPos.isNull() ? defaultGlobalPos() : globalPos);
    popup->openAtGlobal({0,0});
    popup->raise();
}

void ActionManager::openYearOfPlentyPopup() {
    auto* popup = ensureYearOfPlentyPopup();
    if (!popup) return;

    hideAllExcept(popup);
    popup->resetState();
    //popup->openAtGlobal(globalPos.isNull() ? defaultGlobalPos() : globalPos);
    popup->openAtGlobal({0,0});
    popup->raise();
}

void ActionManager::closeAll() {
    if (m_discardPopup) m_discardPopup->closePopup();
    if (m_stealPopup) m_stealPopup->closePopup();
    if (m_monopolyPopup) m_monopolyPopup->closePopup();
    if (m_yearOfPlentyPopup) m_yearOfPlentyPopup->closePopup();
}

QPoint ActionManager::defaultGlobalPos() const {
    auto* anchor = m_anchorWidget ? m_anchorWidget : m_popupParent;
    if (!anchor) return QPoint(50, 50);

    return anchor->mapToGlobal(anchor->rect().center()) + QPoint(12, 12);
}

void ActionManager::hideAllExcept(QWidget* keep) {
    if (m_discardPopup && m_discardPopup != keep) m_discardPopup->hide();
    if (m_stealPopup && m_stealPopup != keep) m_stealPopup->hide();
    if (m_monopolyPopup && m_monopolyPopup != keep) m_monopolyPopup->hide();
    if (m_yearOfPlentyPopup && m_yearOfPlentyPopup != keep) m_yearOfPlentyPopup->hide();
}

DiscardPopup* ActionManager::ensureDiscardPopup() {
    if (m_discardPopup) return m_discardPopup;

    m_discardPopup = new DiscardPopup(m_player, m_popupParent);
    connect(m_discardPopup, &DiscardPopup::discardConfirmed, this, &ActionManager::discardConfirmed);
    return m_discardPopup;
}

StealCardPopup* ActionManager::ensureStealCardPopup() {
    if (m_stealPopup) return m_stealPopup;

    m_stealPopup = new StealCardPopup(m_popupParent);
    connect(m_stealPopup, &StealCardPopup::playerChosen, this, &ActionManager::playerChosenToSteal);
    return m_stealPopup;
}

MonopolyPopup* ActionManager::ensureMonopolyPopup() {
    if (m_monopolyPopup) return m_monopolyPopup;

    m_monopolyPopup = new MonopolyPopup(m_popupParent);
    connect(m_monopolyPopup, &MonopolyPopup::resourceChosen, this, &ActionManager::monopolyResourceChosen);
    return m_monopolyPopup;
}

YearOfPlentyPopup* ActionManager::ensureYearOfPlentyPopup() {
    if (m_yearOfPlentyPopup) return m_yearOfPlentyPopup;

    m_yearOfPlentyPopup = new YearOfPlentyPopup(m_bank, m_popupParent);
    connect(m_yearOfPlentyPopup, &YearOfPlentyPopup::yearOfPlentySubmitted, this, &ActionManager::yearOfPlentySubmitted);
    return m_yearOfPlentyPopup;
}
