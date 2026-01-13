#ifndef GAMEDETAILSVIEW_H
#define GAMEDETAILSVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include<QTableWidget>

#include <QHBoxLayout>
#include <QGroupBox>
#include <QString>

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include<../../game/gamemodel/GameData.h>
#include<QDialog>

class GameDetailsView : public QDialog
{
    Q_OBJECT
    int m_gameId;
    std::map<int, int> m_diceCounts;

public:
    GameDetailsView(const GameData &game);
    GameDetailsView(const GameData &game, QWidget *parent);
    QGroupBox* SetBasicInfo(const GameData &game);
    QHBoxLayout* SetChartsLayout(const GameData &game);
    QGroupBox* SetPlayersTable(const GameData &game);
};

#endif // GAMEDETAILSVIEW_H
