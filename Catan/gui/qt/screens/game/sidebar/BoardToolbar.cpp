#include "BoardToolbar.h"

#include <model/MoveCosts.h>

BoardToolbar::BoardToolbar(QWidget* parent) : QWidget(parent) {

    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("background: transparent;");
    setAutoFillBackground(false);
    auto* buttonsLayout = new QHBoxLayout(this);
    buttonsLayout->sizeHint();
    buttonsLayout->setContentsMargins(10,10,10,10);
    buttonsLayout->setSpacing(10);
    m_costPopup = new CostPopup();
    m_tradePopup = new TradePopup();
    m_tradeBankPopup = new TradeBankPopup(m_player, nullptr);


    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMinimumHeight(60);/*
    for(ToolbarActionType action : ToolbarActionTypes){
        buttonsLayout->addWidget(
            createPanelWithButton(
                createActionButton(QString::fromStdString(nameFromToolbarActionType[action]), action)
                )
            );
    }*/

    buttonsLayout->addWidget(
        createPanelWithButton(
            createActionButton("Build Settle.", ToolbarActionType::BuildSettlement)
            , ToolbarActionType::BuildSettlement)
        );
    buttonsLayout->addWidget(
        createPanelWithButton(
            createActionButton("Build City", ToolbarActionType::BuildCity)
            , ToolbarActionType::BuildCity)
        );
    buttonsLayout->addWidget(
        createPanelWithButton(
            createActionButton("Build Road", ToolbarActionType::BuildRoad)
            ,ToolbarActionType::BuildRoad)
        );
    buttonsLayout->addWidget(
        createPanelWithButton(
            createActionButton("Buy Dev", ToolbarActionType::BuyDevCard)
            , ToolbarActionType::BuyDevCard)
        );
    buttonsLayout->addWidget(
        createPanelWithButton(
            createActionButton("Play Dev", ToolbarActionType::PlayDevCard)
            , ToolbarActionType::PlayDevCard)
        );


    buttonsLayout->addWidget(
        createPanelWithButton(
            createTradeButton("Trade Players", ToolbarActionType::PlayerTrade)
            , ToolbarActionType::PlayerTrade)
        );

    buttonsLayout->addWidget(
        createPanelWithButton(
            createTradeButton("Trade Bank", ToolbarActionType::BankTrade)
            , ToolbarActionType::BankTrade)
        );

    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(
        createPanelWithButton(
            createActionButton("Roll Dice", ToolbarActionType::RollDice)
            , ToolbarActionType::RollDice)
        );
    auto* diceEndBox = new QVBoxLayout();

    auto* diceWidget = new DiceWidget(this);
    diceEndBox->addWidget(diceWidget);

    diceEndBox->addWidget(
        createPanelWithButton(
            createActionButton("End Turn", ToolbarActionType::EndTurn)
            , ToolbarActionType::EndTurn)
        );

    buttonsLayout->addLayout(diceEndBox);
    diceWidget->setDice(3, 5);

    connect(m_tradePopup, &TradePopup::tradeSubmitted,
            this, &BoardToolbar::playerTradeRequested);
    connect(m_tradeBankPopup, &TradeBankPopup::tradeSubmitted,
            this, &BoardToolbar::bankTradeRequested);
/*
    buttonsLayout->addWidget(createMenuButton(
        "Build",
        {
            {"Road", ToolbarActionType::BuildRoad},
            {"Settlement", ToolbarActionType::BuildSettlement},
            {"City", ToolbarActionType::BuildCity}
        }
        ));
    addButton("Buy Dev", ToolbarActionType::BuyDevCard,buttonsLayout);
    addButton("Play Dev", ToolbarActionType::PlayDevCard,buttonsLayout);
    buttonsLayout->addWidget(createMenuButton(
        "Trade",
        {
            {"Player Trade", ToolbarActionType::PlayerTrade},
            {"Bank Trade", ToolbarActionType::BankTrade}
        }
        ));

    buttonsLayout->addStretch(1); // keeps Roll / End on the right

    addButton("Roll Dice", ToolbarActionType::BankTrade,buttonsLayout);
    addButton("End Turn", ToolbarActionType::BankTrade,buttonsLayout);*/
}

void BoardToolbar::showTradePopup()
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
        QPoint(button->width()/2 - m_tradeBankPopup->sizeHint().width()/2,
                -m_tradeBankPopup->sizeHint().height() - 8));

    m_tradeBankPopup->move(pos);
    m_tradeBankPopup->show();
    m_tradeBankPopup->raise();
    m_tradeBankPopup->activateWindow();

}
FloatingPanel* BoardToolbar::createPanelWithButton(QWidget* button, ToolbarActionType action) {
    auto* panel = new FloatingPanel(this);
    if(not MoveCosts::costFor(action).empty()){
        panel->setProperty("action", QVariant::fromValue(action));
        panel->setAttribute(Qt::WA_Hover);
        panel->installEventFilter(this);
    }
    auto* layout = new QHBoxLayout(panel);
    layout->setContentsMargins(8, 27, 8, 27);
    layout->setSpacing(0);

    button->setParent(panel);
    //button->installEventFilter(this);
    layout->addWidget(button);
    if (action == ToolbarActionType::BuildSettlement) {
        m_countSettlements = new CountBadge(5, panel);
        layout->addWidget(m_countSettlements);
    }
    else if (action == ToolbarActionType::BuildCity) {
        m_countCities = new CountBadge(4, panel);
        layout->addWidget(m_countCities);
    }
    else if (action == ToolbarActionType::BuildRoad) {
        m_countRoads = new CountBadge(15, panel);
        layout->addWidget(m_countRoads);
    }

    return panel;
}
QPushButton* BoardToolbar::createTradeButton(const QString& text,ToolbarActionType action) {
    auto* btn = new QPushButton(text);
    btn->setMinimumHeight(32);
    btn->setStyleSheet(R"(
    QPushButton {
        background: transparent;
        border: none;
        padding: 6px 14px;
        font-weight: 500;
    }

    QPushButton:hover {
        background: rgba(0, 0, 0, 18);
    }

    QPushButton:pressed {
        background: rgba(0, 0, 0, 35);
    }

    QPushButton:disabled {
        color: #999;
    }
    )");
    if(action == ToolbarActionType::PlayerTrade){
        connect(btn, &QPushButton::clicked,
            this, &BoardToolbar::showTradePopup);
    }
    else{
        connect(btn,&QPushButton::clicked,
                this,  &BoardToolbar::showBankTradePopup);
    }

    return btn;
}
QPushButton* BoardToolbar::createActionButton(const QString& text,ToolbarActionType action) {
    auto* btn = new QPushButton(text);
    btn->setMinimumHeight(32);
    btn->setStyleSheet(R"(
    QPushButton {
        background: transparent;
        border: none;
        padding: 6px 14px;
        font-weight: 500;
    }

    QPushButton:hover {
        background: rgba(0, 0, 0, 18);
    }

    QPushButton:pressed {
        background: rgba(0, 0, 0, 35);
    }

    QPushButton:disabled {
        color: #999;
    }
    )");

    connect(btn, &QPushButton::clicked, this, [this, action] {
        emit actionTriggered(action);
    });

    return btn;
}
bool BoardToolbar::eventFilter(QObject* obj, QEvent* event)
{
    qDebug() << "event" << event->type() << "on" << obj;
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
