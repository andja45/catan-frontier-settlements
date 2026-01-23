#ifndef PLAYERDOT_H
#define PLAYERDOT_H

#include <QWidget>
#include <QColor>

class PlayerDot : public QWidget {
public:
    explicit PlayerDot(QWidget* parent = nullptr) : QWidget(parent) {
        setFixedSize(24, 24);
    }
    void setColor(const QColor& c) { m_color = c; update(); }

protected:
    void paintEvent(QPaintEvent*) override;

private:
    QColor m_color = QColor(255, 140, 60); // placeholder
};

#endif // PLAYERDOT_H
