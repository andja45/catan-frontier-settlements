#include "BoardToolbar.h"

BoardToolbar::BoardToolbar(QWidget* parent) : QWidget(parent) {
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("background: transparent;");
    setAutoFillBackground(false);
    auto* buttonsLayout = new QHBoxLayout(this);
    buttonsLayout->sizeHint();
    buttonsLayout->setContentsMargins(10,10,10,10);
    buttonsLayout->setSpacing(10);


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
            )
        );
    buttonsLayout->addWidget(
        createPanelWithButton(
            createActionButton("Build City", ToolbarActionType::BuildCity)
            )
        );
    buttonsLayout->addWidget(
        createPanelWithButton(
            createActionButton("Build Road", ToolbarActionType::BuildRoad)
            )
        );
    buttonsLayout->addWidget(
        createPanelWithButton(
            createActionButton("Buy Dev", ToolbarActionType::BuyDevCard)
            )
        );
    buttonsLayout->addWidget(
        createPanelWithButton(
            createActionButton("Play Dev", ToolbarActionType::PlayDevCard)
            )
        );
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(
        createPanelWithButton(
            createActionButton("Roll Dice", ToolbarActionType::RollDice)
            )
        );

    buttonsLayout->addWidget(
        createPanelWithButton(
            createActionButton("End Turn", ToolbarActionType::EndTurn)
            )
        );

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
void BoardToolbar::addButton(const QString& text, ToolbarActionType action, QHBoxLayout* layout)
{
    auto* button = new QPushButton(text, this);
    button->setMinimumHeight(36);

    connect(button, &QPushButton::clicked, this, [this, action]() {
        emit actionTriggered(action);
    });
    layout->addWidget(button);
}
FloatingPanel* BoardToolbar::createPanelWithButton(
    QWidget* button
    ) {
    auto* panel = new FloatingPanel(this);

    auto* layout = new QHBoxLayout(panel);
    layout->setContentsMargins(8, 6, 8, 6);
    layout->setSpacing(0);

    button->setParent(panel);
    layout->addWidget(button);

    return panel;
}
QPushButton* BoardToolbar::createActionButton(
    const QString& text,
    ToolbarActionType action
    ) {
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

QToolButton* BoardToolbar::createMenuButton(const QString& text,const QMap<QString, ToolbarActionType>& actions)
{
    auto* button = new QToolButton(this);
    button->setText(text);
    button->setMinimumHeight(36);
    button->setToolButtonStyle(Qt::ToolButtonTextOnly);

    auto* menu = new QMenu(button);
    menu->setStyleSheet(R"(
    QMenu {
        background-color: white;
        border: 1px solid #cfcfcf;
        border-radius: 8px;
        padding: 6px;
    }

    QMenu::item {
        padding: 10px 28px;
        margin: 2px;
        color: #222;
        border-radius: 6px;
    }

    QMenu::item:selected {
        background-color: #e6f0ff;
        color: #1a4fff;
    }

    QMenu::item:disabled {
        color: #9a9a9a;
        background-color: transparent;
    }
)");


    for (auto it = actions.begin(); it != actions.end(); ++it) {
        QAction* act = menu->addAction(it.key());
        connect(act, &QAction::triggered, this,
                [this, action = it.value()] {
                    emit actionTriggered(action);
                });
    }

    connect(button, &QToolButton::clicked, this, [button, menu]() {
        QPoint pos = button->mapToGlobal(QPoint(0, 0));
        pos.setY(pos.y() - menu->sizeHint().height());
        menu->exec(pos);
    });

    return button;
}
void BoardToolbar::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.fillRect(rect(), GameTheme::getColorByResource(ResourceType::Sea)); // sea
}
