#ifndef GAMEJOINVIEW_H
#define GAMEJOINVIEW_H

#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <RoleType.hpp>

class HostJoinView : public QDialog {
    Q_OBJECT

private:
    QLineEdit *m_gameNameEdit;
    QLineEdit *m_playerNameEdit;
    QPushButton *m_actionButton;
    QLabel *m_errorLabel;
    RoleType m_type;

    void closeEvent(QCloseEvent *event) override;
public:
    explicit HostJoinView(RoleType type,QWidget *parent = nullptr);

public slots:
    void onRejected(const std::string &reason);
public:
    void clearError();

signals:
    void requestSent(const QString &gameName, const QString &playerName);
    void closed();

};
#endif // GAMEJOINVIEW_H
