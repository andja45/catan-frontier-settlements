//
// Created by matija on 2/8/26.
//

#ifndef CATAN_PATHSERVICE_HPP
#define CATAN_PATHSERVICE_HPP


#include <QString>
#include <QStandardPaths>
#include <QDir>

class PathService
{
public:
    static PathService& instance()
    {
        static PathService instance;
        return instance;
    }
    QString appDataDir() const {
        return QStandardPaths::writableLocation(
            QStandardPaths::AppDataLocation);
    }
    QString resourceDir() const
    {
        return appDataDir() + "/resources/";
    }

    QString resourceFile(const QString& fileName) const
    {
        return resourceDir() + fileName;
    }
private:
    PathService(){
        (void)QDir().mkpath(appDataDir());
        (void)QDir().mkpath(resourceDir());

    }
};


#endif //CATAN_PATHSERVICE_HPP