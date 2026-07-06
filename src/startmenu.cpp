#include "startmenu.h"
#include <QPalette>
#include <QBrush>
#include <QPixmap>

GameConfig configForDifficulty(GameDifficulty diff)
{
    switch (diff) {
    case GameDifficulty::Easy:
        return {100000.0, 100000.0, 0.005, 600.0, 3.0, 0.15};
    case GameDifficulty::Hard:
        return {500.0, 500.0, 0.02, 300.0, 1.5, 0.25};
    default:
        return {1000.0, 1000.0, 0.0001, 500.0, 2.0, 0.20};
    }
}

QString difficultyToString(GameDifficulty diff)
{
    switch (diff) {
    case GameDifficulty::Easy: return "easy";
    case GameDifficulty::Hard: return "hard";
    default: return "normal";
    }
}

StartMenu::StartMenu(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("赛博功德银行");
    resize(800, 600);

    // 尝试设置背景图
    QPixmap bg(":/images/main_background.jpg");
    if (!bg.isNull()) {
        QPalette palette;
        palette.setBrush(QPalette::Window, bg.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        setPalette(palette);
    }

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    m_titleLabel = new QLabel("赛博功德银行", this);
    m_titleLabel->setStyleSheet(
        "font-size: 48px; font-weight: bold; color: #FFD700; "
        "font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', serif;"
    );
    m_titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_titleLabel);

    QLabel *subtitle = new QLabel("—— 每一轮都是一次轮回 ——", this);
    subtitle->setStyleSheet("font-size: 18px; color: #E0E0E0;");
    subtitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(subtitle);

    layout->addSpacing(40);

    QLabel *diffLabel = new QLabel("选择轮回难度:", this);
    diffLabel->setStyleSheet("font-size: 16px; color: #FFFFFF;");
    diffLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(diffLabel);

    m_difficultyCombo = new QComboBox(this);
    m_difficultyCombo->addItem("简单 (初始功德 2000，下世储备 2000)", static_cast<int>(GameDifficulty::Easy));
    m_difficultyCombo->addItem("普通 (初始功德 1000，下世储备 1000)", static_cast<int>(GameDifficulty::Normal));
    m_difficultyCombo->addItem("困难 (初始功德 500，下世储备 500)", static_cast<int>(GameDifficulty::Hard));
    m_difficultyCombo->setStyleSheet(
        "QComboBox { font-size: 14px; padding: 6px; background: #2D2D2D; color: #FFD700; border: 1px solid #FFD700; }"
    );
    m_difficultyCombo->setFixedWidth(400);
    layout->addWidget(m_difficultyCombo, 0, Qt::AlignCenter);

    m_descLabel = new QLabel(this);
    m_descLabel->setStyleSheet("font-size: 13px; color: #B0B0B0;");
    m_descLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_descLabel);

    connect(m_difficultyCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&](int index) {
        switch (index) {
        case 0: m_descLabel->setText("适合初入佛门的施主。通胀温和，容错率高。"); break;
        case 1: m_descLabel->setText("标准轮回体验。风险与收益平衡。"); break;
        case 2: m_descLabel->setText("地狱难度。一念天堂，一念地狱。"); break;
        }
    });
    m_difficultyCombo->setCurrentIndex(1);
    m_descLabel->setText("标准轮回体验。风险与收益平衡。");

    layout->addSpacing(30);

    m_startBtn = new QPushButton("开始轮回", this);
    m_startBtn->setFixedSize(200, 50);
    m_startBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #5D4037;"
        "  color: #FFD700;"
        "  font-size: 20px;"
        "  font-weight: bold;"
        "  font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', serif;"
        "  border: 2px solid #FFD700;"
        "  border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #FFD700;"
        "  color: #5D4037;"
        "}"
    );
    layout->addWidget(m_startBtn, 0, Qt::AlignCenter);

    connect(m_startBtn, &QPushButton::clicked, this, &StartMenu::onStartClicked);
}

void StartMenu::onStartClicked()
{
    GameDifficulty diff = static_cast<GameDifficulty>(m_difficultyCombo->currentData().toInt());

    // 内嵌式教程由 MeritHall 首次显示时自动启动，不在此处弹窗
    emit startGame(diff);
}
