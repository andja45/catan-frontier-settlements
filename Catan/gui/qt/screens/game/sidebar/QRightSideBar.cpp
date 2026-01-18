//
// Created by marko on 1/14/2026.
//

#include "QRightSideBar.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QDateTime>

QRightSideBar::QRightSideBar(QWidget* parent)
    : QWidget(parent)
{
    buildUi();
}

void QRightSideBar::buildUi() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(8, 8, 8, 8);
    root->setSpacing(8);

    // -------- Chat --------
    auto* chatBox = new QGroupBox("Chat", this);
    auto* chatLayout = new QVBoxLayout(chatBox);

    m_chatList = new QListWidget(chatBox);
    m_chatList->setWordWrap(true);
    m_chatList->setSelectionMode(QAbstractItemView::NoSelection);

    auto* inputRow = new QHBoxLayout();
    m_chatInput = new QLineEdit(chatBox);
    m_chatInput->setPlaceholderText("Type a message...");
    m_sendBtn = new QPushButton("Send", chatBox);
    m_sendBtn->setDefault(true);

    inputRow->addWidget(m_chatInput, 1);
    inputRow->addWidget(m_sendBtn);

    chatLayout->addWidget(m_chatList, 1);
    chatLayout->addLayout(inputRow);

    root->addWidget(chatBox, 3);

    // Hook up send
    auto sendNow = [this]() {
        const QString text = m_chatInput->text().trimmed();
        if (text.isEmpty()) return;
        emit chatSendRequested(text);
        m_chatInput->clear();
    };
    QObject::connect(m_sendBtn, &QPushButton::clicked, this, sendNow);
    QObject::connect(m_chatInput, &QLineEdit::returnPressed, this, sendNow);

    // -------- Bank --------
    auto* bankBox = new QGroupBox("Bank", this);
    auto* bankLayout = new QHBoxLayout(bankBox);

    m_woodLbl  = new QLabel("Wood: 0", bankBox);
    m_brickLbl = new QLabel("Brick: 0", bankBox);
    m_woolLbl  = new QLabel("Wool: 0", bankBox);
    m_wheatLbl = new QLabel("Wheat: 0", bankBox);
    m_oreLbl   = new QLabel("Ore: 0", bankBox);

    bankLayout->addWidget(m_woodLbl);
    bankLayout->addWidget(m_brickLbl);
    bankLayout->addWidget(m_woolLbl);
    bankLayout->addWidget(m_wheatLbl);
    bankLayout->addWidget(m_oreLbl);
    bankLayout->addStretch(1);

    root->addWidget(bankBox, 0);

    // -------- Players --------
    auto* playersBox = new QGroupBox("Players", this);
    auto* playersLayout = new QVBoxLayout(playersBox);

    m_playersTable = new QTableWidget(playersBox);
    m_playersTable->setColumnCount(4);
    m_playersTable->setHorizontalHeaderLabels({"Name", "Resources", "Devs", "Knights"});
    m_playersTable->verticalHeader()->setVisible(false);
    m_playersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_playersTable->setSelectionMode(QAbstractItemView::NoSelection);
    m_playersTable->setFocusPolicy(Qt::NoFocus);
    m_playersTable->horizontalHeader()->setStretchLastSection(true);
    m_playersTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_playersTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_playersTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_playersTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    playersLayout->addWidget(m_playersTable);
    root->addWidget(playersBox, 2);

    setMinimumWidth(320);
    setMaximumWidth(420);
}

void QRightSideBar::addChatMessage(const QString& author, const QString& message) {
    const QString time = QDateTime::currentDateTime().toString("HH:mm");
    m_chatList->addItem(QString("[%1] %2: %3").arg(time, author, message));
    m_chatList->scrollToBottom();
}

void QRightSideBar::setBankSummary(const BankSummary& b) {
    m_woodLbl->setText(QString("Wood: %1").arg(b.wood));
    m_brickLbl->setText(QString("Brick: %1").arg(b.brick));
    m_woolLbl->setText(QString("Wool: %1").arg(b.wool));
    m_wheatLbl->setText(QString("Wheat: %1").arg(b.wheat));
    m_oreLbl->setText(QString("Ore: %1").arg(b.ore));
}

void QRightSideBar::setPlayers(const QVector<PlayerSummary>& players) {
    m_playersTable->setRowCount(players.size());

    for (int i = 0; i < players.size(); ++i) {
        const auto& p = players[i];

        m_playersTable->setItem(i, 0, new QTableWidgetItem(p.name));
        m_playersTable->setItem(i, 1, new QTableWidgetItem(QString::number(p.resources)));
        m_playersTable->setItem(i, 2, new QTableWidgetItem(QString::number(p.knights)));
        m_playersTable->setItem(i, 3, new QTableWidgetItem(QString::number(p.development)));
    }
}

void QRightSideBar::updatePlayer(int row, const PlayerSummary& p) {
    if (row < 0 || row >= m_playersTable->rowCount()) return;
    if (!m_playersTable->item(row, 0)) m_playersTable->setItem(row, 0, new QTableWidgetItem());
    if (!m_playersTable->item(row, 1)) m_playersTable->setItem(row, 1, new QTableWidgetItem());
    if (!m_playersTable->item(row, 2)) m_playersTable->setItem(row, 2, new QTableWidgetItem());

    m_playersTable->item(row, 0)->setText(p.name);
    m_playersTable->item(row, 1)->setText(QString::number(p.resources));
    m_playersTable->item(row, 2)->setText(QString::number(p.knights));
    m_playersTable->item(row, 2)->setText(QString::number(p.development));
}
