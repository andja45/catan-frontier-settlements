//
// Created by marko on 1/26/2026.
//

#ifndef DISCARDPOPUP_H
#define DISCARDPOPUP_H

#include "FloatingPanel.h"

#include <map>
#include <vector>

class QVBoxLayout;
class QLabel;
class QPushButton;
class QCardRow;
class QCard;

#include <player/Player.h>

struct DiscardChoice {
    std::map<ResourceType,int> discard;
};

class DiscardPopup : public FloatingPanel {
    Q_OBJECT
public:
    explicit DiscardPopup(Player* player, QWidget* parent = nullptr);

    void updateHoldings();

    void openAtGlobal(const QPoint& globalPos);
    void closePopup();

    signals:
        void discardConfirmed(const DiscardChoice& choice);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    void rebuild();
    void updateUiState();

    int totalHeld() const;
    int requiredDiscard() const;
    int totalSelected() const;

private:
    Player* m_player = nullptr;

    std::map<ResourceType,int> m_holdings;   // current player holdings
    DiscardChoice m_choice;                  // how many to discard per type

    QVBoxLayout* m_layout = nullptr;
    QLabel* m_title = nullptr;
    QLabel* m_hint = nullptr;
    QCardRow* m_row = nullptr;
    QPushButton* m_confirmBtn = nullptr;

    std::vector<ResourceType> m_types;
    std::vector<QCard*> m_cards;
};

#endif //DISCARDPOPUP_H
