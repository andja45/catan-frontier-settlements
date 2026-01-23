#ifndef ROBPLAYERPOPUP_H
#define ROBPLAYERPOPUP_H

#include "FloatingPanel.h"
#include <QVector>
#include <QString>
#include <player/Player.h>

class QVBoxLayout;

class RobPlayerPopup : public FloatingPanel {
    Q_OBJECT
public:
    explicit RobPlayerPopup(QWidget* parent = nullptr);

    void setCandidates(const QVector<Player*>& cands);
    void openAtGlobal(const QPoint& globalPos);   // show near mouse/hex
    void closePopup();

signals:
    void playerChosen(PlayerId id);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    void rebuild();

    QVector<Player*> m_cands;
    QVBoxLayout* m_root = nullptr;
};
#endif // ROBPLAYERPOPUP_H
