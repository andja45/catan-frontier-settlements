#ifndef GAMEJOINVIEW_H
#define GAMEJOINVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <host-join/RoleType.hpp>

class HostJoinView : public QWidget {
    Q_OBJECT

private:
    QLineEdit *m_gameNameEdit;
    QLineEdit *m_playerNameEdit;
    QPushButton *m_actionButton;
    QLabel *m_errorLabel;
    RoleType m_type;

public:
    explicit HostJoinView(RoleType type,QWidget *parent = nullptr);

public slots:
    void onRejected(const std::string &reason);
    void onAccepted();
public:
    void clearError();

signals:
    void requestSent(const QString &gameName, const QString &playerName);
};
#endif // GAMEJOINVIEW_H
