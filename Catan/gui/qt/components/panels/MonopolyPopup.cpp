#include "MonopolyPopup.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QCursor>

#include <components/cards/QCardRow.h>
#include <components/cards/QCard.h>

MonopolyPopup::MonopolyPopup(QWidget* parent)
    : FloatingPanel(Qt::Dialog, parent)
{
    setWindowModality(Qt::ApplicationModal);
    setWindowFlag(Qt::FramelessWindowHint, true);
    setAttribute(Qt::WA_StyledBackground, false);     // we paint rounded bg ourselves
    setAttribute(Qt::WA_TranslucentBackground, true); // true rounded corners for top-level popup

    // setStyleSheet(R"(
    //     QPushButton { background: transparent; }
    // )");
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


    m_root = new QVBoxLayout(this);
    m_root->setContentsMargins(12, 12, 12, 12);
    m_root->setSpacing(10);

    m_title = new QLabel("Choose a resource for Monopoly", this);
    m_title->setStyleSheet("font-weight: 600;");
    m_root->addWidget(m_title);

    m_row = new QCardRow(this);
    m_root->addWidget(m_row);

    // Buttons row
    auto* btnRow = new QHBoxLayout();
    btnRow->addStretch(1);

    m_cancelBtn = new QPushButton("Cancel", this);
    m_confirmBtn = new QPushButton("Confirm", this);
    m_confirmBtn->setEnabled(false);

    btnRow->addWidget(m_cancelBtn);
    btnRow->addWidget(m_confirmBtn);

    m_root->addLayout(btnRow);

    connect(m_cancelBtn, &QPushButton::clicked, this, [this]() {
        closePopup();
    });

    connect(m_confirmBtn, &QPushButton::clicked, this, [this]() {
        if (m_selected < 0 || m_selected >= m_choices.size()) return;
        emit resourceChosen(m_choices[m_selected]);
        closePopup();
    });

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

        connect(card, &QCard::leftClicked, this, [this, i]() {
            selectIndex(i);   // enable confirm
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

    if (m_confirmBtn)
        m_confirmBtn->setEnabled(m_selected >= 0);
}

void MonopolyPopup::openAtGlobal(const QPoint& globalPos) {
    move(globalPos + QPoint(12, 12));
    show();
    raise();
    activateWindow();
}

void MonopolyPopup::closePopup() {
    for (auto* c : m_cardWidgets) {
        if (!c) continue;
        c->setSelected(false);
        c->clearHover();
    }
    m_selected = -1;
    if (m_confirmBtn) m_confirmBtn->setEnabled(false);
    hide();
}


void MonopolyPopup::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.save();

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

    p.restore();
}
