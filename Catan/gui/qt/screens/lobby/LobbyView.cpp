#define LOBBY_TEST_PLAYERS 1

#include "LobbyView.h"

#include <algorithm>
#include <QGridLayout>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QPalette>
#include <QAbstractItemView>
#include <QStyleOptionSlider>

#include <common/theme/GameTheme.h>
#include <screens/lobby/PlayerListItem.h>

#include "common/audio/AudioManager.h"

class ValueSlider : public QSlider {
public:
    using QSlider::QSlider;

protected:
    void paintEvent(QPaintEvent* e) override {
        QSlider::paintEvent(e);

        QStyleOptionSlider opt;
        initStyleOption(&opt);
        QRect handle = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);

        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);

        QFont f = font();
        f.setBold(true);
        f.setPointSizeF(std::max(9.0, handle.height() * 0.45));
        p.setFont(f);
        p.setPen(QColor(25, 35, 45));

        p.drawText(handle, Qt::AlignCenter, QString::number(value()));
    }
};

static void applyLobbyStyle(QWidget* w) {
    w->setStyleSheet(R"(
        QLabel {
        font-size: 16px;
        font-weight: bold;
        color: #1f2a33;
        }

        QWidget#Card {
            background: rgba(255,255,255,185);
            border: 2px solid rgba(0,0,0,40);
            border-radius: 16px;
        }

        QLabel#Title {
            font-size: 20px;
            font-weight: 800;
        }

        QPushButton#MapBtn {
            min-width: 76px; max-width: 76px;
            min-height: 54px; max-height: 54px;
            border-radius: 12px;
            background: rgba(255,255,255,220);
            border: 2px solid rgba(0,0,0,55);
            font-weight: 800;
        }
        QPushButton#MapBtn:checked {
            background: rgba(41,128,185,235);
            border: 2px solid rgba(0,0,0,80);
            color: white;
        }

        QPushButton#LoadBtn {
            min-height: 40px;
            border-radius: 12px;
            background: rgba(255,255,255,220);
            border: 2px solid rgba(0,0,0,55);
            font-weight: 700;
            padding: 0 14px;
        }

        QSlider::groove:horizontal {
            height: 10px; border-radius: 5px;
            background: rgba(0,0,0,28);
        }
        QSlider::sub-page:horizontal {
            border-radius: 5px;
            background: rgba(41,128,185,210);
        }
        QSlider::handle:horizontal {
            width: 30px; height: 30px;      /* bigger so number fits */
            margin: -11px 0;
            border-radius: 15px;
            background: white;
            border: 2px solid rgba(0,0,0,60);
        }

        QPushButton#StartBtn {
            min-height: 50px;
            border-radius: 25px;
            background: rgba(41,128,185,235);
            border: 2px solid rgba(0,0,0,80);
            font-size: 16px;
            font-weight: 900;
            padding: 0 40px;
            color: white;
        }
        QPushButton#StartBtn:disabled {
            background: rgba(0,0,0,22);
            border: 2px solid rgba(0,0,0,18);
            color: rgba(0,0,0,90);
        }

        QListWidget { background: transparent; }
    )");
}

static QPushButton* makeMapBtn(QWidget* parent, const QString& text) {
    auto* b = new QPushButton(text, parent);
    b->setObjectName("MapBtn");
    b->setCheckable(true);
    return b;
}

LobbyView::LobbyView(const std::string& gameName, RoleType type, QWidget* parent)
    : QWidget(parent),
      m_gameName(QString::fromStdString(gameName)) {
    setWindowTitle(QString("Game lobby: %1").arg(m_gameName));

    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, GameTheme::getColorByResource(ResourceType::Sea));
    setPalette(pal);

    applyLobbyStyle(this);

    auto* root = new QHBoxLayout(this);
    root->setContentsMargins(14, 14, 14, 14);
    root->setSpacing(14);

    root->addWidget(makePlayersCard(), 1);
    root->addWidget(makeSettingsCard(), 2);

    resize(860, 480);

    if (type == RoleType::Player) disableAll();

    connect(m_startButton, &QPushButton::clicked, this, [this]() {
        AudioManager::instance().playClick();
        if (m_customBtn->isChecked() && m_customMapPath.isEmpty()) {
            m_errorLabel->setText("Please load a custom map before starting the game.");
            m_errorLabel->setVisible(true);
            return;
        }
        m_errorLabel->setVisible(false);
        emit startGameRequested(getConfig(), m_customMapPath.toStdString());
    });

#if LOBBY_TEST_PLAYERS
    onAddPlayer("Ana");
    onAddPlayer("Marko");
    onAddPlayer("Jovan");
    onAddPlayer("Mina");
#endif
}

QWidget* LobbyView::makePlayersCard() {
    auto* card = new QWidget(this);
    card->setObjectName("Card");
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto* lay = new QVBoxLayout(card);
    lay->setContentsMargins(14, 14, 14, 14);
    lay->setSpacing(12);

    auto* title = new QLabel("Players", card);
    title->setObjectName("Title");
    title->setAlignment(Qt::AlignHCenter);

    m_playerCountLabel = new QLabel("Players joined: 0", card);
    m_playerCountLabel->setStyleSheet("font-size: 16px; font-weight: normal;");
    m_playerCountLabel->setAlignment(Qt::AlignHCenter);

    m_playerList = new QListWidget(card);
    m_playerList->setFrameShape(QFrame::NoFrame);
    m_playerList->setSpacing(8);

    m_playerList->setSelectionMode(QAbstractItemView::NoSelection);
    m_playerList->setFocusPolicy(Qt::NoFocus);
    m_playerList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    lay->addWidget(title);
    lay->addWidget(m_playerCountLabel);
    lay->addWidget(m_playerList, 1);

    return card;
}

QWidget* LobbyView::makeSettingsCard() {
    auto* card = new QWidget(this);
    card->setObjectName("Card");
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto* grid = new QGridLayout(card);
    grid->setContentsMargins(16, 16, 16, 16);
    grid->setHorizontalSpacing(12);
    grid->setVerticalSpacing(22);
    grid->setColumnMinimumWidth(0, 140);

    auto* room = new QLabel(QString("Room name: %1").arg(m_gameName), card);
    room->setObjectName("Title");
    room->setAlignment(Qt::AlignHCenter);
    grid->addWidget(room, 0, 0, 1, 3);

    auto* boardLbl = new QLabel("Board", card);
    grid->addWidget(boardLbl, 1, 0);

    m_normalBtn   = makeMapBtn(card, "Standard");
    m_extendedBtn = makeMapBtn(card, "Extended");
    m_customBtn   = makeMapBtn(card, "Custom");
    m_normalBtn->setChecked(true);

    m_mapButtonGroup = new QButtonGroup(card);
    m_mapButtonGroup->setExclusive(true);
    m_mapButtonGroup->addButton(m_normalBtn, 1);
    m_mapButtonGroup->addButton(m_extendedBtn, 2);
    m_mapButtonGroup->addButton(m_customBtn, 3);

    auto* mapBtns = new QHBoxLayout;
    mapBtns->setContentsMargins(0, 0, 0, 0);
    mapBtns->setSpacing(10);
    mapBtns->addWidget(m_normalBtn);
    mapBtns->addWidget(m_extendedBtn);
    mapBtns->addWidget(m_customBtn);
    mapBtns->addStretch(1);

    auto* mapBtnsWrap = new QWidget(card);
    mapBtnsWrap->setLayout(mapBtns);
    mapBtnsWrap->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    grid->addWidget(mapBtnsWrap, 1, 1);

    m_loadBoardButton = new QPushButton("Load board", card);
    m_loadBoardButton->setObjectName("LoadBtn");
    m_loadBoardButton->setVisible(false);
    grid->addWidget(m_loadBoardButton, 1, 2);

    connect(m_customBtn, &QPushButton::toggled, this, [this](bool checked){
        m_loadBoardButton->setVisible(checked);
    });

    connect(m_loadBoardButton, &QPushButton::clicked, this, [this](){
        AudioManager::instance().playClick();
        QString filePath = QFileDialog::getOpenFileName(
            this, "Load Custom Board", QString(), "JSON Files (*.json)"
        );
        if (!filePath.isEmpty()) {
            m_customMapPath = filePath;
            m_errorLabel->setVisible(false);
            emit customMapSelected(filePath);
        }
    });

    connect(m_mapButtonGroup,
            QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            this,
            [this](QAbstractButton*) {
                AudioManager::instance().playClick();
                emit configChanged(getConfig());
            });

    auto* playersLbl = new QLabel("Players", card);
    grid->addWidget(playersLbl, 2, 0);

    m_playersSlider = new ValueSlider(Qt::Horizontal, card);
    m_playersSlider->setRange(2, 8);
    m_playersSlider->setValue(4);
    m_playersSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    grid->addWidget(m_playersSlider, 2, 1, 1, 2);

    connect(m_playersSlider, &QSlider::valueChanged, this, [this](int){
        emit configChanged(getConfig());
        m_playersSlider->update();
    });

    auto* pointsLbl = new QLabel("Winning points", card);
    grid->addWidget(pointsLbl, 3, 0);

    m_pointsSlider = new ValueSlider(Qt::Horizontal, card);
    m_pointsSlider->setRange(3, 20);
    m_pointsSlider->setValue(10);
    m_pointsSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    grid->addWidget(m_pointsSlider, 3, 1, 1, 2);

    connect(m_pointsSlider, &QSlider::valueChanged, this, [this](int){
        emit configChanged(getConfig());
        m_pointsSlider->update();
    });

    m_errorLabel = new QLabel(card);
    m_errorLabel->setStyleSheet("color:#b00020; font-weight:800;");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setVisible(false);
    grid->addWidget(m_errorLabel, 4, 0, 1, 3);

    m_startButton = new QPushButton("Start", card);
    m_startButton->setObjectName("StartBtn");

    auto* startWrap = new QWidget(card);
    startWrap->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto* startLay = new QHBoxLayout(startWrap);
    startLay->setContentsMargins(0, 0, 0, 0);
    startLay->addStretch(1);
    startLay->addWidget(m_startButton);
    startLay->addStretch(1);

    grid->addWidget(startWrap, 5, 0, 1, 3);

    grid->setAlignment(mapBtnsWrap, Qt::AlignVCenter);
    grid->setAlignment(m_playersSlider, Qt::AlignVCenter);
    grid->setAlignment(m_pointsSlider, Qt::AlignVCenter);
    grid->setAlignment(startWrap, Qt::AlignVCenter);

    return card;
}

void LobbyView::disableAll()
{
    if (m_startButton) m_startButton->setEnabled(false);
    if (m_playersSlider) m_playersSlider->setEnabled(false);
    if (m_pointsSlider)  m_pointsSlider->setEnabled(false);
    setMapButtonsEnabled(false);
    if (m_loadBoardButton) m_loadBoardButton->setEnabled(false);
}

void LobbyView::setMapButtonsEnabled(bool on) {
    if (m_normalBtn)   m_normalBtn->setEnabled(on);
    if (m_extendedBtn) m_extendedBtn->setEnabled(on);
    if (m_customBtn)   m_customBtn->setEnabled(on);
}

void LobbyView::setPlayerCount(int count) {
    m_playerCountLabel->setText(QString("Players joined: %1").arg(count));
}

void LobbyView::addPlayer(const QString& playerName) {
    const int idx = m_playerList->count();
    const QColor col = GameTheme::playerColors.at(idx % (int)GameTheme::playerColors.size());

    auto* item = new QListWidgetItem;
    auto* widget = new PlayerListItem(col, playerName, m_playerList);

    item->setSizeHint(widget->sizeHint());
    item->setBackground(Qt::transparent);

    m_playerList->addItem(item);
    m_playerList->setItemWidget(item, widget);

    setPlayerCount(m_playerList->count());
}

GameConfig LobbyView::getConfig() const { // TODO add

}

void LobbyView::setConfig(const GameConfig& config) {

}

void LobbyView::onAddPlayer(const QString& playerName) {
    addPlayer(playerName);
}

void LobbyView::onConfigChanged(const GameConfig& config) {
    setConfig(config);
}
