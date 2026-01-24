#include "DevCardPopup.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QPen>
#include <QCursor>

#include <components/cards/QCardRow.h>
#include <components/cards/QCard.h>

DevCardPopup::DevCardPopup(QWidget* parent)
    : FloatingPanel(Qt::Popup, parent)
{
    setWindowFlag(Qt::FramelessWindowHint, true);
    setAttribute(Qt::WA_StyledBackground, false);     // we paint rounded bg ourselves
    setAttribute(Qt::WA_TranslucentBackground, true); // enables true rounded window corners

    // Keep stylesheet minimal; don't style QFrame globally.
    setStyleSheet(R"(
        QPushButton {
            padding: 6px 10px;
            border-radius: 8px;
            background: rgba(0,0,0,0);
        }
        QPushButton:hover {
            background: rgba(0,0,0,18);
        }
        QPushButton:pressed {
            background: rgba(0,0,0,32);
        }
    )");

    m_root = new QVBoxLayout(this);
    m_root->setContentsMargins(12, 12, 12, 12);
    m_root->setSpacing(10);

    m_title = new QLabel("Choose a development card to play", this);
    m_title->setStyleSheet("font-weight: 600;");
    m_root->addWidget(m_title);

    m_row = new QCardRow(this);
    m_root->addWidget(m_row);

    // Buttons row
    auto* btnRow = new QHBoxLayout();
    btnRow->addStretch(1);

    m_cancelBtn = new QPushButton("Cancel", this);
    connect(m_cancelBtn, &QPushButton::clicked, this, [this]() {
        emit cancelled();
        closePopup();
    });
    btnRow->addWidget(m_cancelBtn);

    m_root->addLayout(btnRow);

    adjustSize();
}

void DevCardPopup::setCards(const QVector<DevCardType>& cards) {
    m_cards = cards;
    m_selected = -1;
    rebuild();
}

void DevCardPopup::rebuild() {
    m_row->clear();
    m_cardWidgets.clear();

    // If no cards, show a friendly message and disable
    if (m_cards.isEmpty()) {
        m_title->setText("No development cards to play");
        adjustSize();
        return;
    } else {
        m_title->setText("Choose a development card to play");
    }

    m_cardWidgets.reserve(m_cards.size());

    for (int i = 0; i < m_cards.size(); ++i) {
        const DevCardType dt = m_cards[i];

        // Dev cards are visually undistinguished right now -> tint only.
        CardSpec spec;
        spec.kind = CardKind::Development;      // or FaceDown if you want hidden
        spec.resource = ResourceType::None;
        spec.dev = dt;                        // if CardSpec has it; otherwise ignore
        spec.countBadge = -1;

        QCard* card = m_row->addCard(spec);
        card->setCursor(Qt::PointingHandCursor);

        // Clicking a card chooses it immediately
        connect(card, &QCard::clicked, this, [this, i](Qt::MouseButton b) {
            if (b != Qt::LeftButton) return;
            selectIndex(i);
            closePopup();
            emit devCardChosen(m_cards[i]);
        });

        // Optional: allow hover to "preselect" visually (nice UX)
        connect(card, &QCard::hovered, this, [this, i](bool on) {
            if (!on) return;
            // show hover only; do not permanently select
            // (your QCard already highlights on hover)
        });

        m_cardWidgets.push_back(card);
    }

    adjustSize();
}

void DevCardPopup::selectIndex(int idx) {
    if (idx == m_selected) return;

    if (m_selected >= 0 && m_selected < m_cardWidgets.size())
        m_cardWidgets[m_selected]->setSelected(false);

    m_selected = idx;

    if (m_selected >= 0 && m_selected < m_cardWidgets.size())
        m_cardWidgets[m_selected]->setSelected(true);
}

void DevCardPopup::openAtGlobal(const QPoint& globalPos) {
    // show near point
    move(globalPos + QPoint(12, 12));
    show();
    raise();
    activateWindow();
}

void DevCardPopup::closePopup() {
    hide();
}

void DevCardPopup::paintEvent(QPaintEvent*) {
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
