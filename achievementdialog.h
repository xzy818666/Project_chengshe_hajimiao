#ifndef ACHIEVEMENTDIALOG_H
#define ACHIEVEMENTDIALOG_H

#include <QDialog>

class AchievementManager;
class QListWidget;

class AchievementDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AchievementDialog(AchievementManager* manager, QWidget* parent = nullptr);

private:
    QListWidget* m_list;
};

#endif // ACHIEVEMENTDIALOG_H
