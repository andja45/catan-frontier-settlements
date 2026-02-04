#ifndef PLAYERLISTITEM_H
#define PLAYERLISTITEM_H

#include <QWidget>
#include <QString>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "common/panels/FloatingPanel.h"
#include "common/player/PlayerDot.h"

class PlayerListItem : public FloatingPanel {
    Q_OBJECT

private:
    QString m_playerName;
public:
    explicit PlayerListItem(const QColor &color, const QString &playerName, QWidget *parent = nullptr);

signals:
    void kickRequested(const QString &playerName);

};
#endif // PLAYERLISTITEM_H
