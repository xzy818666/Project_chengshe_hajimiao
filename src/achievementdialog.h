#ifndef ACHIEVEMENTDIALOG_H
#define ACHIEVEMENTDIALOG_H

#include <QDialog>

class AchievementManager;

class AchievementDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AchievementDialog(AchievementManager* manager, QWidget* parent = nullptr);
};

#endif // ACHIEVEMENTDIALOG_H
