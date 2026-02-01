//
// Created by matija on 1/31/26.
//

#ifndef CATAN_ACTIONMANAGER_HPP
#define CATAN_ACTIONMANAGER_HPP

#include <QObject>
#include <QPoint>
#include <QVector>

#include <vector>

#include <player/Player.h>

struct DiscardChoice;
struct YearOfPlentyChoice;

class QWidget;

class Bank;

class DiscardPopup;
class StealCardPopup;
class MonopolyPopup;
class YearOfPlentyPopup;

class ActionManager : public QObject {
    Q_OBJECT
public:
    ActionManager(std::vector<Player *> players, PlayerId local, Bank *bank, QWidget *parent, QWidget *anchor);

public slots:
    void setStealCandidates(const std::vector<Player*>& candidates);

    void openActionPopup(DevCardType type);
    void openDiscardPopup();
    void openStealCardPopup(const std::vector<PlayerId> &candidates);
    void openMonopolyPopup();
    void openYearOfPlentyPopup();

    void closeAll();

    void setAnchorWidget(QWidget* anchor);

signals:
    void discardConfirmed(const DiscardChoice& choice);
    void playerChosenToSteal(PlayerId id);
    void monopolyResourceChosen(ResourceType r);
    void yearOfPlentySubmitted(const YearOfPlentyChoice& choice);

private:
    QPoint defaultGlobalPos() const;
    void hideAllExcept(QWidget* keep);

    DiscardPopup* ensureDiscardPopup();
    StealCardPopup* ensureStealCardPopup();
    MonopolyPopup* ensureMonopolyPopup();
    YearOfPlentyPopup* ensureYearOfPlentyPopup();

private:
    QWidget* m_popupParent = nullptr;
    QWidget* m_anchorWidget = nullptr;

    Player* m_player = nullptr;
    Bank* m_bank = nullptr;

    DiscardPopup* m_discardPopup = nullptr;
    StealCardPopup* m_stealPopup = nullptr;
    MonopolyPopup* m_monopolyPopup = nullptr;
    YearOfPlentyPopup* m_yearOfPlentyPopup = nullptr;
    PlayerId m_localId;
    std::vector<Player *> m_players;
};

#endif //CATAN_ACTIONMANAGER_HPP

