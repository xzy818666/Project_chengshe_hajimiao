#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QString>

class SaveManager
{
public:
    static QString savePath();
    static bool saveFinal(double nextLifeLoss, double yezhang, const QString& difficulty);
    static bool hasSave();
    static double loadNextLifeLoss();
};

#endif // SAVEMANAGER_H
