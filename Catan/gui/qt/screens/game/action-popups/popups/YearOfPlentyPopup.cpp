//
// Created by marko on 1/26/2026.
//

#include "YearOfPlentyPopup.h"

#include <qevent.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QPen>

#include <common/cards/QCardRow.h>
#include <common/cards/QCard.h>
#include <player/Bank.h>

#include "common/audio/AudioManager.h"


YearOfPlentyPopup::YearOfPlentyPopup(Bank* bank, QWidget* parent)
    : FloatingPanel( parent,Qt::Dialog), m_bank(bank)
{
    // True rounded popup window
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
        QLabel#hint { color: rgba(0,0,0,160); }
    )");

    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(12, 12, 12, 12);
    m_layout->setSpacing(8);

    auto* title = new QLabel("Year of Plenty", this);
    title->setStyleSheet("font-weight: 600;");
    m_layout->addWidget(title);

    m_hint = new QLabel(this);
    m_hint->setObjectName("hint");
    m_layout->addWidget(m_hint);

    m_row = new QCardRow(this);
    m_layout->addWidget(m_row);

    m_playBtn = new QPushButton("Play", this);
    m_playBtn->setEnabled(false);
    m_layout->addWidget(m_playBtn);

    connect(m_playBtn, &QPushButton::clicked, this, [this]() {
        // valid only when exactly 2 selected
        AudioManager::instance().playClick();
        ResourcePack rp;
        for (auto [rt, cnt] : m_choice.receive) {
            if (cnt!=0) {
                rp[rt] = cnt;
            }
        }
        emit yearOfPlentySubmitted({rp});
        closePopup();
    });


    rebuild();
    updateUiState();
}


int YearOfPlentyPopup::totalSelected() const {
    int s = 0;
    for (auto& [rt, cnt] : m_choice.receive) s += cnt;
    return s;
}

void YearOfPlentyPopup::updateUiState() {
    const int sel = totalSelected();
    m_playBtn->setEnabled(sel == 2 || sel == 1 && m_bank->getNumOfResourceCards()==1);

    if (m_bank->getNumOfResourceCards()<=0) {
        m_hint->setText(QString("No resources to choose"));
        m_playBtn->setEnabled(true);
    }
    else{
        if (sel < 2 && m_bank->getNumOfResourceCards()>1) m_hint->setText(QString("Pick %1 more card(s). (Left/Right click)").arg(2 - sel));
        else if (sel == 2||sel==1&& m_bank->getNumOfResourceCards()==1) m_hint->setText("Ready. Click Play.");
        else m_hint->setText("Too many selected (should not happen).");
    }
    // Update badge visuals (in case bank resources changed)
    for (int i = 0; i < (int)m_cards.size(); ++i) {
        QCard* c = m_cards[i];
        const ResourceType rt = m_types[i];

        // Keep the card spec badge synced with selection
        CardSpec s = c->spec();
        s.countBadge = m_choice.receive[rt];

        // “disable look” when bank is empty
        const bool bankHasAny = (m_bankResources.count(rt) ? m_bankResources.at(rt) : 0) > 0;
        if (!bankHasAny)
            s.disabled=true;
        c->setSpec(s);

        (void)bankHasAny;
    }
}

void YearOfPlentyPopup::rebuild() {
    m_bankResources= m_bank->getResources();
    // init data
    for (auto [rt,_] : m_bank->getResources()) {
        m_types.push_back(rt);
        m_choice.receive[rt] = 0;
    }

    m_row->clear();
    m_cards.clear();
    m_cards.reserve(m_types.size());

    for (auto rt : m_types) {
        CardSpec spec;
        spec.kind = CardKind::Resource;
        spec.resource = rt;
        spec.dev = DevCardType::None;
        spec.countBadge = 0;              // selected amount
        QCard* card = m_row->addCard(spec);

        // Left = add (up to 2 total, respecting bank availability)
        connect(card, &QCard::leftClicked, this, [this, rt]() {
            const int sel = totalSelected();
            if (sel >= 2) return;

            const int bankLeft = m_bankResources.count(rt) ? m_bankResources[rt] : 0;
            const int already = m_choice.receive[rt];
            if (already + 1 > bankLeft) return;

            m_choice.receive[rt] = already + 1;
            updateUiState();
        });

        // Right = remove
        connect(card, &QCard::rightClicked, this, [this, rt]() {
            const int already = m_choice.receive[rt];
            if (already <= 0) return;
            m_choice.receive[rt] = already - 1;
            updateUiState();
        });

        m_cards.push_back(card);
    }

    adjustSize();
}

void YearOfPlentyPopup::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const qreal radius = 12.0;
    QRectF rr = rect();
    rr.adjust(0.5, 0.5, -0.5, -0.5);

    // Fill
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 255, 240));
    p.drawRoundedRect(rr, radius, radius);

    // Border
    p.setPen(QPen(QColor(0,0,0,40), 1.0));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(rr, radius, radius);
}

void YearOfPlentyPopup::openAtGlobal(const QPoint& globalPos) {
    move(globalPos + QPoint(12, 12));
    show();
    raise();
    activateWindow();
}

void YearOfPlentyPopup::resetState(){
    for (auto& [rt, cnt] : m_choice.receive) {
        cnt = 0;
    }

    for (QCard* c : m_cards) {
        CardSpec s = c->spec();
        s.countBadge = 0;
        c->setSpec(s);
    }

    updateUiState();
}

void YearOfPlentyPopup::closePopup(){
    resetState();
    hide();
}

void YearOfPlentyPopup::closeEvent(QCloseEvent* event)
{
    event->ignore();
}

void YearOfPlentyPopup::showEvent(QShowEvent* event)
{
    //rebuild();
    updateUiState();
}