//
// Created by marko on 1/26/2026.
//

#include "DiscardPopup.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QPen>

#include <components/cards/QCardRow.h>
#include <components/cards/QCard.h>

static const std::vector<ResourceType> kResourceCardTypes = {
    ResourceType::Wood,
    ResourceType::Brick,
    ResourceType::Wool,
    ResourceType::Wheat,
    ResourceType::Ore
};

DiscardPopup::DiscardPopup(Player* player, QWidget* parent)
    : FloatingPanel(Qt::Dialog, parent), m_player(player)
{
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_StyledBackground, false);

    setStyleSheet(R"(
        QPushButton {
            padding: 6px 10px;
            border-radius: 8px;
            background: rgba(0,0,0,0);
        }
        QPushButton:hover { background: rgba(0,0,0,18); }
        QPushButton:pressed { background: rgba(0,0,0,32); }
        QPushButton:disabled { color: rgba(0,0,0,120); }
        QLabel#hint { color: rgba(0,0,0,160); }
    )");

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(12, 12, 12, 12);
    m_layout->setSpacing(8);

    m_title = new QLabel("Discard cards", this);
    m_title->setStyleSheet("font-weight: 600;");
    m_layout->addWidget(m_title);

    m_hint = new QLabel(this);
    m_hint->setObjectName("hint");
    m_layout->addWidget(m_hint);

    m_row = new QCardRow(this);
    m_layout->addWidget(m_row);

    m_confirmBtn = new QPushButton("Confirm", this);
    m_confirmBtn->setEnabled(false);
    m_layout->addWidget(m_confirmBtn);

    connect(m_confirmBtn, &QPushButton::clicked, this, [this]() {
        // only allow when exactly reached
        if (totalSelected() != requiredDiscard()) return;
        emit discardConfirmed(m_choice);
        closePopup();
    });

    m_types = kResourceCardTypes;
    for (auto rt : m_types) {
        m_holdings[rt] = 0;
        m_choice.discard[rt] = 0;
    }

    rebuild();
    updateUiState();
}

void DiscardPopup::updateHoldings() {
    m_holdings = m_player->getResources();
    for (auto rt : m_types) m_choice.discard[rt] = 0;
    updateUiState();
}


int DiscardPopup::totalHeld() const {
    int s = 0;
    for (auto& [rt, cnt] : m_holdings) s += cnt;
    return s;
}

int DiscardPopup::requiredDiscard() const {
    const int total = totalHeld();
    if (total <= 7) return 0;
    return total / 2; // floor half
}

int DiscardPopup::totalSelected() const {
    int s = 0;
    for (auto& [rt, cnt] : m_choice.discard) s += cnt;
    return s;
}

void DiscardPopup::updateUiState() {
    const int need = requiredDiscard();
    const int discarded = totalSelected();
    const int remainingToDiscard = std::max(0, need - discarded);

    if (need <= 0) {
        m_hint->setText("No discard required.");
        m_confirmBtn->setEnabled(true);
    } else {
        if (discarded < need) {
            m_hint->setText(QString("Discard %1 card(s). %2 remaining. (Left click discard, right click undo)")
                            .arg(need)
                            .arg(remainingToDiscard));
        } else if (discarded == need) {
            m_hint->setText("Ready. Click Confirm.");
        } else {
            m_hint->setText(QString("Too many selected (%1/%2). Undo with right click.")
                            .arg(discarded).arg(need));
        }

        m_confirmBtn->setEnabled(discarded == need);
    }

    // Show "remaining cards" on each resource card badge
    for (int i = 0; i < (int)m_cards.size(); ++i) {
        QCard* c = m_cards[i];
        const ResourceType rt = m_types[i];

        const int have = m_holdings.count(rt) ? m_holdings.at(rt) : 0;
        const int disc = m_choice.discard.count(rt) ? m_choice.discard.at(rt) : 0;
        const int remaining = std::max(0, have - disc);

        CardSpec s = c->spec();
        s.countBadge = remaining;
        c->setSpec(s);
    }

    adjustSize();
}


void DiscardPopup::rebuild() {
    m_row->clear();
    m_cards.clear();
    m_cards.reserve(m_types.size());

    for (auto rt : m_types) {
        CardSpec spec;
        spec.kind = CardKind::Resource;
        spec.resource = rt;
        spec.dev = DevCardType::None;
        spec.countBadge = 0; // will be overwritten by updateUiState()

        QCard* card = m_row->addCard(spec);

        // Left click = discard one (decrease remaining)
        connect(card, &QCard::leftClicked, this, [this, rt]() {
            const int need = requiredDiscard();
            if (need <= 0) return;

            const int discarded = totalSelected();
            if (discarded >= need) return; // already discarded enough

            const int have = m_holdings.count(rt) ? m_holdings.at(rt) : 0;
            const int disc = m_choice.discard[rt];
            if (disc >= have) return; // can't discard more than you have

            m_choice.discard[rt] = disc + 1;
            updateUiState();
        });

        // Right click = undo discard (increase remaining)
        connect(card, &QCard::rightClicked, this, [this, rt]() {
            const int disc = m_choice.discard[rt];
            if (disc <= 0) return;
            m_choice.discard[rt] = disc - 1;
            updateUiState();
        });

        m_cards.push_back(card);
    }

    updateUiState();
}


void DiscardPopup::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const qreal radius = 12.0;
    QRectF rr = rect();
    rr.adjust(0.5, 0.5, -0.5, -0.5);

    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 255, 240));
    p.drawRoundedRect(rr, radius, radius);

    p.setPen(QPen(QColor(0,0,0,40), 1.0));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(rr, radius, radius);
}

void DiscardPopup::openAtGlobal(const QPoint& globalPos) {
    updateHoldings();

    move(globalPos + QPoint(12, 12));
    show();
    raise();
    activateWindow();
}

void DiscardPopup::closePopup() {
    hide();
}