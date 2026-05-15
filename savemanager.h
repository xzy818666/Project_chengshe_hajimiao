#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QString>

class SaveManager
{
public:
    static QString savePath();
    static bool saveFinal(double nextLifeLoss, double yezhang, const QString& difficulty);
    static bool hasSave();
};

#endif // SAVEMANAGER_H
