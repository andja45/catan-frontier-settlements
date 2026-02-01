//
// Created by matija on 1/30/26.
//

#ifndef CATAN_CHAT_HPP
#define CATAN_CHAT_HPP
#include <QListWidget>
#include <QPushButton>
#include <common/panels/FloatingPanel.h>
#include <screens/game/toolbar/buttons/DiceWidget.h>

class Chat : public FloatingPanel {
    Q_OBJECT

private:
    // --- chat ---
    QListWidget* m_chatList = nullptr;
    QLineEdit*   m_chatInput = nullptr;
    QPushButton* m_sendBtn = nullptr;
    QString emojify(QString text);

    QSize sizeHint() const override;
public:
    Chat(QWidget* parent = nullptr);

    void buildChatUi();

public slots:
    void addChatMessage(const QString& author, const QString& message);
signals:
    void chatSendRequested(const QString& text);

};

#endif //CATAN_CHAT_HPP