#ifndef RIGHTOVERLAY_H
#define RIGHTOVERLAY_H

#include <QWidget>
#include <QVector>
#include <QString>

class FloatingPanel;
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

class RightOverlay : public QWidget {
    Q_OBJECT
public:
    explicit RightOverlay(QWidget* parent=nullptr);

    void addChatMessage(const QString& author, const QString& message);
    void setBankSummary(const BankSummary& b);
    void setPlayers(const QVector<PlayerSummary>& players);
    void updatePlayer(int row, const PlayerSummary& p);

signals:
    void chatSendRequested(const QString& text);

protected:
    void resizeEvent(QResizeEvent*) override { relayout(); }
    void paintEvent(QPaintEvent*) override;

private:
    void relayout();
    void buildChatUi(FloatingPanel* panel);
    void buildBankUi(FloatingPanel* panel);

private:
    int m_margin = 12;
    int m_rightWidth = 360;

    FloatingPanel* m_chat;
    FloatingPanel* m_bank;
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

    // --- optional players table (if you want it inside top panel) ---
    QTableWidget* m_playersTable = nullptr;
};
#endif // RIGHTOVERLAY_H
