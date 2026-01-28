#ifndef BOARDTOOLBAR_H
#define BOARDTOOLBAR_H

#include<QWidget>
#include<QHBoxLayout>
#include "components/panels/FloatingPanel.h"
#include<QPushButton>
#include<QButtonGroup>
#include <QMap>
#include<QMenu>
#include <QToolButton>
#include<QPainter>
#include <QKeyEvent>

#include "components/panels/CostPopup.h"
#include "../../game/types/ToolbarActionType.h"

#include <components/panels/TradeBankPopup.h>
#include <components/DiceWidget.h>
class BoardToolbar : public QWidget
{Q_OBJECT
private:
    FloatingPanel* m_buttonsPanel;
    CostPopup* m_costPopup;
    QLabel* m_countSettlements;
    QLabel* m_countRoads;
    QLabel* m_countCities;
    Player * m_player = new Player(1,"Petar"); //TODO fix
    TradeBankPopup* m_tradeBankPopup=nullptr;
    TradePopup* m_tradePopup = nullptr;
    QButtonGroup* m_buildGroup = nullptr;
    /*
    CountBadge* m_countSettlements;
    CountBadge* m_countRoads;
    CountBadge* m_countCities;*/
    QMap<ToolbarActionType, QPushButton*> m_buttons;
    QLabel* createBadge(int count, QWidget* parent);//backup cause countbadge is awful for performances
    FloatingPanel* createPanelWithButton(QWidget* button, ToolbarActionType action);
    QPushButton* createActionButton(const QString& text,ToolbarActionType action);
    QPushButton* createTradeButton(const QString& text,ToolbarActionType action);
    void paintEvent(QPaintEvent*) override;
    bool eventFilter(QObject* obj, QEvent* event) override;
    void addButton(const QString& text, ToolbarActionType action, QHBoxLayout* layout);
    void clearBuildSelection();
    void showTradePopup();
    void showBankTradePopup();
    QToolButton* createMenuButton(const QString& text, const QMap<QString, ToolbarActionType>& actions);

public:
    BoardToolbar(QWidget* parent=nullptr);
signals:
    void actionTriggered(ToolbarActionType action);
    void playerTradeRequested(const TradeOffer& offer);
    void bankTradeRequested(const TradeOffer& offer);
};
#endif // BOARDTOOLBAR_H
