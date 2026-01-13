#include "gamehistory.h"
void GameHistory::colorRow(int row, const QColor &color){ //FIXME - ne radi
    for (int col = 0; col < m_historyTable->columnCount(); col++) {
        QTableWidgetItem *item = m_historyTable->item(row, col);
        if (item)
            item->setBackground(QBrush(color));
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

            m_historyTable->setItem(row, m_nameToCol[ColumnNames::m_date], new QTableWidgetItem(QString::fromStdString(game.getDate())));

            for (size_t i = 0; i < players.size(); i++) {
                playersJoined += players[i];
                if (i + 1 < players.size())
                    playersJoined += ", ";
            }

            m_historyTable->setItem(row, m_nameToCol[ColumnNames::m_playerNames], new QTableWidgetItem(QString::fromStdString(playersJoined)));
            if (game.isGameWon()){
                colorRow(row, m_colorWin);}
            else{
                colorRow(row, m_colorLose);}
            row++;
        }
        catch (const std::exception &e) {
            std::cerr << "Skipping invalid game entry: " << e.what() << "\n";
        }
    }

    m_historyTable->resizeColumnsToContents();
}
GameHistory::GameHistory(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);

    auto *title = new QLabel("Game History", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);


    m_historyTable = new QTableWidget(this);
    m_historyTable->setColumnCount(2);
    m_historyTable->setHorizontalHeaderLabels({
        "Date",
        "Players"
    });

    loadGameHistoryToTable();

    auto *buttonsLayout = new QHBoxLayout();

    m_refreshButton = new QPushButton("Refresh", this);
    m_backButton = new QPushButton("Back to Main Menu", this);
    m_backButton->setStyleSheet("padding: 6px 12px;");

    buttonsLayout->addWidget(m_backButton, 0, Qt::AlignLeft);
    buttonsLayout->addWidget(m_refreshButton, 0, Qt::AlignRight);

    mainLayout->addWidget(title);
    mainLayout->addWidget(m_historyTable);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
    resize(600, 400);

    connect(m_historyTable, &QTableWidget::cellDoubleClicked,
            this, &GameHistory::onRowDoubleClicked);
    connect(m_backButton, &QPushButton::clicked, this, [this]() {
        emit backToMainMenuRequested();
    });

}
void GameHistory::onRowDoubleClicked(int row, int column)
{
    auto *details = new GameDetailsView(m_historyGames[row], this);
    details->setAttribute(Qt::WA_DeleteOnClose);
    details->show();
}
