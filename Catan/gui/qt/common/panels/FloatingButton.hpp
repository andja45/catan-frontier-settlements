#ifndef CATAN_FLOATINGBUTTON_HPP
#define CATAN_FLOATINGBUTTON_HPP

#include <QPushButton>
#include <QHBoxLayout>

class FloatingButton : public QPushButton {
    Q_OBJECT
public:
    explicit FloatingButton(QWidget* parent = nullptr);

    void addWidget(QWidget* w);

    QSize sizeHint() const;

    QSize minimumSizeHint() const;

    void mousePressEvent(QMouseEvent *event) override;

    void setMargin(int m) { m_contentLayout->setContentsMargins(m, m, m, m); }

private:
    QHBoxLayout* m_contentLayout = nullptr;
};

#endif // CATAN_FLOATINGBUTTON_HPP
