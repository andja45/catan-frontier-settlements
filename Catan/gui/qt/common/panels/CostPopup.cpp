#include "CostPopup.h"

#include <QPainter>

#include "types/Costs.h"
#include<QVBoxLayout>
#include <QPainterPath>
#include <common/cards/CardSpec.h>
#include <common/cards/QCardRow.h>

CostPopup::CostPopup(QWidget* parent)
    : FloatingPanel(parent,Qt::ToolTip)
{
    setWindowFlag(Qt::FramelessWindowHint, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_StyledBackground, false); // we paint ourselves

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 8, 10, 8);

    m_row = new QCardRow(this);
    m_row->setFixedHeight(50);
    layout->addWidget(m_row);
    m_row->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
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

    const ResourcePack* pack = nullptr;
    switch (m_action) {
        case ToolbarActionType::BuildRoad:       pack = &Costs::Road; break;
        case ToolbarActionType::BuildSettlement: pack = &Costs::Settlement; break;
        case ToolbarActionType::BuildCity:       pack = &Costs::City; break;
        case ToolbarActionType::BuyDevCard:      pack = &Costs::DevCard; break;
        default: break;
    }

    if (pack) {
        for (auto const& [resource, count] : *pack) {
            for (int i = 0; i < count; ++i)
                m_row->addCard(CardSpec({CardKind::Resource, resource}));
        }
    }

    m_row->adjustSize();
    adjustSize();
}
