#ifndef PLAYERLISTITEM_H
#define PLAYERLISTITEM_H

#include <QWidget>
#include <QString>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class PlayerListItem : public QWidget {
    Q_OBJECT

private:
    QString m_playerName;
public:
    explicit PlayerListItem(const QString &playerName, QWidget *parent = nullptr);

signals:
    void kickRequested(const QString &playerName);

};
#endif // PLAYERLISTITEM_H
