#include "GameDetailsView.h"

QGroupBox* GameDetailsView::SetBasicInfo(const GameData &game){
    auto *infoBox = new QGroupBox("Game Info", this);
    auto *infoLayout = new QHBoxLayout(infoBox);

    infoLayout->addWidget(new QLabel(
        QString("Game ID: %1").arg(game.getId()), this));

    infoLayout->addWidget(new QLabel(
        QString("Date: %1")
            .arg(QString::fromStdString(game.getDate())), this));

    infoBox->setLayout(infoLayout);
    return infoBox;
}
QHBoxLayout* GameDetailsView::SetChartsLayout(const GameData &game){
    auto *chartsLayout = new QHBoxLayout();
    {
        QBarSet *set = new QBarSet("Dice Rolls");
        QStringList categories;

        auto diceRolls = game.getDiceRolls(); // map<int,int>

        for (int i = 2; i <= 12; ++i) {
            set->append(diceRolls.count(i) ? diceRolls.at(i) : 0);
            categories << QString::number(i);
        }

        QBarSeries *series = new QBarSeries();
        series->append(set);

        QChart *chart = new QChart();
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
        QBarSet *set = new QBarSet("Resources");
        QStringList categories;

        auto resources = game.getResourceRolls();

        for (const auto &[type, count] : resources) {
            set->append(count);
            categories << QString::fromStdString(toString(type));
        }

        QBarSeries *series = new QBarSeries();
        series->append(set);

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Resource Distribution");
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
    return chartsLayout;
}

QGroupBox* GameDetailsView::SetPlayersTable(const GameData &game){
    auto *playersBox = new QGroupBox("Players & Final Points", this);
    auto *playersLayout = new QVBoxLayout(playersBox);

    QTableWidget *pointsTable = new QTableWidget(this);
    pointsTable->setColumnCount(3);
    pointsTable->setHorizontalHeaderLabels({
        "Player",
        "Points",
        "Winner"
    });

    pointsTable->horizontalHeader()->setStretchLastSection(true);
    pointsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pointsTable->setSelectionMode(QAbstractItemView::NoSelection);

    const auto &points = game.getPointsByPlayer();
    const std::string &winner = game.getWinningPlayer();

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
