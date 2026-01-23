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
#include "common/GameTheme.h"
#include "components/cards/CountBadge.h"
#include "../../game/types/ToolbarActionType.h"
#include"../../game/types/ResourceType.h"
#include "components/panels/CostPopup.h"
#include"../../../components/panels/TradePopup.h"
class BoardToolbar : public QWidget
{Q_OBJECT
private:
    FloatingPanel* m_buttonsPanel;
    CostPopup* m_costPopup;
    QLabel* m_countSettlements;
    QLabel* m_countRoads;
    QLabel* m_countCities;
    TradePopup* m_tradePopup = nullptr;
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
    bool eventFilter(QObject* obj, QEvent* event);
    void addButton(const QString& text, ToolbarActionType action, QHBoxLayout* layout);
    void showTradePopup();
    QToolButton* createMenuButton(const QString& text, const QMap<QString, ToolbarActionType>& actions);

public:
    BoardToolbar(QWidget* parent=nullptr);
signals:
    void actionTriggered(ToolbarActionType action);
    void playerTradeRequested(const TradeOffer& offer);
};
#endif // BOARDTOOLBAR_H
