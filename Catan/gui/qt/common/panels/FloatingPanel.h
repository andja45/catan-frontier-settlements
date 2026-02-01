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
};

#endif // FLOATINGPANEL_H
