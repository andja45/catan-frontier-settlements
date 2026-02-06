#include "ChooseDevCardPopup.h"

#include "../../../../common/audio/AudioManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QPen>
#include <QCursor>

#include <common/cards/QCardRow.h>
#include <common/cards/QCard.h>

#include <iostream>
#include <qevent.h>
#include <screens/game/toolbar/tool-popups/ChooseDevCardPopup.h>

ChooseDevCardPopup::ChooseDevCardPopup(Player* player,QWidget* parent)
    : FloatingPanel( parent,Qt::Popup), m_player(player)
{
    setWindowFlag(Qt::FramelessWindowHint, true);
    setAttribute(Qt::WA_StyledBackground, false);     // we paint rounded bg ourselves
    setAttribute(Qt::WA_TranslucentBackground, true); // enables true rounded window corners

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
        AudioManager::instance().playClick();

        emit cancelled();
        closePopup();
    });
    btnRow->addWidget(m_cancelBtn);

    m_root->addLayout(btnRow);

    rebuild();
    adjustSize();
}


void ChooseDevCardPopup::rebuild() {
    m_selected = -1;
    reset();
    m_displayOrder.clear();

    for (const auto& [type, count] : m_player->getDevCards()) {
        if (count <= 0) continue;
        if(type == DevCardType::VictoryPoint) continue;
        m_displayOrder.push_back(type);
    }

    m_row->clear();
    m_cardWidgets.clear();

    if (m_displayOrder.isEmpty()) {
        m_title->setText("No development cards to play");
        adjustSize();
        return;
    }
    m_title->setText("Choose a development card to play");

    m_cardWidgets.reserve(m_displayOrder.size());

    for (int i = 0; i < m_displayOrder.size(); ++i) {
        const DevCardType dt = m_displayOrder[i];
        const int count = m_player->getDevCards()[dt];
        CardSpec spec;
        spec.kind = CardKind::Development;
        spec.resource = ResourceType::None;
        spec.dev = dt;
        spec.countBadge = count;

        QCard* card = m_row->addCard(spec);
        card->setCursor(Qt::PointingHandCursor);

        connect(card, &QCard::leftClicked, this, [this, i]() {
            if (m_player->getDevCards()[m_displayOrder[i]]<=0) // we wont react if count 0
                return;
            selectIndex(i);
            emit devCardChosen(m_displayOrder[i]);
            closePopup();
        });

        m_cardWidgets.push_back(card);
    }
    updateUiState();
    adjustSize();
}

void ChooseDevCardPopup::selectIndex(int idx) {
    if (idx == m_selected) return;

    if (m_selected >= 0 && m_selected < m_cardWidgets.size())
        m_cardWidgets[m_selected]->setSelected(false);

    m_selected = idx;

    if (m_selected >= 0 && m_selected < m_cardWidgets.size())
        m_cardWidgets[m_selected]->setSelected(true);
}

void ChooseDevCardPopup::reset() {
    for (auto* c : m_cardWidgets) {
        if (!c) continue;
        c->setSelected(false);
        c->clearHover();
    }
    m_selected = -1;
}


void ChooseDevCardPopup::updateUiState() {
    for (int i = 0; i < m_displayOrder.size(); ++i) {
        const DevCardType dt = m_displayOrder[i];
        auto c=m_cardWidgets[i];
        const int count = m_player->getDevCards()[dt];
        CardSpec spec;
        spec.kind = CardKind::Development;
        spec.resource = ResourceType::None;
        spec.dev = dt;
        spec.countBadge = count;
        c->setSpec(spec);
    }
}

void ChooseDevCardPopup::openAtGlobal(const QPoint& globalPos) {
    // show near point
    move(globalPos - QPoint(180, 180));
    show();
    raise();
    activateWindow();
}

void ChooseDevCardPopup::closePopup() {
    reset();
    hide();
}

void ChooseDevCardPopup::paintEvent(QPaintEvent*) {
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

void ChooseDevCardPopup::closeEvent(QCloseEvent *e) {
    e->ignore();
    closePopup();
}

void ChooseDevCardPopup::showEvent(QShowEvent* event)
{
    updateUiState();
}
