#include "AddBotListItem.h"

AddBotListItem::AddBotListItem(QWidget *parent)
    : QWidget(parent)
{
    m_difficultyBox = new QComboBox(this);
    m_difficultyBox->addItems({ "Easy", "Medium", "Hard" });

    m_addBotButton = new QPushButton("Add Bot", this);

    auto *layout = new QHBoxLayout(this);
    layout->addWidget(m_addBotButton);
    layout->addStretch();
    layout->addWidget(m_difficultyBox);

    connect(m_addBotButton, &QPushButton::clicked, this, [this]() {
        emit addBotRequested(m_difficultyBox->currentText());
    });
}
