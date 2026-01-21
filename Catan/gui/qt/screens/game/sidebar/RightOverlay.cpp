#include "RightOverlay.h"
#include <components/panels/FloatingPanel.h>
#include <common/GameTheme.h>
#include <components/cards/QCardRow.h>

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

        PlayerUiRow uiRow;

        uiRow.resources = new QCard(p);
        uiRow.resources->setSpec({CardKind::Resource, CardFace::FaceDown, ResourceType::None, DevType::Unknown,
                                  m_players[i]->getNumOfResourceCards()});
        pl->addWidget(uiRow.resources);

        uiRow.devs = new QCard(p);
        uiRow.devs->setSpec({CardKind::Development, CardFace::FaceDown, ResourceType::None, DevType::Unknown,
                             m_players[i]->getNumOfDevCards()});
        pl->addWidget(uiRow.devs);

        uiRow.knights = new QLabel("Knights: 0", p);
        uiRow.roads   = new QLabel("Roads: 0", p);
        pl->addWidget(uiRow.knights);
        pl->addWidget(uiRow.roads);

        m_playerRows.push_back(uiRow);

        stackL->addWidget(p);
    }

    m_you = new FloatingPanel(this);
    m_you->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    buildYouUi(m_you);

    stackL->addWidget(m_you);
    stackL->addStretch(1);

    relayout();

    auto* t = new QTimer(this);
    connect(t, &QTimer::timeout, this, &RightOverlay::refreshAll);
    t->start(200);
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

    m_bankCards[0] = row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Wood,  DevType::Unknown, m_bank->getResources()[ResourceType::Wood]});
    m_bankCards[1] = row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Brick, DevType::Unknown, m_bank->getResources()[ResourceType::Brick]});
    m_bankCards[2] = row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Wool,  DevType::Unknown, m_bank->getResources()[ResourceType::Wool]});
    m_bankCards[3] = row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Wheat, DevType::Unknown, m_bank->getResources()[ResourceType::Wheat]});
    m_bankCards[4] = row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Ore,   DevType::Unknown, m_bank->getResources()[ResourceType::Ore]});

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

    m_youCards[0] = row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Wood,  DevType::Unknown, playerYou->getResources()[ResourceType::Wood]});
    m_youCards[1] = row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Brick, DevType::Unknown, playerYou->getResources()[ResourceType::Brick]});
    m_youCards[2] = row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Wool,  DevType::Unknown, playerYou->getResources()[ResourceType::Wool]});
    m_youCards[3] = row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Wheat, DevType::Unknown, playerYou->getResources()[ResourceType::Wheat]});
    m_youCards[4] = row->addCard({CardKind::Resource, CardFace::FaceUp, ResourceType::Ore,   DevType::Unknown, playerYou->getResources()[ResourceType::Ore]});


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

void RightOverlay::refreshAll() {
    if (!m_bank) return;

    // --- refresh bank cards ---
    if (m_bankCards[0]) m_bankCards[0]->setSpec({CardKind::Resource, CardFace::FaceUp, ResourceType::Wood,  DevType::Unknown, m_bank->getResources()[ResourceType::Wood]});
    if (m_bankCards[1]) m_bankCards[1]->setSpec({CardKind::Resource, CardFace::FaceUp, ResourceType::Brick, DevType::Unknown, m_bank->getResources()[ResourceType::Brick]});
    if (m_bankCards[2]) m_bankCards[2]->setSpec({CardKind::Resource, CardFace::FaceUp, ResourceType::Wool,  DevType::Unknown, m_bank->getResources()[ResourceType::Wool]});
    if (m_bankCards[3]) m_bankCards[3]->setSpec({CardKind::Resource, CardFace::FaceUp, ResourceType::Wheat, DevType::Unknown, m_bank->getResources()[ResourceType::Wheat]});
    if (m_bankCards[4]) m_bankCards[4]->setSpec({CardKind::Resource, CardFace::FaceUp, ResourceType::Ore,   DevType::Unknown, m_bank->getResources()[ResourceType::Ore]});

    // --- refresh per-player hidden counts (resource/dev hand sizes) ---
    const int n = std::min<int>(m_players.size(), m_playerRows.size());
    for (int i = 0; i < n; ++i) {
        Player* pl = m_players[i];
        if (!pl) continue;

        if (m_playerRows[i].resources) {
            m_playerRows[i].resources->setSpec({
                CardKind::Resource, CardFace::FaceDown,
                ResourceType::None, DevType::Unknown,
                pl->getNumOfResourceCards()
            });
        }

        if (m_playerRows[i].devs) {
            m_playerRows[i].devs->setSpec({
                CardKind::Development, CardFace::FaceDown,
                ResourceType::None, DevType::Unknown,
                pl->getNumOfDevCards()
            });
        }

        m_playerRows[i].knights->setText(QString("Knights: %1").arg(pl->getKnightsUsed()));
        m_playerRows[i].roads->setText(QString("Roads: %1").arg(pl->getRoads().size()));
    }

    // --- refresh "you" panel (currently: last player) ---
    if (!m_players.empty()) {
        Player* you = m_players.back();
        if (you) {
            if (m_youCards[0]) m_youCards[0]->setSpec({CardKind::Resource, CardFace::FaceUp, ResourceType::Wood,  DevType::Unknown, you->getResources()[ResourceType::Wood]});
            if (m_youCards[1]) m_youCards[1]->setSpec({CardKind::Resource, CardFace::FaceUp, ResourceType::Brick, DevType::Unknown, you->getResources()[ResourceType::Brick]});
            if (m_youCards[2]) m_youCards[2]->setSpec({CardKind::Resource, CardFace::FaceUp, ResourceType::Wool,  DevType::Unknown, you->getResources()[ResourceType::Wool]});
            if (m_youCards[3]) m_youCards[3]->setSpec({CardKind::Resource, CardFace::FaceUp, ResourceType::Wheat, DevType::Unknown, you->getResources()[ResourceType::Wheat]});
            if (m_youCards[4]) m_youCards[4]->setSpec({CardKind::Resource, CardFace::FaceUp, ResourceType::Ore,   DevType::Unknown, you->getResources()[ResourceType::Ore]});
        }
    }
}
