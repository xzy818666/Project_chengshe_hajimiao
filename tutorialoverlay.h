#ifndef TUTORIALOVERLAY_H
#define TUTORIALOVERLAY_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QTimer>
#include "tutorialmanager.h"

class QGraphicsDropShadowEffect;

/**
 * 教程覆盖层
 * 主界面模式：全屏遮罩+高亮挖洞
 * 对话框模式：只显示气泡，用金色发光效果提示目标控件
 */
class TutorialOverlay : public QWidget
{
    Q_OBJECT
public:
    explicit TutorialOverlay(QWidget *parent = nullptr);
    ~TutorialOverlay();

    void startTutorial();
    void endTutorial();

    void setHighlightRect(const QRect& rect);
    void clearHighlight();

    void updateStep(const TutorialManager::Step* step);

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
    void updateGlowPulse();  // 目标控件发光脉冲

private:
    void setupUI();
    void positionBubble();
    void animateIn();
    QString avatarPath(const QString& expression) const;
    void setWidgetGlow(QWidget* target);   // 给目标控件加金色发光
    void clearWidgetGlow();                // 清除发光

    // 高亮区域（仅主界面模式）
    QRect m_highlightRect;
    bool m_hasHighlight;

    // 模式
    bool m_dialogMode = false;

    // 气泡控件
    QWidget* m_bubble;
    QLabel* m_avatarLabel;
    QLabel* m_speakerLabel;
    QLabel* m_textLabel;
    QPushButton* m_nextBtn;
    QPushButton* m_skipBtn;
    QLabel* m_stepIndicator;
    QLabel* m_arrowLabel;

    // 动画
    QTimer* m_autoTimer;
    QTimer* m_glowTimer;  // 发光脉冲定时器

    // 目标控件发光（对话框模式用）
    QWidget* m_highlightTarget = nullptr;
    QGraphicsDropShadowEffect* m_glowEffect = nullptr;
    int m_glowPhase = 0;

    // 主窗口引用
    QWidget* m_mainWindow = nullptr;

    bool m_tutorialActive;
};

#endif // TUTORIALOVERLAY_H
