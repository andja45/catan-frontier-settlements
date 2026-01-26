#ifndef MONOPOLYPOPUP_H
#define MONOPOLYPOPUP_H

#include "FloatingPanel.h"
#include <QVector>

class QLabel;
class QVBoxLayout;
class QCardRow;
class QCard;

#include <types/ResourceType.h>

class MonopolyPopup : public FloatingPanel {
    Q_OBJECT
public:
    explicit MonopolyPopup(QWidget* parent = nullptr);
    void setChoices(const QVector<ResourceType>& choices);

    void openAtGlobal(const QPoint& globalPos);
    void closePopup();

signals:
    void resourceChosen(ResourceType r);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    void rebuild();
    void selectIndex(int idx);

private:
    QVector<ResourceType> m_choices;

    QVBoxLayout* m_root = nullptr;
    QLabel*      m_title = nullptr;
    QCardRow*    m_row = nullptr;

    QVector<QCard*> m_cardWidgets;
    int m_selected = -1;
};
#endif // MONOPOLYPOPUP_H
