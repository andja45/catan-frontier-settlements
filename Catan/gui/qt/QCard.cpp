#include <QCard.h>
#include <GameTheme.h>
#include <QPainter>
#include <QMouseEvent>

QCard::QCard(QWidget* parent) : QWidget(parent) {
    setMouseTracking(true);
    setCursor(Qt::PointingHandCursor);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

QSize QCard::sizeHint() const { return QSize(44, 60); } // tweak later

void QCard::setSpec(const CardSpec& s) { m_spec = s; update(); }
void QCard::setSelected(bool on) { m_selected = on; update(); }

void QCard::enterEvent(QEnterEvent*) { m_hover = true; emit hovered(true); update(); }
void QCard::leaveEvent(QEvent*) { m_hover = false; emit hovered(false); update(); }

void QCard::mousePressEvent(QMouseEvent* e) {
    emit clicked(e->button());
    QWidget::mousePressEvent(e);
}

void QCard::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    QRectF r = rect().adjusted(1,1,-1,-1);
    const qreal radius = 8;

    // Background (face-down vs face-up)
    QColor base = QColor(245,245,245);
    if (m_spec.face == CardFace::FaceDown) base = QColor(80,120,160); // placeholder “back”

    // Resource/dev tint if face-up
    if (m_spec.face == CardFace::FaceUp) {
        if (m_spec.kind == CardKind::Resource) {
            if(m_spec.resource == ResourceType::None)
                base = QColor(153, 50, 204);
            else base = GameTheme::getColorByResource(m_spec.resource);
        } else {
            base = QColor(120, 90, 160); // dev card tint for now
        }
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
    if (m_hover || m_selected || m_spec.emphasized) {
        p.save();
        QPen glow(m_selected ? QColor(255,255,255,180) : QColor(255,255,255,120));
        glow.setWidthF(3.0);
        p.setPen(glow);
        p.drawRoundedRect(r.adjusted(1,1,-1,-1), radius, radius);
        p.restore();
    }

    // Simple icon/text for now (replace with SVG later)
    if (m_spec.face == CardFace::FaceUp) {
        p.save();
        p.setPen(QColor(0,0,0,200));
        QFont f = p.font();
        f.setBold(true);
        f.setPointSize(10);
        p.setFont(f);

        QString label;
        if (m_spec.kind == CardKind::Resource) {
            switch (m_spec.resource) {
            case ResourceType::Wood:  label = "W"; break;
            case ResourceType::Brick: label = "B"; break;
            case ResourceType::Wool:  label = "Wo"; break;
            case ResourceType::Wheat: label = "Wh"; break;
            case ResourceType::Ore:   label = "O"; break;
            default: label = "?"; break;
            }
        } else {
            label = "Dev";
        }

        p.drawText(r, Qt::AlignCenter, label);
        p.restore();
    } else {
        // face-down hint
        p.setPen(QColor(255,255,255,200));
        p.drawText(r, Qt::AlignCenter, "?");
    }

    // Count badge
    if (m_spec.countBadge > 0) {
        const int badge = m_spec.countBadge;
        QRectF b(r.right() - 18, r.top() + 4, 14, 14);
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
}
