#include "BoardToolbar.h"

#include "../../../common/theme/GameTheme.h"
#include "common/cards/QCountBadge.h"
#include"../../game/types/ResourceType.h"
#include <model/MoveCosts.h>

BoardToolbar::BoardToolbar(Player* player,QWidget* parent) : QWidget(parent), m_player(player) {
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("background: transparent;");
    setAutoFillBackground(false);

    auto* buttonsLayout = new QHBoxLayout(this);
    buttonsLayout->setContentsMargins(10,10,10,10);
    buttonsLayout->setSpacing(10);

    m_costPopup = new CostPopup();
    m_tradePopup = new RequestPlayerTradePopup(m_player,this);
    m_requestBankTradePopup = new RequestBankTradePopup(m_player, this);
    m_chooseDevCardPopup = new ChooseDevCardPopup(m_player,this);

    connect(m_tradePopup, &RequestPlayerTradePopup::tradeSubmitted,
            this, &BoardToolbar::playerTradeRequested);
    connect(m_requestBankTradePopup, &RequestBankTradePopup::tradeSubmitted,
            this, &BoardToolbar::bankTradeRequested);
    connect(m_chooseDevCardPopup, &ChooseDevCardPopup::devCardChosen,this, &BoardToolbar::devCardChosen);

    m_buildGroup = new QButtonGroup(this);
    m_buildGroup->setExclusive(true);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumHeight(60);

    for (ToolbarActionType type : ToolbarActionTypes) {
        if (type==ToolbarActionType::EndTurn || type==ToolbarActionType::RollDice) continue;
        auto *btn = createButtonForType(type);
        m_buttons[type] = btn;
        buttonsLayout->addWidget(btn);
    }

    buttonsLayout->addStretch(1);

    auto* diceWidget = new DiceWidget(this);
    buttonsLayout->addWidget(diceWidget);

    buttonsLayout->addWidget(createButtonForType(ToolbarActionType::EndTurn));




    connect(m_tradePopup, &RequestPlayerTradePopup::tradeSubmitted,
            this, &BoardToolbar::playerTradeRequested);
    connect(m_requestBankTradePopup, &RequestBankTradePopup::tradeSubmitted,
            this, &BoardToolbar::bankTradeRequested);
    connect(m_chooseDevCardPopup, &ChooseDevCardPopup::devCardChosen,this, &BoardToolbar::devCardChosen);

}

void BoardToolbar::updateState(ToolbarRenderState rs) {
    // we unclick build buttons here, consider changing
    clearBuildSelection();
    for (auto& [action, button] : m_buttons.toStdMap()) {
        if (rs.isEnabled(action)) {
            button->setEnabled(true);
        } else {
            button->setEnabled(false);
        }
    }
}

void BoardToolbar::showPlayerTradePopup()
{
    auto* button = qobject_cast<QWidget*>(sender());
    if (!button) return;

    QPoint pos = button->mapToGlobal(
        QPoint(button->width()/2 - m_tradePopup->sizeHint().width()/2,
               -m_tradePopup->sizeHint().height() - 8));

    m_tradePopup->move(pos);
    m_tradePopup->show();
    m_tradePopup->raise();
}

void BoardToolbar::showBankTradePopup()
{
    auto* button = qobject_cast<QWidget*>(sender());
    if (!button) return;

    QPoint pos = button->mapToGlobal(
        QPoint(button->width()/2 - m_requestBankTradePopup->sizeHint().width()/2,
                -m_requestBankTradePopup->sizeHint().height() - 8));

    m_requestBankTradePopup->move(pos);
    m_requestBankTradePopup->show();
    m_requestBankTradePopup->raise();
    m_requestBankTradePopup->activateWindow();

}

void BoardToolbar::showDevCardPopup() {
    auto* button = qobject_cast<QWidget*>(sender());
    if (!button) return;

    QPoint pos = button->mapToGlobal(
        QPoint(button->width()/2 - m_chooseDevCardPopup->sizeHint().width()/2,
                - m_chooseDevCardPopup->sizeHint().height() - 8));

     m_chooseDevCardPopup->move(pos);
     m_chooseDevCardPopup->show();
     m_chooseDevCardPopup->raise();
     m_chooseDevCardPopup->activateWindow();
}

void BoardToolbar::disableAllButtons() {
    for (auto* btn : m_buttons) {
        btn->setEnabled(false);
    }
}

void BoardToolbar::enableAllButtons() {
    for (auto* btn : m_buttons) {
        btn->setEnabled(true);
    }
}

void BoardToolbar::disableButton(ToolbarActionType action) {
    m_buttons[action]->setEnabled(false);
}

FloatingButton *BoardToolbar::createButtonForType( ToolbarActionType action) {

    FloatingButton* btn = new FloatingButton(this);
    QString text=nameFromToolbarActionType.at(action).c_str();
    QLabel* label = new QLabel(text, btn);
    btn->addWidget(label);

    if (action == ToolbarActionType::BuildSettlement) {
        m_countSettlements = new QCountBadge(m_player->getNumOfSettlementsLeft(), btn);
        btn->addWidget(m_countSettlements);
    }
    else if (action == ToolbarActionType::BuildCity) {
        m_countCities = new QCountBadge(m_player->getNumOfCitiesLeft(), btn);
        btn->addWidget(m_countCities);
    }
    else if (action == ToolbarActionType::BuildRoad) {
        m_countRoads = new QCountBadge(m_player->getNumOfRoadsLeft(), btn);
        btn->addWidget(m_countRoads);
    }

    const bool isBuildAction =
        action == ToolbarActionType::BuildRoad ||
        action == ToolbarActionType::BuildCity ||
        action == ToolbarActionType::BuildSettlement;

    btn->setCheckable(isBuildAction);

    if (isBuildAction) {
        m_buildGroup->addButton(btn);
    }

    if(not MoveCosts::costFor(action).empty()){
        btn->setProperty("action", QVariant::fromValue(action));
        btn->setAttribute(Qt::WA_Hover);
        btn->installEventFilter(this);
    }

    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    btn->setMinimumHeight(70);



    connect(btn, &QPushButton::clicked, this, [this, action, isBuildAction]() {
        if (!isBuildAction)
            clearBuildSelection();

        if (action == ToolbarActionType::PlayerTrade) {
            showPlayerTradePopup();
        } else if (action == ToolbarActionType::BankTrade) {
            showBankTradePopup();
        } else if ( action == ToolbarActionType::PlayDevCard) {
            showDevCardPopup();
        } else if (action == ToolbarActionType::BuyDevCard) {
            emit buyDevCardRequested();
        } else if (action==ToolbarActionType::EndTurn){
            emit endTurnRequested();
        } else if (action==ToolbarActionType::BuildRoad) {
            emit buildRoadClicked();
        } else if (action==ToolbarActionType::BuildSettlement) {
            emit buildSettlementClicked();
        } else if (action==ToolbarActionType::BuildCity) {
            emit buildCityClicked();
        }

    });

    return btn;
}

void BoardToolbar::clearBuildSelection()
{
    m_buildGroup->setExclusive(false);
    for (auto* btn : m_buildGroup->buttons()) {
        btn->setChecked(false);
    }
    m_buildGroup->setExclusive(true);
}

bool BoardToolbar::eventFilter(QObject* obj, QEvent* event)
{
    auto* panel = qobject_cast<QWidget*>(obj);
    if (!panel)
        return QWidget::eventFilter(obj, event);

    if (!panel->property("action").isValid())
        return QWidget::eventFilter(obj, event);

    const auto action =
        panel->property("action").value<ToolbarActionType>();

    if (event->type() == QEvent::HoverEnter) {
        m_costPopup->adjustSize();
        m_costPopup->setAction(action);
        m_costPopup->refresh();


        const QPoint globalPos = panel->mapToGlobal(
            QPoint(panel->width() / 2 - m_costPopup->width() / 2,
                   -m_costPopup->height() - 8));

        m_costPopup->move(globalPos);
        m_costPopup->show();
        m_costPopup->adjustSize();
    }
    else if (event->type() == QEvent::HoverLeave) {
        m_costPopup->hide();
    }

    return QWidget::eventFilter(obj, event);
}

void BoardToolbar::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.fillRect(rect(), GameTheme::getColorByResource(ResourceType::Sea)); // sea
}
