#include <common/cards/QCard.h>
#include <common/theme/GameTheme.h>
#include <QPainter>
#include <QMouseEvent>

static QString devLabel(DevCardType t) {
    switch (t) {
    case DevCardType::Knight:         return "K";
    case DevCardType::Monopoly:       return "MON";
    case DevCardType::RoadBuilding:   return "RB";
    case DevCardType::YearOfPlenty:   return "YOP";
    case DevCardType::VictoryPoint:   return "VP";
    default:                          return "";
    }
}

QCard::QCard(QWidget* parent) : QWidget(parent) {
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setMinimumHeight(22);
    setMinimumWidth(22);
    setMaximumHeight(58);
    m_flickerTimer.setInterval(20); // ~60 FPS
    connect(&m_flickerTimer, &QTimer::timeout, this, [this]() {
        m_flickerAlpha -= 0.05;
        if (m_flickerAlpha <= 0.0) {
            m_flickerAlpha = 0.0;
            m_flickerTimer.stop();
        }
        update();
    });
}
void QCard::flicker(const QColor& color)
{
    m_flickerColor = color;
    m_flickerAlpha = 0.8;   // initial intensity
    m_flickerTimer.start();
    update();
}
QSize QCard::sizeHint() const { return QSize(44, 58); } // tweak later

QSize QCard::minimumSizeHint() const { return QSize(20, 35); }

bool QCard::hasHeightForWidth() const {
    return true;
}

int QCard::heightForWidth(int w) const {
    return 66*w/44;
}

void QCard::setSpec(const CardSpec& s) { m_spec = s; update(); }
void QCard::setSelected(bool on) { m_selected = on; update(); }

void QCard::enterEvent(QEnterEvent*) { m_hover = true; emit hovered(true); update(); }
void QCard::leaveEvent(QEvent*) { m_hover = false; emit hovered(false); update(); }
void QCard::incrementCount() { m_spec.countBadge++; update(); }
void QCard::setCount(int count){m_spec.countBadge=count;update();}
void QCard::decrementCount() { if(m_spec.countBadge>0){m_spec.countBadge--; update();} }
void QCard::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        emit leftClicked();
    }
    else if (e->button() == Qt::RightButton) {
        emit rightClicked();
    }
    QWidget::mousePressEvent(e);
}

void QCard::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    QRectF r = rect().adjusted(1,1,-1,-1);
    const qreal radius = 8;

    // Background (face-down vs face-up)
    QColor base = QColor(245,245,245);

    // Resource/dev tint if face-up
    if (m_spec.kind == CardKind::Resource) {
        if(m_spec.resource == ResourceType::None)
            base = QColor(60, 70, 180);
        else base = GameTheme::getColorByResource(m_spec.resource);
    } else {
        base = QColor(120, 90, 160); // dev card tint for now
    }

    // Card body
    p.setPen(Qt::NoPen);
    p.setBrush(base);
    p.drawRoundedRect(r, radius, radius);

    // Border
    QPen border(QColor(0,0,0,80));
    border.setWidthF(1.5);
    p.setPen(border);
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(r, radius, radius);

    // Hover/selected glow
    if ((m_hover || m_selected || m_spec.emphasized)&&m_clickable) {
        p.save();
        QPen glow(m_selected ? QColor(255,255,255,180) : QColor(255,255,255,120));
        glow.setWidthF(3.0);
        p.setPen(glow);
        p.drawRoundedRect(r.adjusted(1,1,-1,-1), radius, radius);
        p.restore();
    }


    if (m_spec.kind == CardKind::Development) {
        const QString label = devLabel(m_spec.dev);

        // Font: compact, bold
        QFont f = p.font();
        f.setBold(true);
        f.setPointSize(9);
        p.setFont(f);

        // Text color
        QColor textColor(255, 255, 255, 230);

        const qreal topPadding = 6.0;
        const qreal labelHeight = 14.0;

        QRectF labelRect(
            r.left() + 2,
            r.top() + topPadding,
            r.width() - 4,
            labelHeight
            );

        // Optional subtle shadow for contrast
        p.setPen(QColor(0,0,0,120));
        p.drawText(labelRect.translated(1,1), Qt::AlignCenter, label);

        // Main text
        p.setPen(textColor);
        p.drawText(labelRect, Qt::AlignCenter, label);
    }
    //Flicker:
    if (m_flickerAlpha > 0.0) {
        QColor c = m_flickerColor;
        c.setAlphaF(m_flickerAlpha);

        p.setPen(Qt::NoPen);
        p.setBrush(c);
        p.drawRoundedRect(r, radius, radius);
    }
    if (m_spec.disabled) {
        QColor c = GameTheme::getColorByResource(ResourceType::None);
        p.setPen(Qt::NoPen);
        p.setBrush(c);
        p.drawRoundedRect(r, radius, radius);
    }
    // Count badge
        if (m_spec.countBadge >= 0) {
        const int badge = m_spec.countBadge;
        qreal number_radius = 25;
        QRectF b((r.right() + r.left()) / 2 - number_radius / 2, (r.top() + r.bottom()) / 2 - number_radius / 2,
                 number_radius, number_radius);
        p.setBrush(QColor(255,255,255,220));
        p.setPen(QColor(0,0,0,120));
        p.drawEllipse(b);

        p.setPen(QColor(0,0,0,220));
        QFont bf = p.font();
        bf.setPointSize(8);
        bf.setBold(true);
        p.setFont(bf);
        p.drawText(b, Qt::AlignCenter, QString::number(badge));
        }
        /*removed because of awful performances
        if (m_spec.countBadge >= 0) {
            m_countBadge = new CountBadge(m_spec.countBadge, this);
            m_countBadge->raise();   // VERY important
            m_countBadge->show();
        }
        */
}

