#include "GameDetailsView.h"

QGroupBox* GameDetailsView::SetBasicInfo(const GameData &game){
    auto *infoBox = new QGroupBox("Game Info", this);
    auto *infoLayout = new QHBoxLayout(infoBox);

    infoLayout->addWidget(new QLabel(
        QString("Game name: %1").arg(game.getGameName().data()), this));

    infoLayout->addWidget(new QLabel(
        QString("Date: %1")
            .arg(QString::fromStdString(game.getDate())), this));

    infoLayout->addWidget(new QLabel(
        QString("Total Turns: %1")
            .arg(game.getTurns()), this));

    infoBox->setLayout(infoLayout);
    return infoBox;
}
QHBoxLayout* GameDetailsView::SetChartsLayout(const GameData &game){
    auto *chartsLayout = new QHBoxLayout();
    {
        auto *set = new QBarSet("Dice Rolls");
        QStringList categories;

        auto diceRolls = game.getDiceRolls();

        for (int i = 2; i <= 12; ++i) {
            set->append(diceRolls.count(i) ? diceRolls.at(i) : 0);
            categories << QString::number(i);
        }

        auto *series = new QBarSeries();
        series->append(set);

        auto *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Dice Roll Distribution");
        chart->legend()->setVisible(false);

        auto *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        auto *axisY = new QValueAxis();
        axisY->setTitleText("Count");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        auto *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        chartsLayout->addWidget(chartView);
    }

    {
        auto *series = new QStackedBarSeries();
        QStringList categories;

        auto resources = game.getResourceRolls();

        for (ResourceType type : resourceCardTypes) {
            categories << QString::fromStdString(toString(type));
        }
        for (int i = 0; i < resourceCardTypes.size(); i++) {
            ResourceType type = resourceCardTypes[i];
            auto *set = new QBarSet(QString::fromStdString(toString(type)));

            for (int j = 0; j < resourceCardTypes.size(); j++) {
                if (i == j){
                    set->append(resources[type]);
                }
                else{
                    set->append(0);
                }
            }

            set->setColor(GameTheme::getColorByResource(type));
            series->append(set);
        }

        series->setLabelsVisible(true);
        series->setLabelsPosition(QAbstractBarSeries::LabelsCenter);
        series->setBarWidth(0.7);

        auto *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Resource Distribution");
        chart->legend()->setVisible(false);


        auto *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        auto *axisY = new QValueAxis();
        axisY->setVisible(false);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        auto *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartsLayout->addWidget(chartView);
    }
    return chartsLayout;
}

QGroupBox* GameDetailsView::SetPlayersTable(const GameData &game){
    auto *playersBox = new QGroupBox("Players & Final Points", this);
    auto *playersLayout = new QVBoxLayout(playersBox);

    QTableWidget *pointsTable = new QTableWidget(this);
    pointsTable->setColumnCount(5);
    pointsTable->setHorizontalHeaderLabels({
        "Player",
        "Points",
        "Winner",
        "Largest Army",
        "Longest Road"
    });
    pointsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    pointsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pointsTable->setSelectionMode(QAbstractItemView::NoSelection);

    const auto &points = game.getPointsByPlayer();
    const std::string &winner = game.getWinningPlayer();
    const std::string &biggestArmyOwner = game.getLargestArmyOwner();
    const std::string &longestRoadOwner = game.getLongestRoadOwner();

    int row = 0;
    pointsTable->setRowCount(static_cast<int>(points.size()));

    for (const auto &[player, score] : points) {
        pointsTable->setItem(row, 0,
                             new QTableWidgetItem(QString::fromStdString(player)));

        pointsTable->setItem(row, 1,
                             new QTableWidgetItem(QString::number(score)));

        if (player == winner) {
            auto *winnerItem = new QTableWidgetItem("⭐");
            winnerItem->setTextAlignment(Qt::AlignCenter);
            pointsTable->setItem(row, 2, winnerItem);
        } else {
            pointsTable->setItem(row, 2,
                                 new QTableWidgetItem(""));
        }

        if (player == biggestArmyOwner) {
            auto *armyItem = new QTableWidgetItem("⚔️");
            armyItem->setTextAlignment(Qt::AlignCenter);
            pointsTable->setItem(row, 3, armyItem);
        } else {
            pointsTable->setItem(row, 3,
                                 new QTableWidgetItem(""));
        }

        if (player == longestRoadOwner) {
            auto *roadItem = new QTableWidgetItem("🐎️");
            roadItem->setTextAlignment(Qt::AlignCenter);
            pointsTable->setItem(row, 4, roadItem);
        } else {
            pointsTable->setItem(row, 4,
                                 new QTableWidgetItem(""));
        }

        ++row;
    }

    playersLayout->addWidget(pointsTable);
    playersBox->setLayout(playersLayout);
    return playersBox;
}
GameDetailsView::GameDetailsView(const GameData &game, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Game Details");
    resize(900, 650);

    auto *mainLayout = new QVBoxLayout(this);
    auto *infoBox = SetBasicInfo(game);
    mainLayout->addWidget(infoBox);

    auto *playersBox = SetPlayersTable(game);
    mainLayout->addWidget(playersBox);

    auto * chartsLayout = SetChartsLayout(game);
    mainLayout->addLayout(chartsLayout);

}
