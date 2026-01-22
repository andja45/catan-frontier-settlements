#include "CountBadge.h"

CountBadge::CountBadge(int count, QWidget* parent)
    : QLabel(QString::number(count), parent)
{
    setFixedSize(22, 22);
    setAlignment(Qt::AlignCenter);
    setStyleSheet(R"(
        QLabel {
            background: rgba(255,255,255,220);
            color: black;
            border-radius: 11px;
            font: bold 8pt;
        }
    )");
    setAttribute(Qt::WA_TransparentForMouseEvents);
}
/*void CountBadge::paint(QPainter* painter, const QRectF& rect) const {
    if (!painter) return;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);

    // Draw background circle
    painter->setBrush(QColor(255, 255, 255, 220));
    painter->setPen(QColor(0, 0, 0, 120));
    painter->drawEllipse(rect);

    // Draw number
    painter->setPen(QColor(0, 0, 0, 220));
    QFont f = painter->font();
    f.setPointSize(8);
    f.setBold(true);
    painter->setFont(f);

    painter->drawText(rect, Qt::AlignCenter, QString::number(m_count));
    painter->restore();
}*/
/*CountBadge::CountBadge(int count, QWidget* parent)
    : QWidget(parent)
    , m_count(count)
{

    setFixedSize(22, 22);

    // Optional but nice
    setAttribute(Qt::WA_TransparentForMouseEvents);
}
void CountBadge::refresh(){
    update();
}
void CountBadge::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRectF b = rect().adjusted(1, 1, -1, -1);

    // Background circle
    p.setBrush(QColor(255, 255, 255, 220));
    p.setPen(QColor(0, 0, 0, 120));
    p.drawEllipse(b);

    // Number text
    p.setPen(QColor(0, 0, 0, 220));

    QFont f = font();
    f.setPointSize(8);
    f.setBold(true);
    p.setFont(f);

    p.drawText(b, Qt::AlignCenter, QString::number(m_count));
}
*/
