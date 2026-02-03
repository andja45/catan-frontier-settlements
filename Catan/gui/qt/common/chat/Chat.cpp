//
// Created by matija on 1/30/26.
//

#include "Chat.hpp"

#include <QDateTime>
#include <QGroupBox>
#include <QLineEdit>
#include <QVBoxLayout>

#include <QRegularExpression>

#include "common/audio/AudioManager.h"
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
}

QSize Chat::sizeHint() const {
    return {300,0};
}

Chat::Chat(QWidget *parent) {
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    setMinimumHeight(100);
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
    QObject::connect(m_sendBtn, &QPushButton::clicked, this, sendNow);
    QObject::connect(m_chatInput, &QLineEdit::returnPressed, this, sendNow);
}

void Chat::addChatMessage(const QString& author, const QString& message) {
    if (!m_chatList) return;
    const QString time = QDateTime::currentDateTime().toString("HH:mm");
    auto emojified = emojify(message);
    m_chatList->addItem(QString("[%1] %2: %3").arg(time, author, emojified));
    m_chatList->scrollToBottom();
}
