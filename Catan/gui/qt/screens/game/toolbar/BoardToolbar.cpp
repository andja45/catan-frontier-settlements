#include "BoardToolbar.h"

#include "../../../common/theme/GameTheme.h"
#include "common/cards/QCountBadge.h"
#include"../../game/types/ResourceType.h"
#include "types/Costs.h"

BoardToolbar::BoardToolbar(Player* player,const std::pair<int,int>* dice,QWidget* parent) : QWidget(parent), m_player(player) {
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

    m_dice = new DiceWidget(dice,this);
    buttonsLayout->addWidget(m_dice);
    connect(m_dice, &DiceWidget::clicked, this, [this]() {

        emit diceRolled();
    });

    //m_buttons[ToolbarActionType::RollDice] = m_dice;

    auto btn=createButtonForType(ToolbarActionType::EndTurn);
    buttonsLayout->addWidget(btn);
    m_buttons[ToolbarActionType::EndTurn] = btn;

    if (auto* board = parent ? parent->findChild<QBoard*>() : nullptr) {
        connect(this, &BoardToolbar::onBuildCleared, board, &QBoard::onBuildCleared);
    }

}

void BoardToolbar::updateState(const ToolbarRenderState& rs) {
    if (!rs.isEnabled(ToolbarActionType::BuildRoad) &&
        !rs.isEnabled(ToolbarActionType::BuildSettlement) &&
        !rs.isEnabled(ToolbarActionType::BuildCity)) {
        clearBuildSelection();
    }

    for (auto& [action, button] : m_buttons.toStdMap()) {
        if (rs.isEnabled(action)) {
            button->setEnabled(true);
        } else {
            button->setEnabled(false);
        }

        const ResourcePack* cost = nullptr;
        switch (action) {
            case ToolbarActionType::BuildRoad:       cost = &Costs::Road; break;
            case ToolbarActionType::BuildSettlement: cost = &Costs::Settlement; break;
            case ToolbarActionType::BuildCity:       cost = &Costs::City; break;
            case ToolbarActionType::BuyDevCard:      cost = &Costs::DevCard; break;
            default: break;
        }

        if (cost && !m_player->hasResources(*cost))
            button->setEnabled(false);
    }
    if (rs.isEnabled(ToolbarActionType::RollDice)) {
        m_dice->setEnabled(true);
    }
    else {
        m_dice->setEnabled(false);
    }
// /
    m_countCities->setText(QString::number(m_player->getNumOfCitiesLeft()));
    m_countRoads->setText(QString::number(m_player->getNumOfRoadsLeft()));
    m_countSettlements->setText(QString::number(m_player->getNumOfSettlementsLeft()));

    QWidget::update();
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

    m_chooseDevCardPopup->rebuild();

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
    m_dice->setEnabled(false);

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

    const bool isBuildAction =
        action == ToolbarActionType::BuildRoad ||
        action == ToolbarActionType::BuildSettlement ||
        action == ToolbarActionType::BuildCity;

    btn->setCheckable(isBuildAction);
    btn->setChecked(false);
    if (isBuildAction && m_buildGroup) {
        m_buildGroup->addButton(btn);
    }

    QString text=nameFromToolbarActionType.at(action).c_str();
    QLabel* label = new QLabel(text, btn);
    label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    label->setStyleSheet("font-weight: 400; color: #2c3e50;");

    if (isBuildAction) {
        connect(btn, &QPushButton::toggled, this, [label](bool checked) {
            if (checked) label->setStyleSheet("font-weight: 700; color: rgb(80, 140, 200);");
            else         label->setStyleSheet("font-weight: 400; color: #2c3e50;");
        });
    }

    btn->addWidget(label);

    if (action == ToolbarActionType::BuildSettlement) {
        m_countSettlements = new QCountBadge(m_player->getNumOfSettlementsLeft(), btn);
        m_countSettlements->setColor(GameTheme::getPlayerColor(m_player->getPlayerId()));
        btn->addWidget(m_countSettlements);
    }
    else if (action == ToolbarActionType::BuildCity) {
        m_countCities = new QCountBadge(m_player->getNumOfCitiesLeft(), btn);
        m_countCities->setColor(GameTheme::getPlayerColor(m_player->getPlayerId()));
        btn->addWidget(m_countCities);
    }
    else if (action == ToolbarActionType::BuildRoad) {
        m_countRoads = new QCountBadge(m_player->getNumOfRoadsLeft(), btn);
        m_countRoads->setColor(GameTheme::getPlayerColor(m_player->getPlayerId()));
        btn->addWidget(m_countRoads);
    }

    const bool hasCost =
        action == ToolbarActionType::BuildRoad ||
        action == ToolbarActionType::BuildSettlement ||
        action == ToolbarActionType::BuildCity ||
        action == ToolbarActionType::BuyDevCard;

    if (hasCost) {
        btn->setProperty("action", QVariant::fromValue(action));
        btn->setAttribute(Qt::WA_Hover);
        btn->installEventFilter(this);
    }

    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    btn->setMinimumHeight(70);

    connect(btn, &QPushButton::clicked, this, [this, action, isBuildAction, label, btn]() {
        if (!isBuildAction) {
            clearBuildSelection();
            emit onBuildCleared();
        }

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
            btn->setChecked(true);
            emit buildRoadClicked();
        } else if (action==ToolbarActionType::BuildSettlement) {
            btn->setChecked(true);
            emit buildSettlementClicked();
        } else if (action==ToolbarActionType::BuildCity) {
            btn->setChecked(true);
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

void BoardToolbar::paintEvent(QPaintEvent* e) {
    QWidget::paintEvent(e);
}
