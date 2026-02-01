#ifndef DEVCARDPOPUP_H
#define DEVCARDPOPUP_H

#include "common/panels/FloatingPanel.h"
#include <map>
#include <player/Player.h>

#include <types/DevCardType.h>

class QLabel;
class QPushButton;
class QVBoxLayout;
class QCardRow;
class QCard;


class ChooseDevCardPopup : public FloatingPanel {
    Q_OBJECT
public:
    explicit ChooseDevCardPopup(Player* player,QWidget* parent = nullptr);

    void openAtGlobal(const QPoint& globalPos);
    void closePopup();

    void showEvent(QShowEvent *event);

signals:
    void devCardChosen(DevCardType type);
    void cancelled();

protected:
    void paintEvent(QPaintEvent*) override;

    void closeEvent(QCloseEvent *e);

private:
    void rebuild();
    void selectIndex(int idx);
    void reset();


    void updateUiState();

private:
    QVector<DevCardType> m_displayOrder;

    QVBoxLayout* m_root = nullptr;
    QLabel*      m_title = nullptr;
    QCardRow*    m_row = nullptr;
    QPushButton* m_cancelBtn = nullptr;

    QVector<QCard*> m_cardWidgets;
    int m_selected = -1;

    Player* m_player;
};

#endif // DEVCARDPOPUP_H
