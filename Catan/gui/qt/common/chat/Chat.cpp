//
// Created by matija on 1/30/26.
//

#include "Chat.hpp"

#include <QDateTime>
#include <QGroupBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QBrush>
#include <QColor>
#include <QFont>

#include <QRegularExpression>

#include "common/audio/AudioManager.h"
#include "common/theme/GameTheme.h"
#include "screens/game/action-popups/ActionManager.hpp"

QString Chat::emojify(QString text)
{
    struct Replace {
        const char* word;
        const char* emoji;
    };

    const Replace replacements[] = {
        {"wood",  "🌲"},
        {"wool",  "🐑"},
        {"brick", "🧱"},
        {"ore",   "🪨"},
        {"wheat", "🌾"}
    };

    for (const auto &r : replacements) {
        QRegularExpression re(
            QStringLiteral("\\b%1\\b").arg(r.word),
            QRegularExpression::CaseInsensitiveOption
        );
        text.replace(re, r.emoji);
    }
    return text;
}

QSize Chat::sizeHint() const {
    return {500,0};
}

Chat::Chat(QWidget *parent) {
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    setMinimumHeight(250);
    setMinimumWidth(150);
    buildChatUi();
}

void Chat::buildChatUi() {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(10,10,10,10);
    root->setSpacing(10);

    auto* chatBox = new QGroupBox("Chat", this);
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
        if (text.isEmpty()) {
        AudioManager::instance().playError();
            return;
        }
        AudioManager::instance().playClick();
        emit chatSendRequested(text);
        m_chatInput->clear();
    };
    connect(m_sendBtn, &QPushButton::clicked, this, sendNow);
    connect(m_chatInput, &QLineEdit::returnPressed, this, sendNow);
}

void Chat::addChatMessage(const QString& author, const QString& message) {
    addChatMessage(author, message, false);
}

void Chat::addChatMessage(const QString& author, const QString& message, bool isSystem) {
    if (!m_chatList) return;
    const QString time = QDateTime::currentDateTime().toString("HH:mm");
    const auto emojified = emojify(message);

    QString line;
    if (author.trimmed().isEmpty())
        line = QString("[%1] %2").arg(time, emojified);
    else
        line = QString("[%1] %2: %3").arg(time, author, emojified);

    auto* item = new QListWidgetItem(line);
    if (isSystem) {
        item->setForeground(QBrush(GameTheme::getColorByResource(ResourceType::Sea)));
        auto font = item->font();
        font.setBold(false);
        font.setWeight(QFont::DemiBold);
        item->setFont(font);
    }
    m_chatList->addItem(item);
    m_chatList->scrollToBottom();
}
