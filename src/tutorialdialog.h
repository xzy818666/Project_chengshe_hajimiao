#ifndef TUTORIALDIALOG_H
#define TUTORIALDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QProgressBar>

/**
 * 新手教程对话框
 * 角色：赛博引路人·玄机子
 * 风格：古风 + 现代梗
 */
class TutorialDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TutorialDialog(QWidget *parent = nullptr);

private slots:
    void nextStep();
    void prevStep();
    void skipTutorial();

private:
    void setupUI();
    void createStep(int index, const QString& title, const QString& content,
                    const QString& tip, const QString& icon);
    void updateStepDisplay();

    QStackedWidget* m_stack;
    QPushButton* m_nextBtn;
    QPushButton* m_prevBtn;
    QPushButton* m_skipBtn;
    QLabel* m_stepIndicator;
    QProgressBar* m_progressBar;

    int m_currentStep;
    static const int TOTAL_STEPS = 7;
};

#endif // TUTORIALDIALOG_H
