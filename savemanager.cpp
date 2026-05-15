#include "savemanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDateTime>

QString SaveManager::savePath()
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dir);
    return dir + "/final_record.json";
}

bool SaveManager::saveFinal(double nextLifeLoss, double yezhang, const QString& difficulty)
{
    QJsonObject root;
    root["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    root["difficulty"] = difficulty;
    root["nextLifeLoss"] = nextLifeLoss;
    root["yezhang"] = yezhang;

    QFile file(savePath());
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    return true;
}

bool SaveManager::hasSave()
{
    return QFile::exists(savePath());
}
