#ifndef BOARDTOOLBAR_H
#define BOARDTOOLBAR_H

#include<QWidget>
#include<QHBoxLayout>
#include "common/panels/FloatingPanel.h"
#include<QPushButton>
#include<QButtonGroup>
#include <QMap>
#include<QMenu>
#include <QToolButton>
#include<QPainter>
#include <QKeyEvent>
#include <common/panels/FloatingButton.hpp>
#include <renderstate/ToolbarRenderState.h>
#include <screens/game/toolbar/tool-popups/ChooseDevCardPopup.h>

#include "common/panels/CostPopup.h"
#include "../../game/types/ToolbarActionType.h"

#include <screens/game/toolbar/tool-popups/RequestBankTradePopup.h>
#include "buttons/DiceWidget.h"

class BoardToolbar : public QWidget
{Q_OBJECT
private:
    CostPopup* m_costPopup;

    QCountBadge* m_countSettlements;
    QCountBadge* m_countRoads;
    QCountBadge* m_countCities;

    Player * m_player;

    RequestBankTradePopup* m_requestBankTradePopup=nullptr;
    RequestPlayerTradePopup* m_tradePopup = nullptr;
    ChooseDevCardPopup* m_chooseDevCardPopup = nullptr;

    QButtonGroup* m_buildGroup = nullptr;
    QMap<ToolbarActionType, FloatingButton*> m_buttons;

    FloatingButton *createButtonForType(ToolbarActionType action);

    void paintEvent(QPaintEvent*) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

    void clearBuildSelection();

    void showPlayerTradePopup();
    void showBankTradePopup();
    void showDevCardPopup();

    void disableAllButtons();
    void enableAllButtons();
    void disableButton(ToolbarActionType action);


public:
    BoardToolbar(Player* player,QWidget* parent=nullptr);
signals:
    void actionTriggered(ToolbarActionType action);

    void devCardChosen(DevCardType type);
    void playerTradeRequested(const TradeOffer& offer);
    void bankTradeRequested(const TradeOffer& offer);

    void diceRolled(int dice1,int dice2);
    void buyDevCardRequested();
    void endTurnRequested();
    void buildRoadClicked();
    void buildSettlementClicked();
    void buildCityClicked();

public slots:
    void updateState(ToolbarRenderState rs);
};
#endif // BOARDTOOLBAR_H
