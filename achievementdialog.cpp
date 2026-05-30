#include "achievementdialog.h"
#include "achievementmanager.h"
#include <QLabel>
#include <QGridLayout>
#include <QPalette>
#include <QPixmap>
#include <QBrush>

AchievementDialog::AchievementDialog(AchievementManager* manager, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("🏆 成就榜");

    // 适配主界面实际尺寸（默认回退 1600×900）
    QSize sz = parent ? parent->size() : QSize(1600, 900);
    resize(sz.width(), sz.height());
    setMinimumSize(800, 450);

    // 背景图按当前窗口尺寸缩放
    QPixmap bg(":/images/achievement_hall.jpeg");
    bg = bg.scaled(sz.width(), sz.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QBrush(bg));
    setPalette(palette);
    setAutoFillBackground(true);

    // 成就固定展示顺序（与 AGENTS.md 一致）
    const QStringList order = {
        "first_step", "click_master", "wall_street", "debt_king",
        "infinite_merit", "rooftop", "diversified", "leverage_gambler"
    };

    // 中央画布容器 —— 对应背景图中央卷轴区域（按主界面比例定位）
    QWidget* canvas = new QWidget(this);
    int canvasX = qRound(sz.width() * 0.34);
    int canvasY = qRound(sz.height() * 0.16);
    int canvasW = qRound(sz.width() * 0.32);
    int canvasH = qRound(sz.height() * 0.69);
    canvas->setGeometry(canvasX, canvasY, canvasW, canvasH);

    QGridLayout* grid = new QGridLayout(canvas);
    grid->setSpacing(14);
    grid->setContentsMargins(24, 32, 24, 32);
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(1, 1);
    for (int i = 0; i < 4; ++i)
        grid->setRowStretch(i, 1);

    int idx = 0;
    for (const QString& id : order) {
        if (!manager->allIds().contains(id))
            continue;

        bool unlocked = manager->isUnlocked(id);
        QString name  = manager->title(id);
        QString desc  = manager->description(id);

        QLabel* label = new QLabel(canvas);
        label->setWordWrap(true);
        label->setAlignment(Qt::AlignCenter);

        if (unlocked) {
            label->setText(
                QString("<b>✅ %1</b><br>"
                        "<span style='font-size:14px;'>%2</span>")
                    .arg(name).arg(desc));
            label->setStyleSheet(
                "color: #1A4A0A;"
                "font-size: 16px;"
                "background-color: rgba(200, 240, 180, 0.55);"
                "border: 1px solid rgba(100, 160, 80, 0.45);"
                "border-radius: 10px;"
                "padding: 10px;");
        } else {
            label->setText(
                QString("<b>⬜ %1</b><br>"
                        "<span style='font-size:14px;'>%2</span>")
                    .arg(name).arg(desc));
            label->setStyleSheet(
                "color: #7A7A7A;"
                "font-size: 16px;"
                "background-color: rgba(235, 230, 215, 0.40);"
                "border: 1px solid rgba(160, 150, 130, 0.30);"
                "border-radius: 10px;"
                "padding: 10px;");
        }

        grid->addWidget(label, idx / 2, idx % 2);
        ++idx;
    }
}
