#ifndef FLOATINGPANEL_H
#define FLOATINGPANEL_H

#include <QFrame>

class FloatingPanel : public QFrame {
    Q_OBJECT
public:
    explicit FloatingPanel(QWidget* parent=nullptr) : QFrame(parent) {
        setFrameStyle(QFrame::NoFrame);
        setAttribute(Qt::WA_StyledBackground, true);
        setStyleSheet(R"(
            QFrame {
                background: rgba(245,245,245,230);
                border-radius: 10px;
            }
        )");
    }
};

#endif // FLOATINGPANEL_H
