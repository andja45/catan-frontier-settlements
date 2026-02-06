#include "QCountBadge.h"

#include <common/theme/GameTheme.h>

void QCountBadge::setColor(QColor col) {
    m_color=col;
    col=col.darker(100);
    setStyleSheet(QString(R"(
        QLabel {
            background: (255,255,255,200);
            color: black;
            border: 3px solid %1;
            border-radius: 12px;
            font: bold 8pt;
        }
    )").arg(col.name(QColor::HexArgb)));
    refresh();
}

QCountBadge::QCountBadge(int count, QWidget* parent)
    : QLabel(QString::number(count), parent), m_count(count)
{
    setFixedSize(25, 25);
    setAlignment(Qt::AlignCenter);
    setColor(Qt::black);

    setAttribute(Qt::WA_TransparentForMouseEvents);
}
