#ifndef RIGHTOVERLAY_H
#define RIGHTOVERLAY_H

#include <QWidget>
#include <QString>
#include <QTimer>

#include <components/cards/QCard.h>
#include <components/panels/RobPlayerPopup.h>

#include <player/Player.h>
#include <player/Bank.h>

class FloatingPanel;
class QListWidget;
class QLineEdit;
class QPushButton;
class QLabel;
class QTableWidget;

struct PlayerUiRow {
    QCard* resources = nullptr;   // face-down resource card with count badge
    QCard* devs      = nullptr;   // face-down dev card with count badge
    QLabel* knights  = nullptr;   // for now, still text
    QLabel* roads    = nullptr;
};

class RightOverlay : public QWidget {
    Q_OBJECT
public:
    explicit RightOverlay(std::vector<Player*>& players, Bank* bank, QWidget* parent=nullptr);

    void addChatMessage(const QString& author, const QString& message);
    void refreshAll();

signals:
    void chatSendRequested(const QString& text);

protected:
    void resizeEvent(QResizeEvent*) override { relayout(); }
    void paintEvent(QPaintEvent*) override;

private:
    void relayout();
    void buildChatUi(FloatingPanel* panel);
    void buildBankUi(FloatingPanel* panel);
    void buildYouUi(FloatingPanel* panel);

private:
    int m_margin = 12;
    int m_rightWidth = 360;

    FloatingPanel* m_chat;
    FloatingPanel* m_bankIsland;
    FloatingPanel* m_you;
    QWidget* m_playersStack = nullptr;     // islands

    // --- chat ---
    QListWidget* m_chatList = nullptr;
    QLineEdit*   m_chatInput = nullptr;
    QPushButton* m_sendBtn = nullptr;

    // --- bank ---
    QLabel* m_woodLbl  = nullptr;
    QLabel* m_brickLbl = nullptr;
    QLabel* m_woolLbl  = nullptr;
    QLabel* m_wheatLbl = nullptr;
    QLabel* m_oreLbl   = nullptr;
    Bank* m_bank;

    // --- players ---
    QTableWidget* m_playersTable = nullptr;
    QVector<Player*> m_players;
    Player* m_playerYou;

    // timer polling
    QTimer* m_refreshTimer = nullptr;

    // bank cards in fixed order
    std::array<QCard*, 5> m_bankCards {nullptr, nullptr, nullptr, nullptr, nullptr};

    // "you" cards in fixed order
    std::array<QCard*, 5> m_youCards {nullptr, nullptr, nullptr, nullptr, nullptr};

    // player rows
    std::vector<PlayerUiRow> m_playerRows;

    // for robber popup
    RobPlayerPopup* m_robPopup = nullptr;
};
#endif // RIGHTOVERLAY_H
