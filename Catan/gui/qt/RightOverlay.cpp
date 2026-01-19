#include "RightOverlay.h"
#include "FloatingPanel.h"
#include <GameTheme.h>
#include <QCard.h>
#include <QCardRow.h>

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
#include <QPainter>

RightOverlay::RightOverlay(QWidget* parent) : QWidget(parent) {
    // -------- Chat panel --------
    m_chat = new FloatingPanel(this);
    m_chat->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    buildChatUi(m_chat);

    // -------- Bank panel --------
    m_bank = new FloatingPanel(this);
    m_bank->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    buildBankUi(m_bank);

    // -------- Player islands (floating) --------
    m_playersStack = new QWidget(this);
    m_playersStack->setAttribute(Qt::WA_TransparentForMouseEvents, false);

    auto* stackL = new QVBoxLayout(m_playersStack);
    stackL->setContentsMargins(0,0,0,0);
    stackL->setSpacing(10);

    for (int i = 0; i < 4; ++i) {
        auto* p = new FloatingPanel(m_playersStack);
        p->setFixedHeight(80);

        auto* pl = new QHBoxLayout(p);
        pl->setContentsMargins(10,10,10,10);
        pl->addWidget(new QLabel(QString("Player %1").arg(i+1), p));
        pl->addStretch();

        auto resources = new QCard(p);
        resources->setSpec({CardKind::Resource, CardFace::FaceDown});
        pl->addWidget(resources);

        auto devs = new QCard(p);
        devs->setSpec({CardKind::Development, CardFace::FaceDown});
        pl->addWidget(devs);

        pl->addWidget(new QLabel("Knights: 0", p));
        pl->addWidget(new QLabel("Roads: 0", p));

        stackL->addWidget(p);
    }
    stackL->addStretch(1);

    relayout();
}

void RightOverlay::buildChatUi(FloatingPanel* panel) {
    auto* root = new QVBoxLayout(panel);
    root->setContentsMargins(10,10,10,10);
    root->setSpacing(10);

    auto* chatBox = new QGroupBox("Chat", panel);
    auto* chatLayout = new QVBoxLayout(chatBox);

    m_chatList = new QListWidget(chatBox);
    m_chatList->setWordWrap(true);
    m_chatList->setSelectionMode(QAbstractItemView::NoSelection);

    auto* inputRow = new QHBoxLayout();
    m_chatInput = new QLineEdit(chatBox);
    m_chatInput->setPlaceholderText("Send a message...");
    m_sendBtn = new QPushButton("Send", chatBox);
    m_sendBtn->setDefault(true);

    inputRow->addWidget(m_chatInput, 1);
    inputRow->addWidget(m_sendBtn);

    chatLayout->addWidget(m_chatList, 1);
    chatLayout->addLayout(inputRow);

    root->addWidget(chatBox, 1);

    auto sendNow = [this]() {
        const QString text = m_chatInput->text().trimmed();
        if (text.isEmpty()) return;
        emit chatSendRequested(text);
        m_chatInput->clear();
    };
    QObject::connect(m_sendBtn, &QPushButton::clicked, panel, sendNow);
    QObject::connect(m_chatInput, &QLineEdit::returnPressed, panel, sendNow);
}

void RightOverlay::buildBankUi(FloatingPanel* panel) {
    auto* root = new QVBoxLayout(panel);
    root->setContentsMargins(10,10,10,10);
    root->setSpacing(10);

    auto* bankBox = new QGroupBox("Bank", panel);

    // Layout INSIDE the group box so content respects the border/title
    auto* bankLayout = new QVBoxLayout(bankBox);
    bankLayout->setContentsMargins(10, 18, 10, 10); // top margin leaves room for title
    bankLayout->setSpacing(0);

    auto* row = new QCardRow(bankBox);
    row->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Wood,  DevType::Unknown, 19});
    row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Brick, DevType::Unknown, 19});
    row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Wool,  DevType::Unknown, 19});
    row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Wheat, DevType::Unknown, 19});
    row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Ore,   DevType::Unknown, 19});

    bankLayout->addWidget(row);

    root->addWidget(bankBox, 1);
}

void RightOverlay::addChatMessage(const QString& author, const QString& message) {
    if (!m_chatList) return;
    const QString time = QDateTime::currentDateTime().toString("HH:mm");
    m_chatList->addItem(QString("[%1] %2: %3").arg(time, author, message));
    m_chatList->scrollToBottom();
}

void RightOverlay::setBankSummary(const BankSummary& b) {
    if (!m_woodLbl) return;
    m_woodLbl->setText(QString("Wood: %1").arg(b.wood));
    m_brickLbl->setText(QString("Brick: %1").arg(b.brick));
    m_woolLbl->setText(QString("Wool: %1").arg(b.wool));
    m_wheatLbl->setText(QString("Wheat: %1").arg(b.wheat));
    m_oreLbl->setText(QString("Ore: %1").arg(b.ore));
}

void RightOverlay::setPlayers(const QVector<PlayerSummary>& players) {
    if (!m_playersTable) return; // only if you add the table to the top panel
    m_playersTable->setRowCount(players.size());
    for (int i = 0; i < players.size(); ++i) {
        const auto& p = players[i];
        m_playersTable->setItem(i, 0, new QTableWidgetItem(p.name));
        m_playersTable->setItem(i, 3, new QTableWidgetItem(QString::number(p.knights)));
        m_playersTable->setItem(i, 3, new QTableWidgetItem(QString::number(p.roads)));
    }
}

void RightOverlay::updatePlayer(int row, const PlayerSummary& p) {
    if (!m_playersTable) return;
    if (row < 0 || row >= m_playersTable->rowCount()) return;

    auto ensure = [&](int c) {
        if (!m_playersTable->item(row, c))
            m_playersTable->setItem(row, c, new QTableWidgetItem());
    };
    ensure(0); ensure(1); ensure(2); ensure(3);

    m_playersTable->item(row, 0)->setText(p.name);
    m_playersTable->item(row, 3)->setText(QString::number(p.knights));
    m_playersTable->item(row, 3)->setText(QString::number(p.roads));
}

void RightOverlay::relayout() {
    const int W = width();
    const int H = height();

    const int x = W - m_rightWidth - m_margin;
    const int y = m_margin;

    const int topAreaH = int(H * 0.45);

    // Bank gets ~30% of the top area, but never too small / too big
    const int bankH = std::clamp(int(topAreaH * 0.40), 120, 180);
    const int chatH = topAreaH - bankH - m_margin;

    m_chat->setGeometry(x, y, m_rightWidth, chatH);
    m_bank->setGeometry(x, y + chatH + m_margin, m_rightWidth, bankH);

    const int playersY = y + topAreaH + m_margin;
    m_playersStack->setGeometry(x, playersY, m_rightWidth, H - playersY - m_margin);
}

void RightOverlay::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.fillRect(rect(), GameTheme::getColorByResource(ResourceType::Sea)); // sea
}
