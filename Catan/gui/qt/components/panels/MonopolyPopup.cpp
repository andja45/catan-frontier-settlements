#include "MonopolyPopup.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QCursor>

#include <components/cards/QCardRow.h>
#include <components/cards/QCard.h>

MonopolyPopup::MonopolyPopup(QWidget* parent)
    : FloatingPanel(Qt::Popup, parent)
{
    setWindowFlag(Qt::FramelessWindowHint, true);
    setAttribute(Qt::WA_StyledBackground, false);     // we paint rounded bg ourselves
    setAttribute(Qt::WA_TranslucentBackground, true); // true rounded corners for top-level popup

    // Keep stylesheet minimal; don't style QFrame globally.
    setStyleSheet(R"(
        QPushButton { background: transparent; }
    )");

    m_root = new QVBoxLayout(this);
    m_root->setContentsMargins(12, 12, 12, 12);
    m_root->setSpacing(10);

    m_title = new QLabel("Choose a resource for Monopoly", this);
    m_title->setStyleSheet("font-weight: 600;");
    m_root->addWidget(m_title);

    m_row = new QCardRow(this);
    m_root->addWidget(m_row);

    // Default choices (5 standard resources)
    m_choices = {
        ResourceType::Wood,
        ResourceType::Brick,
        ResourceType::Wool,
        ResourceType::Wheat,
        ResourceType::Ore
    };

    rebuild();
    adjustSize();
}

void MonopolyPopup::setChoices(const QVector<ResourceType>& choices) {
    m_choices = choices;
    m_selected = -1;
    rebuild();
}

void MonopolyPopup::rebuild() {
    m_row->clear();
    m_cardWidgets.clear();

    if (m_choices.isEmpty()) {
        m_title->setText("No resources available");
        adjustSize();
        return;
    } else {
        m_title->setText("Choose a resource for Monopoly");
    }

    m_cardWidgets.reserve(m_choices.size());

    for (int i = 0; i < m_choices.size(); ++i) {
        const ResourceType r = m_choices[i];

        CardSpec spec;
        spec.kind = CardKind::Resource;
        spec.resource = r;
        spec.dev = DevCardType::None;
        spec.countBadge = -1;

        QCard* card = m_row->addCard(spec);
        card->setCursor(Qt::PointingHandCursor);

        // Choose immediately on click
        connect(card, &QCard::clicked, this, [this, i](Qt::MouseButton b) {
            if (b != Qt::LeftButton) return;
            selectIndex(i);
            emit resourceChosen(m_choices[i]);
            closePopup();
        });

        m_cardWidgets.push_back(card);
    }

    adjustSize();
}

void MonopolyPopup::selectIndex(int idx) {
    if (idx == m_selected) return;

    if (m_selected >= 0 && m_selected < m_cardWidgets.size())
        m_cardWidgets[m_selected]->setSelected(false);

    m_selected = idx;

    if (m_selected >= 0 && m_selected < m_cardWidgets.size())
        m_cardWidgets[m_selected]->setSelected(true);
}

void MonopolyPopup::openAtGlobal(const QPoint& globalPos) {
    move(globalPos + QPoint(12, 12));
    show();
    raise();
    activateWindow();
}

void MonopolyPopup::closePopup() {
    // reset transient visuals so next time starts clean
    for (auto* c : m_cardWidgets) {
        if (!c) continue;
        c->setSelected(false);
        c->clearHover();
    }
    m_selected = -1;

    hide();
}


void MonopolyPopup::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const qreal radius = 10.0;
    QRectF rr = rect();
    rr.adjust(0.5, 0.5, -0.5, -0.5);

    // Fill
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 255, 240));
    p.drawRoundedRect(rr, radius, radius);

    // Border
    p.setPen(QPen(QColor(0, 0, 0, 40), 1.0));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(rr, radius, radius);
}
