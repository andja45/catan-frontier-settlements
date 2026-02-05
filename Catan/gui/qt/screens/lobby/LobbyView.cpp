
#include <QButtonGroup>
#include <QFormLayout>
#include <screens/lobby/LobbyView.h>
#include<string.h>
#include <screens/lobby/AddBotListItem.h>

LobbyView::LobbyView(const std::string &gameName,RoleType type, QWidget *parent)
    : QWidget(parent), m_gameName(QString::fromStdString(gameName)), m_role(type)
{

    setWindowTitle(QString("Game lobby: %1").arg(m_gameName));

    m_playerCountLabel = new QLabel("Players joined: 0", this);

    m_playerList = new QListWidget(this);
    m_playerList->setSpacing(4);
    addAddBotEntry();

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_playerCountLabel);
    mainLayout->addWidget(m_playerList);

    m_maxPlayersSpin = new QSpinBox(this);
    m_maxPlayersSpin->setRange(2, 4);
    m_maxPlayersSpin->setValue(4);

    connect(m_maxPlayersSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [this]() {
        if (m_role==RoleType::Host)
            emit configChanged(getConfig());
    });


    m_pointsToWinSpin = new QSpinBox(this);
    m_pointsToWinSpin->setRange(8, 20);
    m_pointsToWinSpin->setValue(10);
    connect(m_pointsToWinSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [this]() {
        if (m_role==RoleType::Host)
            emit configChanged(getConfig());
        });


    auto *mapGroupBox = setMapSelect();
    auto *settingsForm = new QFormLayout;
    settingsForm->addRow("Max players:", m_maxPlayersSpin);
    settingsForm->addRow("Points to win:", m_pointsToWinSpin);

    auto *settingsBox = new QGroupBox("Game Settings", this);
    settingsBox->setLayout(settingsForm);

    m_errorLabel = new QLabel(this);
    m_errorLabel->setStyleSheet("color: red;");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setVisible(false);

    m_startButton = new QPushButton("Start Game", this);

    auto *sideLayout = new QVBoxLayout;
    sideLayout->addWidget(settingsBox);
    sideLayout->addWidget(mapGroupBox);
    sideLayout->addStretch();
    sideLayout->addWidget(m_errorLabel);
    sideLayout->addWidget(m_startButton);

    auto *rootLayout = new QHBoxLayout(this);
    rootLayout->addLayout(mainLayout, 3);
    rootLayout->addLayout(sideLayout, 1);
    resize(600, 400);


    if (type == RoleType::Player) {
        disableAll();
    }

    connect(m_startButton, &QPushButton::clicked, this, [this]() {
        MapType mapType = MapType::Classic;
        if (m_customMapRadio->isChecked() && m_customMapPath.isEmpty()){
            m_errorLabel->setText("Please load a custom map before starting the game.");
            m_errorLabel->setVisible(true);
            return;
        }

        m_errorLabel->setVisible(false);
        emit startGameRequested(getConfig(),m_customMapPath.toStdString());
    });

}
QGroupBox* LobbyView::setMapSelect()
{
    m_mapButtonGroup = new QButtonGroup(this);

    m_classicMapRadio = new QRadioButton("Classic", this);
    m_extendedMapRadio = new QRadioButton("Extended", this);
    m_customMapRadio = new QRadioButton("Custom", this);

    m_classicMapRadio->setChecked(true);

    m_mapButtonGroup->addButton(m_classicMapRadio);
    m_mapButtonGroup->addButton(m_extendedMapRadio);
    m_mapButtonGroup->addButton(m_customMapRadio);

    auto *mapLayout = new QVBoxLayout;
    mapLayout->addWidget(m_classicMapRadio);
    mapLayout->addWidget(m_extendedMapRadio);
    mapLayout->addWidget(m_customMapRadio);

    m_loadBoardButton = new QPushButton("Load Board", this);
    m_loadBoardButton->setVisible(false);
    mapLayout->addWidget(m_loadBoardButton);

    auto *mapGroupBox = new QGroupBox("Map", this);
    mapGroupBox->setLayout(mapLayout);

    connect(m_customMapRadio, &QRadioButton::toggled, this,
            [this](bool checked) {
                m_loadBoardButton->setVisible(checked);
            });
    connect(m_loadBoardButton, &QPushButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(
            this,
            "Load Custom Board",
            QString(),
            "JSON Files (*.json)"
            );

        if (!filePath.isEmpty()) {
            m_customMapPath = filePath;
            m_errorLabel->setVisible(false);
            emit customMapSelected(filePath);
        }
    });
    return mapGroupBox;
}
void LobbyView::addPlayer(const QString &playerName)
{
    int insertIndex = m_playerList->count() - 1;

    auto *item = new QListWidgetItem;
    auto *widget = new PlayerListItem(playerName);

    item->setSizeHint(widget->sizeHint());
    item->setBackground(
        (m_playerList->count() % 2 == 0)
            ? QColor({211, 223, 240})
            : QColor({215, 211, 240})
        );
    m_playerList->insertItem(insertIndex, item);
    m_playerList->setItemWidget(item, widget);


    connect(widget, &PlayerListItem::kickRequested,
            this, &LobbyView::kickPlayerRequested);
}
void LobbyView::removePlayer(const QString &playerName)
{
    for (int i = 0; i < m_playerList->count(); ++i) {
        auto *item = m_playerList->item(i);
        auto *widget = qobject_cast<PlayerListItem*>(
            m_playerList->itemWidget(item)
            );

        if (widget && widget->findChild<QLabel*>()->text() == playerName) {
            delete m_playerList->takeItem(i);
            return;
        }
    }
}

GameConfig LobbyView::getConfig() {
    GameConfig config;
    BoardType b = BoardType::Classic;
    if (m_extendedMapRadio->isChecked()) b = BoardType::Extended;
    else if (m_customMapRadio->isChecked()) {
        b = BoardType::Custom;
    }
    config.setBoardType(b);
    config.setNumPlayers(m_maxPlayersSpin->cleanText().toInt());
    config.setWinningPoints(m_pointsToWinSpin->cleanText().toInt());

    return config;
}

void LobbyView::setConfig(const GameConfig &config) {
    m_playerList->clear();
    for (const auto& p: config.getPlayerNames()) {
        addPlayer(QString::fromStdString(p));
    }
    m_gameName=QString::fromStdString(config.getName());
    if (m_role==RoleType::Host) { // host sets settings clients observe them
        return;
    }

    if (config.getBoardType() == BoardType::Extended) m_extendedMapRadio->setChecked(true);
    else if (config.getBoardType() == BoardType::Custom) m_customMapRadio->setChecked(true);
    else m_classicMapRadio->setChecked(true);

    m_maxPlayersSpin->setValue(config.getMaxPlayers());
    m_pointsToWinSpin->setValue(config.getPointsToWin());

}

void LobbyView::onAddPlayer(const QString &playerName) {
    addPlayer(playerName);
}

void LobbyView::onRemovePlayer(const QString &playerName) {
    removePlayer(playerName);
}

void LobbyView::onConfigChanged(const GameConfig &config) {
    setConfig(config);
}

void LobbyView::setPlayerCount(int count)
{
    m_playerCountLabel->setText(
        QString("Players joined: %1").arg(count)
        );
}
void LobbyView::addAddBotEntry()
{
    auto *item = new QListWidgetItem(m_playerList);
    auto *widget = new AddBotListItem(m_playerList); // TODO remove this

    item->setSizeHint(widget->sizeHint());
    item->setBackground(QColor({239, 240, 211}));
    m_playerList->addItem(item);
    m_playerList->setItemWidget(item, widget);

    connect(widget, &AddBotListItem::addBotRequested,
            this, &LobbyView::addBotRequested);
}

void LobbyView::disableAll() {
    m_startButton->setEnabled(false);
    m_pointsToWinSpin->setEnabled(false);
    m_maxPlayersSpin->setEnabled(false);
    m_loadBoardButton->setEnabled(false);
    m_extendedMapRadio->setEnabled(false);
    m_classicMapRadio->setEnabled(false);
    m_customMapRadio->setEnabled(false);
}

