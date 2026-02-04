#include <screens/connect/HostJoinView.h>

#include <QVBoxLayout>
#include <QFormLayout>
#include <QPalette>
#include <common/theme/GameTheme.h>

#include "common/audio/AudioManager.h"

static void applyHostJoinStyle(QWidget* w) {
    w->setStyleSheet(R"(
        QWidget#Card {
            background: rgba(255,255,255,185);
            border: 2px solid rgba(0,0,0,40);
            border-radius: 16px;
        }

        QLabel { color:#1f2a33; font-size:16px; font-weight:700; }
        QLabel#Title { font-size: 22px; font-weight: 900; }

        QLineEdit {
            min-height: 40px;
            border-radius: 12px;
            padding: 0 12px;
            background: rgba(255,255,255,230);
            border: 2px solid rgba(0,0,0,55);
            font-size: 16px;
        }
        QLineEdit:focus {
            border: 2px solid rgba(41,128,185,200);
        }

        QPushButton#ActionBtn {
            min-height: 54px;
            border-radius: 27px;
            background: rgba(41,128,185,235);
            border: 2px solid rgba(0,0,0,80);
            font-size: 18px;
            font-weight: 900;
            color: white;
            padding: 0 40px;
        }
        QPushButton#ActionBtn:pressed {
            background: rgba(35,110,160,235);
        }

        QPushButton#BackBtn {
            min-height: 54px;
            border-radius: 27px;
            background: rgba(255,255,255,220);
            border: 2px solid rgba(0,0,0,55);
            font-size: 18px;
            font-weight: 900;
            color: #1f2a33;
            padding: 0 40px;
        }
        QPushButton#BackBtn:pressed {
            background: rgba(255,255,255,180);
        }

        QLabel#Error {
            color: #b00020;
            font-weight: 900;
        }

        QPushButton#ActionBtn:hover {
            background-color: #3A93D4;
            border-color: #2C7FB8;
        }

        QPushButton#ActionBtn:pressed {
            background-color: #1F6FA5;
            border-color: #184F75;
        }

        QPushButton#BackBtn:hover {
            background-color: #EAF2F8;
            border-color: #6FA3C7;
        }

        QPushButton#BackBtn:pressed {
            background-color: #D6E6F2;
            border-color: #5A8FB3;
        }
    )");
}

void HostJoinView::closeEvent(QCloseEvent *event) {
    emit closed();
    QDialog::closeEvent(event);
}

HostJoinView::HostJoinView(RoleType type, QWidget *parent)
    : QDialog(parent), m_type(type) {
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    setWindowState(Qt::WindowNoState);

    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, GameTheme::getColorByResource(ResourceType::Sea));
    setPalette(pal);

    applyHostJoinStyle(this);

    auto* card = new QWidget(this);
    card->setObjectName("Card");
    auto* cardLay = new QVBoxLayout(card);
    cardLay->setContentsMargins(18, 18, 18, 18);
    cardLay->setSpacing(14);

    auto* title = new QLabel(type == RoleType::Host ? "Host game" : "Join game", card);
    title->setObjectName("Title");
    title->setAlignment(Qt::AlignHCenter);

    m_gameNameEdit = new QLineEdit(card);
    m_playerNameEdit = new QLineEdit(card);
    m_gameNameEdit->setPlaceholderText("Game name");
    m_playerNameEdit->setPlaceholderText("Your name");

    auto* formLayout = new QFormLayout;
    formLayout->setLabelAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    formLayout->setFormAlignment(Qt::AlignHCenter);
    formLayout->setHorizontalSpacing(14);
    formLayout->setVerticalSpacing(12);
    formLayout->addRow("Game:", m_gameNameEdit);
    formLayout->addRow("Player:", m_playerNameEdit);

    m_errorLabel = new QLabel("That game does not exist.", card);
    m_errorLabel->setObjectName("Error");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setVisible(false);

    m_actionButton = new QPushButton(card);
    m_actionButton->setObjectName("ActionBtn");
    m_actionButton->setText(type == RoleType::Host ? "Host" : "Join");

    auto* backBtn = new QPushButton("Back", card);
    backBtn->setObjectName("BackBtn");
    connect(backBtn, &QPushButton::clicked, this, [this]() {
        AudioManager::instance().playClick();
        close();
    }); // TODO connect to mainmenu

    cardLay->addWidget(title);
    cardLay->addLayout(formLayout);
    cardLay->addWidget(m_errorLabel);

    auto* btnRow = new QHBoxLayout;
    btnRow->addWidget(backBtn);
    btnRow->addStretch(1);
    btnRow->addWidget(m_actionButton);
    cardLay->addLayout(btnRow);

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(18, 18, 18, 18);
    root->addStretch(1);
    root->addWidget(card);
    root->addStretch(1);

    card->setMaximumWidth(520);
    resize(640, 360);

    connect(m_actionButton, &QPushButton::clicked, this, [this]() {
        AudioManager::instance().playClick();
        clearError();
        if (m_gameNameEdit->text().isEmpty()) {
            onRejected("Please input a game name.");
        } else if (m_playerNameEdit->text().isEmpty()) {
            onRejected("Please input your name.");
        } else {
            emit requestSent(m_gameNameEdit->text(), m_playerNameEdit->text());
        }
    });

    card->setMaximumWidth(QWIDGETSIZE_MAX);
    root->setSizeConstraint(QLayout::SetFixedSize);
    adjustSize();
    setFixedSize(size());
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void HostJoinView::onRejected(const std::string &reason /*= "Couldn't join game."*/) {
    m_errorLabel->setText(QString::fromStdString(reason));
    m_errorLabel->setVisible(true);
}

void HostJoinView::clearError() {
    m_errorLabel->setVisible(false);
}
