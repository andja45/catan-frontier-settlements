//
// Created by marko on 1/26/2026.
//

#ifndef YEAROFPLENTYPOPUP_H
#define YEAROFPLENTYPOPUP_H

#include "FloatingPanel.h"

#include <map>
#include <vector>

class QVBoxLayout;
class QPushButton;
class QLabel;
class QCardRow;
class QCard;

#include <types/ResourceType.h>   // adjust path if needed

struct YearOfPlentyChoice {
    std::map<ResourceType, int> receive; // exactly 2 total when valid
};

class Bank;   // forward declare if you have it
class Player; // optional

class YearOfPlentyPopup : public FloatingPanel {
    Q_OBJECT
public:
    explicit YearOfPlentyPopup(Bank* bank, QWidget* parent = nullptr);

    // optional: if bank amounts can change while open
    void setBankResources(const std::map<ResourceType,int>& bankResources);
    void openAtGlobal(const QPoint& globalPos);
    void closePopup();

    signals:
        void yearOfPlentySubmitted(const YearOfPlentyChoice& choice);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    void rebuild();
    void updateUiState();
    int totalSelected() const;

private:
    Bank* m_bank = nullptr; // can be nullptr if you only use setBankResources()

    std::map<ResourceType,int> m_bankResources;
    YearOfPlentyChoice m_choice;

    QVBoxLayout* m_layout = nullptr;
    QCardRow* m_row = nullptr;
    QPushButton* m_playBtn = nullptr;
    QLabel* m_hint = nullptr;

    std::vector<ResourceType> m_types;
    std::vector<QCard*> m_cards;
};


#endif //YEAROFPLENTYPOPUP_H
