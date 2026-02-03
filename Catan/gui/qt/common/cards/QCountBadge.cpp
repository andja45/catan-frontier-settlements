#include "QCountBadge.h"

QCountBadge::QCountBadge(int count, QWidget* parent)
    : QLabel(QString::number(count), parent)
{
    setFixedSize(22, 22);
    setAlignment(Qt::AlignCenter);
    if (!m_golden) {
        setStyleSheet(R"(
        QLabel {
            background: rgba(255,255,255,220);
            color: black;
            border: 2px solid black;
            border-radius: 11px;
            font: bold 8pt;
        }
    )");
    }
    else {
        setStyleSheet(R"(
        QLabel {
            background: rgba(222,167,16,220);
            color: black;
            border: 2px solid black;
            border-radius: 11px;
            font: bold 8pt;
        }
    )");
    }


    setAttribute(Qt::WA_TransparentForMouseEvents);
}
