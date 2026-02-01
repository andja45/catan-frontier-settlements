#ifndef FLOATINGPANEL_H
#define FLOATINGPANEL_H

#include <QFrame>

class FloatingPanel : public QFrame {
    Q_OBJECT
public:
    explicit FloatingPanel(QWidget* parent=nullptr,Qt::WindowType type={}) : QFrame(parent,type) {
        setFrameStyle(QFrame::NoFrame);
        setAttribute(Qt::WA_StyledBackground, true);
        setStyleSheet(R"(
            FloatingPanel {
                background: rgba(245,245,245,230);
                border-radius: 10px;
            }
        )");
        setMouseTracking(true);
    }

    void setActiveHighlight(bool on) { m_activeHighlight = on; update(); }

private:
    bool m_activeHighlight = false;
    bool activeHighlight() const { return m_activeHighlight; }
    void paintEvent(QPaintEvent* e) override;

};

#endif // FLOATINGPANEL_H
