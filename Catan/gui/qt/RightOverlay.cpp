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
#include <iostream>

RightOverlay::RightOverlay(std::vector<Player*>& players, Bank* bank, QWidget* parent) : m_bank(bank), QWidget(parent) {
    // -------- Chat panel --------
    m_chat = new FloatingPanel(this);
    m_chat->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    buildChatUi(m_chat);

    // -------- Bank panel --------
    m_bankIsland = new FloatingPanel(this);
    m_bankIsland->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    buildBankUi(m_bankIsland);

    // -------- Player islands (floating) --------
    m_playersStack = new QWidget(this);
    m_playersStack->setAttribute(Qt::WA_TransparentForMouseEvents, false);

    auto* stackL = new QVBoxLayout(m_playersStack);
    stackL->setContentsMargins(0,0,0,0);
    stackL->setSpacing(10);

    stackL->addWidget(m_bankIsland);

    for(auto player : players) m_players.push_back(player);

    for (int i = 0; i < m_players.size(); ++i) {
        auto* p = new FloatingPanel(m_playersStack);
        p->setFixedHeight(80);

        auto* pl = new QHBoxLayout(p);
        pl->setContentsMargins(10,10,10,10);
        pl->addWidget(new QLabel(QString::fromStdString(m_players[i]->getName())));
        pl->addStretch();

        auto resources = new QCard(p);
        resources->setSpec({CardKind::Resource, CardFace::FaceDown, ResourceType::None,  DevType::Unknown, m_players[i]->getNumOfResourceCards()});
        pl->addWidget(resources);

        auto devs = new QCard(p);
        devs->setSpec({CardKind::Development, CardFace::FaceDown, ResourceType::None,  DevType::Unknown, m_players[i]->getNumOfDevCards()});
        pl->addWidget(devs);

        pl->addWidget(new QLabel("Knights: 0", p));
        pl->addWidget(new QLabel("Roads: 0", p));

        stackL->addWidget(p);
    }

    m_you = new FloatingPanel(this);
    m_you->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    buildYouUi(m_you);

    stackL->addWidget(m_you);
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
    bankLayout->setContentsMargins(3, 5, 3, 3); // top margin leaves room for title
    bankLayout->setSpacing(0);

    auto* row = new QCardRow(bankBox);
    row->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Wood,  DevType::Unknown, m_bank->getResources()[ResourceType::Wood]});
    row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Brick, DevType::Unknown, m_bank->getResources()[ResourceType::Brick]});
    row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Wool,  DevType::Unknown, m_bank->getResources()[ResourceType::Wool]});
    row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Wheat, DevType::Unknown, m_bank->getResources()[ResourceType::Wheat]});
    row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Ore,   DevType::Unknown, m_bank->getResources()[ResourceType::Ore]});

    bankLayout->addWidget(row);

    root->addWidget(bankBox, 1);
}

void RightOverlay::buildYouUi(FloatingPanel* panel) {
    auto* root = new QVBoxLayout(panel);
    root->setContentsMargins(10,10,10,10);
    root->setSpacing(10);

    auto* youBox = new QGroupBox("You", panel);

    // Layout INSIDE the group box so content respects the border/title
    auto* youLayout = new QVBoxLayout(youBox);
    youLayout->setContentsMargins(3, 5, 3, 3); // top margin leaves room for title
    youLayout->setSpacing(0);

    auto* row = new QCardRow(youBox);
    row->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    auto playerYou = m_players[m_players.size() - 1];

    row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Wood,  DevType::Unknown, playerYou->getResources()[ResourceType::Wood]});
    row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Brick, DevType::Unknown, playerYou->getResources()[ResourceType::Brick]});
    row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Wool,  DevType::Unknown, playerYou->getResources()[ResourceType::Wool]});
    row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Wheat, DevType::Unknown, playerYou->getResources()[ResourceType::Wheat]});
    row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Ore,   DevType::Unknown, playerYou->getResources()[ResourceType::Ore]});

    youLayout->addWidget(row);

    root->addWidget(youBox, 1);
}

void RightOverlay::addChatMessage(const QString& author, const QString& message) {
    if (!m_chatList) return;
    const QString time = QDateTime::currentDateTime().toString("HH:mm");
    m_chatList->addItem(QString("[%1] %2: %3").arg(time, author, message));
    m_chatList->scrollToBottom();
}

void RightOverlay::relayout() {
    const int W = width();
    const int H = height();

    const int x = W - m_rightWidth - m_margin;
    const int y = m_margin;

    // 30% of height for chat
    const int chatH = int(H * 0.3);

    m_chat->setGeometry(x, y, m_rightWidth, chatH);

    const int playersY = y + chatH + m_margin;
    m_playersStack->setGeometry(x, playersY, m_rightWidth, H - playersY - m_margin);
}

void RightOverlay::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.fillRect(rect(), GameTheme::getColorByResource(ResourceType::Sea)); // sea
}
