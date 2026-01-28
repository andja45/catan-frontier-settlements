#include "CostPopup.h"
#include "model/MoveCosts.h"

#include<QVBoxLayout>

#include <QPainterPath>
#include <QRegion>

CostPopup::CostPopup(QWidget* parent)
    : FloatingPanel(Qt::ToolTip, parent)
{
    setWindowFlag(Qt::FramelessWindowHint, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_StyledBackground, false); // we paint ourselves

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 8, 10, 8);

    m_row = new QCardRow(this);
    m_row->setFixedHeight(50);
    layout->addWidget(m_row);
}

void CostPopup::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const int r = 10;
    QRectF rr = rect();
    rr.adjust(0.5, 0.5, -0.5, -0.5);

    // Fill
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 255, 200));
    p.drawRoundedRect(rr, r, r);

    // Border
    p.setPen(QPen(QColor("#cccccc"), 1.0));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(rr, r, r);
}

void CostPopup::refresh()
{
    m_row->clear();
    for(ResourceType resource : MoveCosts::costFor(m_action))
        m_row->addCard(CardSpec({CardKind::Resource, resource}));
    m_row->adjustSize();
    adjustSize();
}

