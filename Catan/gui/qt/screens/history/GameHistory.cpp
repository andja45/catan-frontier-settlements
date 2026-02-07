#include "GameHistory.h"

#include <QVBoxLayout>
#include <QPalette>
#include <QHeaderView>
#include <QAbstractItemView>

#include <common/theme/GameTheme.h>
#include <types/TypeAliases.h>

#include "common/audio/AudioManager.h"

static void applyHistoryStyle(QWidget* w) {
    w->setStyleSheet(R"(
        QWidget#Card {
            background: rgba(255,255,255,185);
            border: 2px solid rgba(0,0,0,40);
            border-radius: 16px;
        }

        QLabel { color:#1f2a33; font-size:16px; font-weight:700; }
        QLabel#Title { font-size: 22px; font-weight: 900; }

        QTableWidget {
            background: rgba(255,255,255,220);
            border: 2px solid rgba(0,0,0,40);
            border-radius: 12px;
            gridline-color: rgba(0,0,0,25);
            selection-background-color: rgba(41,128,185,60);
            selection-color: #1f2a33;
            font-size: 15px;
        }

        QHeaderView::section {
            background: rgba(255,255,255,235);
            color: #1f2a33;
            font-weight: 900;
            border: 1px solid rgba(0,0,0,25);
            padding: 8px;
        }

        QTableWidget::item {
            padding: 6px;
        }

        QPushButton#PrimaryBtn {
            min-height: 46px;
            border-radius: 23px;
            background: rgba(41,128,185,235);
            border: 2px solid rgba(0,0,0,80);
            font-size: 16px;
            font-weight: 900;
            color: white;
            padding: 0 26px;
        }
        QPushButton#PrimaryBtn:hover {
            background-color: #3A93D4;
            border-color: #2C7FB8;
        }
        QPushButton#PrimaryBtn:pressed {
            background-color: #1F6FA5;
            border-color: #184F75;
        }

        QPushButton#SecondaryBtn {
            min-height: 46px;
            border-radius: 23px;
            background: rgba(255,255,255,220);
            border: 2px solid rgba(0,0,0,55);
            font-size: 16px;
            font-weight: 900;
            color: #1f2a33;
            padding: 0 26px;
        }
        QPushButton#SecondaryBtn:hover {
            background-color: #EAF2F8;
            border-color: #6FA3C7;
        }
        QPushButton#SecondaryBtn:pressed {
            background-color: #D6E6F2;
            border-color: #5A8FB3;
        }
    )");
}

void GameHistory::colorRow(int row, const QColor &color) {
    for (int col = 0; col < m_historyTable->columnCount(); col++) {
        QTableWidgetItem *item = m_historyTable->item(row, col);
        if (item) item->setBackground(QBrush(color));
    }
}

void GameHistory::loadGameHistoryToTable() {
    m_historyGames.clear();
    m_historyTable->setRowCount(0);
    m_historyTable->setColumnCount(2);

    std::ifstream file(m_historyPath);
    if (!file.is_open()) {
        return;
    }

    nlohmann::json j;
    try {
        file >> j;
    } catch (const nlohmann::json::parse_error &e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        return;
    }

    if (!j.is_array()) {
        std::cerr << "History JSON is not an array\n";
        return;
    }

    int row = 0;
    for (const auto &entry : j) {
        try {
            GameData game = GameData();
            game.loadFromJson(entry);
            m_historyGames.push_back(game);

            m_historyTable->insertRow(row);

            std::string playersJoined;
            const auto &players = game.getPlayerNames();

            m_historyTable->setItem(row, m_nameToCol[ColumnNames::m_date],
                                    new QTableWidgetItem(QString::fromStdString(game.getDate())));

            for (size_t i = 0; i < players.size(); i++) {
                playersJoined += players[i];
                if (i + 1 < players.size()) playersJoined += ", ";
            }

            m_historyTable->setItem(row, m_nameToCol[ColumnNames::m_playerNames],
                                    new QTableWidgetItem(QString::fromStdString(playersJoined)));

            if (game.isGameWon()){
                colorRow(row, m_colorWin);
            } else{
                colorRow(row, m_colorLose);
            }
            row++;
        } catch (const std::exception &e) {
            std::cerr << "Skipping invalid game entry: " << e.what() << "\n";
        }
    }

    //m_historyTable->resizeColumnsToContents();
}

GameHistory::GameHistory(QWidget *parent)
    : QWidget(parent) {
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, GameTheme::getColorByResource(ResourceType::Sea));
    setPalette(pal);

    applyHistoryStyle(this);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(14, 14, 14, 14);
    mainLayout->setSpacing(14);

    auto* card = new QWidget(this);
    card->setObjectName("Card");

    auto* cardLay = new QVBoxLayout(card);
    cardLay->setContentsMargins(16, 16, 16, 16);
    cardLay->setSpacing(12);

    auto *title = new QLabel("Game History", card);
    title->setObjectName("Title");
    title->setAlignment(Qt::AlignCenter);

    m_historyTable = new QTableWidget(card);
    m_historyTable->setColumnCount(2);
    m_historyTable->setHorizontalHeaderLabels({ "Date", "Players" });
    m_historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    m_historyTable->setAlternatingRowColors(true);
    m_historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_historyTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_historyTable->verticalHeader()->setVisible(false);

    loadGameHistoryToTable();

    auto *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setContentsMargins(0, 6, 0, 0);
    buttonsLayout->setSpacing(12);

    m_refreshButton = new QPushButton("Refresh", card);
    m_refreshButton->setObjectName("PrimaryBtn");

    m_backButton = new QPushButton("Back", card);
    m_backButton->setObjectName("SecondaryBtn");

    buttonsLayout->addWidget(m_backButton, 0, Qt::AlignLeft);
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(m_refreshButton, 0, Qt::AlignRight);

    cardLay->addWidget(title);
    cardLay->addWidget(m_historyTable);
    cardLay->addLayout(buttonsLayout);

    mainLayout->addWidget(card);
    setLayout(mainLayout);

    resize(600, 400);

    connect(m_historyTable, &QTableWidget::cellDoubleClicked,
            this, &GameHistory::onRowDoubleClicked);

    connect(m_refreshButton, &QPushButton::clicked, this, [this]() {
        AudioManager::instance().playClick();
        loadGameHistoryToTable();
    });

    connect(m_backButton, &QPushButton::clicked, this, [this]() {
        AudioManager::instance().playClick();
        emit backToMainMenuRequested();
    });
}

void GameHistory::onRowDoubleClicked(int row, int column) {
    auto *details = new GameDetailsView(m_historyGames[row], this);
    details->setAttribute(Qt::WA_DeleteOnClose);
    details->show();
}