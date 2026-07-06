#ifndef TUTORIALOVERLAY_H
#define TUTORIALOVERLAY_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QTimer>
#include <QProgressBar>
#include "tutorialmanager.h"

/**
 * 教程覆盖层 - 绘制遮罩、高亮、箭头、气泡
 * 支持两种模式：
 *   - 主界面模式：覆盖全屏，绘制遮罩+高亮
 *   - 对话框模式：小型气泡，位于对话框底部，不遮挡控件
 */
class TutorialOverlay : public QWidget
{
    Q_OBJECT
public:
    explicit TutorialOverlay(QWidget *parent = nullptr);

    void startTutorial();
    void endTutorial();

    // 设置目标高亮区域（父窗口坐标）
    void setHighlightRect(const QRect& rect);
    void clearHighlight();

    // 更新当前步骤显示
    void updateStep(const TutorialManager::Step* step);

    // 跟随对话框/主窗口切换
    void attachToDialog(QDialog* dialog);
    void detachToMainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onStepChanged(int stepIndex);
    void onNextClicked();
    void onSkipClicked();
    void onAutoAdvance();

private:
    void setupUI();
    void positionBubble();
    void animateIn();
    QString avatarPath(const QString& expression) const;

    // 高亮区域（仅主界面模式有效）
    QRect m_highlightRect;
    bool m_hasHighlight;

    // 模式
    bool m_dialogMode = false;  // true=对话框模式（小气泡，无遮罩）

    // 气泡控件
    QWidget* m_bubble;
    QLabel* m_avatarLabel;      // 头像
    QLabel* m_speakerLabel;
    QLabel* m_textLabel;
    QPushButton* m_nextBtn;
    QPushButton* m_skipBtn;
    QLabel* m_stepIndicator;

    // 箭头标签（仅主界面模式）
    QLabel* m_arrowLabel;

    // 动画
    QPropertyAnimation* m_bubbleAnim;
    QTimer* m_autoTimer;

    // 主窗口引用（用于从对话框恢复）
    QWidget* m_mainWindow = nullptr;

    bool m_tutorialActive;
};

#endif // TUTORIALOVERLAY_H
