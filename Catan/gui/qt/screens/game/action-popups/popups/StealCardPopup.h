#ifndef ROBPLAYERPOPUP_H
#define ROBPLAYERPOPUP_H

#include "../../../../common/panels/FloatingPanel.h"
#include <QVector>
#include <QString>
#include <common/cards/QCard.h>
#include <player/Player.h>

class QVBoxLayout;

class StealCardPopup : public FloatingPanel {
    Q_OBJECT
public:
    StealCardPopup(QWidget *parent);

    void setCandidates(const QVector<Player*>& cands); // we set each time, then it rebuilds
    void openAtGlobal(const QPoint& globalPos);   // show near mouse/hex
    void closePopup();

signals:
    void playerChosen(PlayerId id);

protected:
    void paintEvent(QPaintEvent*) override;

    void closeEvent(QCloseEvent *event) override;

private:
    void rebuild();

    QVector<Player*> m_cands;
    QVBoxLayout* m_root = nullptr;

};
#endif // ROBPLAYERPOPUP_H
