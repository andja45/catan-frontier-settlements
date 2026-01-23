#ifndef DEVCARDPOPUP_H
#define DEVCARDPOPUP_H

#include "FloatingPanel.h"
#include <QVector>

class QLabel;
class QPushButton;
class QVBoxLayout;
class QCardRow;
class QCard;

#include <types/DevCardType.h>

class DevCardPopup : public FloatingPanel {
    Q_OBJECT
public:
    explicit DevCardPopup(QWidget* parent = nullptr);

    // Pass all dev cards the player currently holds (one entry per card)
    void setCards(const QVector<DevCardType>& cards);

    void openAtGlobal(const QPoint& globalPos);
    void closePopup();

signals:
    void devCardChosen(DevCardType type);
    void cancelled();

protected:
    void paintEvent(QPaintEvent*) override;

private:
    void rebuild();
    void selectIndex(int idx);

private:
    QVector<DevCardType> m_cards;

    QVBoxLayout* m_root = nullptr;
    QLabel*      m_title = nullptr;
    QCardRow*    m_row = nullptr;
    QPushButton* m_cancelBtn = nullptr;

    QVector<QCard*> m_cardWidgets;
    int m_selected = -1;
};

#endif // DEVCARDPOPUP_H
