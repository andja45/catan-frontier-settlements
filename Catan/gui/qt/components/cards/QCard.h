#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include"CountBadge.h"
#include "CardSpec.h"

class QCard : public QWidget {
    Q_OBJECT
public:
    explicit QCard(QWidget* parent=nullptr);

    void setSpec(const CardSpec& s);
    void incrementCount();
    void decrementCount();
    const CardSpec& spec() const { return m_spec; }

    void setSelected(bool on);
    bool isSelected() const { return m_selected; }

signals:
    void clicked(Qt::MouseButton button);
    void hovered(bool on);
    void leftClicked();
    void rightClicked();

protected:
    void paintEvent(QPaintEvent*) override;
    void enterEvent(QEnterEvent*) override;
    void leaveEvent(QEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    QSize sizeHint() const override;

private:
    CardSpec m_spec{};
    bool m_hover = false;
    bool m_selected = false;
    CountBadge* m_countBadge = NULL;
};

#endif // CARDWIDGET_H
