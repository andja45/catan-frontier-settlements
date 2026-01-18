#ifndef GAMEJOINVIEW_H
#define GAMEJOINVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class GameJoinView : public QWidget {
    Q_OBJECT

private:
    QLineEdit *m_gameNameEdit;
    QLineEdit *m_playerNameEdit;
    QPushButton *m_joinButton;
    QLabel *m_errorLabel;

public:
    explicit GameJoinView(QWidget *parent = nullptr);

public slots:
    void onJoinRejected(const std::string &reason);
    void clearError();

signals:
    void joinGameRequested(const QString &gameName, const QString &playerName);
};
#endif // GAMEJOINVIEW_H
