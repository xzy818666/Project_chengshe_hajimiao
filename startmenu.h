#ifndef STARTMENU_H
#define STARTMENU_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

enum class GameDifficulty {
    Easy,
    Normal,
    Hard
};

struct GameConfig {
    double initialMerit;
    double nextLifePool;
    double inflationRate;
    double creditScore;
    double maxLeverage;
    double maintenanceMarginRate;
};

GameConfig configForDifficulty(GameDifficulty diff);
QString difficultyToString(GameDifficulty diff);

class StartMenu : public QWidget
{
    Q_OBJECT

public:
    explicit StartMenu(QWidget *parent = nullptr);

signals:
    void startGame(GameDifficulty difficulty);

private slots:
    void onStartClicked();

private:
    QLabel *m_titleLabel;
    QComboBox *m_difficultyCombo;
    QPushButton *m_startBtn;
    QLabel *m_descLabel;
};

#endif // STARTMENU_H
