#include "PlayerListItem.h"


PlayerListItem::PlayerListItem(const QString &playerName, QWidget *parent)
    : QWidget(parent), m_playerName(playerName)
{
    auto *nameLabel = new QLabel(playerName, this);
    auto *kickButton = new QPushButton("Kick", this);

    kickButton->setFixedWidth(50);

    auto *layout = new QHBoxLayout(this);
    layout->addWidget(nameLabel);
    layout->addStretch();
    layout->addWidget(kickButton);


    connect(kickButton, &QPushButton::clicked, this, [this]() {
        emit kickRequested(m_playerName);
    });
}
