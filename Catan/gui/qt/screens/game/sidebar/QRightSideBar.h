//
// Created by marko on 1/14/2026.
//

#ifndef QRIGHTSIDEBAR_H
#define QRIGHTSIDEBAR_H

#include <QWidget>
#include <QVector>
#include <QString>

class QListWidget;
class QLineEdit;
class QPushButton;
class QLabel;
class QTableWidget;

struct PlayerSummary {
    QString name;
    int resources = 0;
    int development = 0;
    int knights = 0;
};

struct BankSummary {
    int wood = 19;
    int brick = 19;
    int wool = 19;
    int wheat = 19;
    int ore  = 19;
};

class QRightSideBar : public QWidget {
    Q_OBJECT
public:
    explicit QRightSideBar(QWidget* parent = nullptr);

    // --- chat ---
    void addChatMessage(const QString& author, const QString& message);

    // --- bank ---
    void setBankSummary(const BankSummary& b);

    // --- players ---
    void setPlayers(const QVector<PlayerSummary>& players);
    void updatePlayer(int row, const PlayerSummary& p); // convenience

    signals:
        void chatSendRequested(const QString& text);

private:
    void buildUi();

    // Chat
    QListWidget* m_chatList = nullptr;
    QLineEdit*   m_chatInput = nullptr;
    QPushButton* m_sendBtn = nullptr;

    // Bank labels
    QLabel* m_woodLbl  = nullptr;
    QLabel* m_brickLbl = nullptr;
    QLabel* m_woolLbl  = nullptr;
    QLabel* m_wheatLbl = nullptr;
    QLabel* m_oreLbl   = nullptr;

    // Player table
    QTableWidget* m_playersTable = nullptr;
};



#endif //QRIGHTSIDEBAR_H
