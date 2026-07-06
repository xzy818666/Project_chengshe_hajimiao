#ifndef TUTORIALMANAGER_H
#define TUTORIALMANAGER_H

#include <QObject>
#include <QRect>
#include <QString>
#include <QList>
#include <QTimer>

class QDialog;

/**
 * 教程管理器 - 单例状态机
 * 管理内嵌式教程的全局状态，跨界面协调步骤推进
 */
class TutorialManager : public QObject
{
    Q_OBJECT
public:
    static TutorialManager* instance();

    // 教程步骤定义
    struct Step {
        int id;
        QString text;           // 提示文本（支持HTML，字数要少）
        QString speaker;        // 说话人（如"玄机子"）
        QString expression;     // 表情：normal, happy, surprised, serious, worried
        QString targetWidget;   // 目标控件名（用于高亮）
        QString waitFor;        // 等待条件: "none", "click_area", "dialog_open", "dialog_close", "click_cloud"
        QString targetDialog;   // 当waitFor="dialog_open/close"时，目标对话框名
        bool hasArrow;          // 是否显示箭头指向目标
        bool autoAdvance;       // 是否自动进入下一步（无需点击）
        int autoDelayMs;        // 自动推进延迟
    };

    bool isActive() const { return m_active; }
    bool isCompleted() const { return m_completed; }
    int currentStepIndex() const { return m_currentStep; }
    int totalSteps() const { return m_steps.size(); }
    const Step* currentStep() const;
    void startTutorial();
    void endTutorial();
    void nextStep();          // 手动/条件触发推进
    void skipTutorial();

    // 外部通知接口
    void notifyDialogOpened(QDialog* dialog, const QString& dialogName);  // 对话框打开时调用（传入对话框指针）
    void notifyDialogClosed(const QString& dialogName);  // 对话框关闭时调用
    void notifyAreaClicked(const QString& areaName);     // 点击区域时调用（如云、建筑）

signals:
    void stepChanged(int stepIndex);
    void tutorialStarted();
    void tutorialFinished();
    void tutorialSkipped();
    // 对话框教程信号：让 TutorialOverlay 跟随对话框
    void dialogOpened(QDialog* dialog);
    void dialogClosed();

public:
    // 持久化：记录用户是否已完成过教程
    bool hasCompletedBefore() const;
    void markCompleted();

private:
    explicit TutorialManager(QObject *parent = nullptr);
    static TutorialManager* s_instance;

    void buildSteps();

    QList<Step> m_steps;
    int m_currentStep;
    bool m_active;
    bool m_completed;
};

#endif // TUTORIALMANAGER_H
