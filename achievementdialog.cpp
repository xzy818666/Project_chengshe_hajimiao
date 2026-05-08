#include "achievementdialog.h"
#include "achievementmanager.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QBrush>
#include <QColor>

AchievementDialog::AchievementDialog(AchievementManager* manager, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("🏆 成就榜");
    setMinimumSize(450, 350);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("成就列表（绿色为已解锁）", this));

    m_list = new QListWidget(this);
    for (const QString& id : manager->allIds()) {
        bool unlocked = manager->isUnlocked(id);
        QString text = unlocked
            ? QString("✅ %1 — %2").arg(manager->title(id)).arg(manager->description(id))
            : QString("⬜ %1 — %2").arg(manager->title(id)).arg(manager->description(id));
        QListWidgetItem* item = new QListWidgetItem(text);
        if (unlocked) {
            item->setBackground(QBrush(QColor(200, 255, 200)));
        } else {
            item->setForeground(QBrush(QColor(150, 150, 150)));
        }
        m_list->addItem(item);
    }
    layout->addWidget(m_list);
}
