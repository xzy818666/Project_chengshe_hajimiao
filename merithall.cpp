#include "merithall.h"
#include "ui_merithall.h"
#include "bankdialog.h"
#include "exchangedialog.h"
#include "shopdialog.h"
#include "achievementdialog.h"
#include <cstdlib>
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QLabel>
#include <QGridLayout>
#include <QWidget>

MeritHall::MeritHall(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MeritHall)
    , m_wallet(nullptr)
    , m_currentInstrument(Instrument::BasicWoodenFish)
    , m_clickCount(0)
    , m_autoIncomePerSec(0)
    , m_currentDate(2026, 1, 1)
    , m_dayTickCounter(0)
{
    ui->setupUi(this);
    setWindowTitle("赛博功德银行 - 功德堂");

    QPixmap background(":/images/main_background.jpg");
    QPalette palette;
    palette.setBrush(QPalette::Window, background.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    setPalette(palette);

    // 创建金鱼发光层（悬停时显示）
    m_fishGlowLayer = new QLabel(this);
    m_fishGlowLayer->setObjectName("fishGlowLayer");
    updateFishGlowPosition();
    m_fishGlowLayer->setStyleSheet(
        "#fishGlowLayer { "
        "background: radial-gradient(circle at 50% 50%, rgba(255, 215, 0, 0.5) 0%, rgba(255, 140, 0, 0.3) 40%, rgba(255, 69, 0, 0.1) 70%, transparent 100%); "
        "border-radius: 50%; "
        "clip-path: circle(50%); "
        "opacity: 0; "
        "}"
    );
    m_fishGlowLayer->show();
    
    // 创建透明点击区域（覆盖金鱼位置）- 带悬停发光效果
    m_fishClickArea = new QPushButton(this);
    m_fishClickArea->setObjectName("fishClickArea");
    updateFishClickAreaPosition(m_fishClickArea);
    m_fishClickArea->setStyleSheet(
        "#fishClickArea { "
        "background: transparent; "
        "border: none; "
        "border-radius: 50%; "
        "} "
        "#fishClickArea:hover { "
        "border: 2px solid rgba(255, 215, 0, 0.5); "
        "border-radius: 50%; "
        "}"
    );
    m_fishClickArea->setCursor(Qt::PointingHandCursor);
    
    // 使用事件过滤器实现悬停发光效果
    m_fishClickArea->installEventFilter(this);

    // 连接点击事件（功能逻辑沿用 main：released 触发点击）
    connect(m_fishClickArea, &QPushButton::pressed, this, &MeritHall::onInstrumentPressed);
    connect(m_fishClickArea, &QPushButton::released, this, &MeritHall::onInstrumentReleased);

    // 隐藏原来的法器相关控件
    ui->instrumentIconLabel->hide();
    ui->instrumentNameLabel->hide();
    ui->instrumentButton->hide();
    ui->feedbackLabel->hide();

    // 创建四栋阁楼按钮
    createPavilionButtons();

    setupEventPopup();

    connect(ui->bankBtn, &QPushButton::clicked, this, &MeritHall::onBankClicked);
    connect(ui->exchangeBtn, &QPushButton::clicked, this, &MeritHall::onExchangeClicked);
    connect(ui->shopBtn, &QPushButton::clicked, this, &MeritHall::onShopClicked);
    connect(ui->yezhangBtn, &QPushButton::clicked, this, &MeritHall::onYezhangClicked);
    connect(ui->achievementBtn, &QPushButton::clicked, this, &MeritHall::onAchievementClicked);

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &MeritHall::updateAutoIncome);
    connect(m_updateTimer, &QTimer::timeout, this, &MeritHall::updateMarketEvent);
    m_updateTimer->start(1000);
}

MeritHall::~MeritHall()
{
    delete ui;
}

void MeritHall::setupEventPopup()
{
    m_eventPopup = new QFrame(this);
    m_eventPopup->setObjectName("eventPopup");
    m_eventPopup->setStyleSheet(
        "QFrame#eventPopup {"
        "  background-color: #FFF3E0;"
        "  border: 2px solid #E65100;"
        "  border-radius: 8px;"
        "}"
    );
    m_eventPopup->setFixedHeight(50);
    m_eventPopup->setGeometry(20, -60, width() - 40, 50);

    QHBoxLayout* popupLayout = new QHBoxLayout(m_eventPopup);
    popupLayout->setContentsMargins(12, 4, 12, 4);

    m_eventPopupLabel = new QLabel(m_eventPopup);
    m_eventPopupLabel->setAlignment(Qt::AlignCenter);
    m_eventPopupLabel->setStyleSheet("color: #BF360C; font-weight: bold; font-size: 14px;");
    popupLayout->addWidget(m_eventPopupLabel);

    m_eventAnim = new QPropertyAnimation(m_eventPopup, "geometry", this);
    m_eventAnim->setDuration(400);
    m_eventAnim->setEasingCurve(QEasingCurve::OutCubic);

    m_eventHideTimer = new QTimer(this);
    m_eventHideTimer->setSingleShot(true);
    connect(m_eventHideTimer, &QTimer::timeout, this, &MeritHall::hideEventPopup);
}

void MeritHall::showEventPopup(const QString& text)
{
    if (!m_eventPopup) return;

    m_eventPopupLabel->setText(text);
    m_eventPopup->raise();

    QRect startRect(20, -60, width() - 40, 50);
    QRect endRect(20, 10, width() - 40, 50);

    m_eventAnim->setStartValue(startRect);
    m_eventAnim->setEndValue(endRect);
    m_eventAnim->start();

    m_eventHideTimer->stop();
    m_eventHideTimer->start(6000);
}

void MeritHall::hideEventPopup()
{
    if (!m_eventPopup) return;

    QRect startRect(20, 10, width() - 40, 50);
    QRect endRect(20, -60, width() - 40, 50);

    m_eventAnim->setStartValue(startRect);
    m_eventAnim->setEndValue(endRect);
    m_eventAnim->start();
}

void MeritHall::setWallet(Wallet* wallet)
{
    m_wallet = wallet;
    connect(m_wallet, &Wallet::meritChanged, this, &MeritHall::updateHUD);
    connect(m_wallet, &Wallet::debtChanged, this, &MeritHall::updateHUD);
    connect(m_wallet, &Wallet::yezhangChanged, this, &MeritHall::updateHUD);
    connect(m_wallet, &Wallet::assetsChanged, this, &MeritHall::updateHUD);
    updateHUD();
}

void MeritHall::setAssets(QList<Asset*> assets)
{
    m_assets = assets;
}

void MeritHall::setMarketEvent(MarketEvent* marketEvent)
{
    m_marketEvent = marketEvent;
}

void MeritHall::setAchievementManager(AchievementManager* manager)
{
    m_achievementManager = manager;
}

void MeritHall::onInstrumentPressed()
{
    // 保留 HEAD 的法器点击发光动画效果
    QLabel *rewardLabel = new QLabel(this);
    rewardLabel->setText("+2");
    rewardLabel->setStyleSheet(
        "QLabel { "
        "color: #FFD700; "
        "font-size: 72px; "
        "font-weight: bold; "
        "font-family: 'STKaiti', 'STXingkai', 'LiSu', 'KaiTi', serif; "
        "background: transparent; "
        "text-shadow: 0 0 10px #FFD700, 0 0 20px #FFD700, 0 0 30px #FFD700, "
        "             0 0 40px #FF8C00, 0 0 50px #FF8C00, 0 0 60px #FF4500; "
        "}"
    );
    
    // 设置在金鱼下方显示，从下方飘出
    rewardLabel->adjustSize();
    rewardLabel->move(380, 380);
    rewardLabel->raise();
    rewardLabel->show();
    
    // 1.5秒后消失
    QTimer::singleShot(1500, rewardLabel, &QLabel::deleteLater);
    
    // 向上浮动并放大的夸张动画
    QPropertyAnimation *posAnim = new QPropertyAnimation(rewardLabel, "pos");
    posAnim->setDuration(1500);
    posAnim->setStartValue(QPoint(380, 380));
    posAnim->setEndValue(QPoint(380, 50));
    
    // 添加缩放动画
    QPropertyAnimation *scaleAnim = new QPropertyAnimation(rewardLabel, "scale");
    scaleAnim->setDuration(1500);
    scaleAnim->setStartValue(0.5);  // 从0.5倍开始
    scaleAnim->setKeyValueAt(0.2, 1.5);  // 快速放大到1.5倍
    scaleAnim->setKeyValueAt(0.5, 1.2);  // 稍微缩小
    scaleAnim->setEndValue(0.8);  // 最终缩小并消失
    
    // 添加透明度动画
    QPropertyAnimation *opacityAnim = new QPropertyAnimation(rewardLabel, "windowOpacity");
    opacityAnim->setDuration(1500);
    opacityAnim->setStartValue(1.0);
    opacityAnim->setKeyValueAt(0.7, 1.0);
    opacityAnim->setEndValue(0.0);  // 最后淡出
    
    // 启动所有动画
    posAnim->start(QPropertyAnimation::DeleteWhenStopped);
    scaleAnim->start(QPropertyAnimation::DeleteWhenStopped);
    opacityAnim->start(QPropertyAnimation::DeleteWhenStopped);
}

void MeritHall::onInstrumentReleased()
{
    onInstrumentClicked();
}

void MeritHall::onInstrumentClicked()
{
    if (!m_wallet) return;

    m_clickCount++;
    double reward = m_currentInstrument.clickReward();

    if (m_currentInstrument.hasCrit() && (rand() / (double)RAND_MAX) < m_currentInstrument.critChance()) {
        reward *= 10;
        ui->feedbackLabel->setText(QString("功德 +%1 (暴击!)").arg(reward));
    } else {
        ui->feedbackLabel->setText(QString("功德 +%1").arg(reward));
    }

    double efficiency = m_wallet->efficiency();
    m_wallet->earn(reward * efficiency);

    if (m_achievementManager) {
        m_achievementManager->onClickReport(m_clickCount);
    }

    QTimer::singleShot(500, [this]() {
        ui->feedbackLabel->setText("");
    });

    updateHUD();
}

void MeritHall::updateAutoIncome()
{
    if (!m_wallet) return;

    // 推进游戏日期：每 30 秒为一天
    m_dayTickCounter++;
    if (m_dayTickCounter >= 1) {
        m_dayTickCounter = 0;
        m_currentDate = m_currentDate.addDays(1);
        updateDateDisplay();
    }

    double autoReward = m_currentInstrument.autoReward();
    double maintenance = m_currentInstrument.maintenanceCost();
    double netIncome = autoReward - maintenance;

    if (netIncome > 0) {
        m_wallet->earn(netIncome);
        m_autoIncomePerSec = netIncome;
    } else if (maintenance > 0 && m_wallet->merit() >= maintenance) {
        m_wallet->spend(maintenance);
        m_autoIncomePerSec = -maintenance;
    }

    updateHUD();
}

void MeritHall::updateHUD()
{
    if (!m_wallet) return;

    ui->meritLabel->setText(QString("功德: %1").arg(m_wallet->merit(), 0, 'f', 2));

    double assetValue = 0;
    for (Asset* asset : m_assets) {
        assetValue += m_wallet->getAssetShares(asset->id()) * asset->price();
    }
    ui->assetLabel->setText(QString("资产市值: %1").arg(assetValue, 0, 'f', 2));

    double netWorth = m_wallet->merit() + assetValue + m_wallet->savingsBalance() - m_wallet->totalDebt();
    ui->networthLabel->setText(QString("净资产: %1").arg(netWorth, 0, 'f', 2));

    double yezhang = m_wallet->yezhang();
    ui->yezhangLabel->setText(QString("业障: %1").arg(yezhang, 0, 'f', 0));
    ui->yezhangLabel->setStyleSheet("color: #EF5350; font-weight: bold;");

    ui->clickCountLabel->setText(QString("今日点击: %1").arg(m_clickCount));
    ui->autoIncomeLabel->setText(QString("自动收益: %1/秒").arg(m_autoIncomePerSec, 0, 'f', 1));
    ui->efficiencyLabel->setText(QString("效率系数: %1%").arg(m_wallet->efficiency() * 100, 0, 'f', 1));
    ui->inflationLabel->setText(QString("通胀: %1%/日").arg(m_wallet->dailyInflationRate() * 100, 0, 'f', 2));
    if (m_wallet->dailyInflationRate() > 0.02) {
        ui->inflationLabel->setStyleSheet("color: #E65100; font-weight: bold;");
    } else if (m_wallet->dailyInflationRate() > 0.005) {
        ui->inflationLabel->setStyleSheet("color: #F57C00;");
    } else {
        ui->inflationLabel->setStyleSheet("color: #B0B0B0;");
    }

    if (m_achievementManager) {
        m_achievementManager->onMeritChanged(m_wallet->merit());
        double assetValue = 0;
        QMap<QString, double> prices;
        for (Asset* asset : m_assets) {
            double p = asset->price();
            prices[asset->id()] = p;
            assetValue += m_wallet->getAssetShares(asset->id()) * p;
        }
        double netWorth = m_wallet->merit() + assetValue + m_wallet->savingsBalance() - m_wallet->totalDebt();
        m_achievementManager->onNetWorthChanged(netWorth);
        m_achievementManager->checkHoldings(m_wallet->assets());
        m_achievementManager->checkPortfolioLoss(m_wallet->assets(), prices);
    }
}

void MeritHall::updateDateDisplay()
{
    ui->dateLabel->setText(m_currentDate.toString("yyyy年MM月dd日"));
}

void MeritHall::updateMarketEvent()
{
    QString eventText;
    bool hasEvent = false;

    if (m_marketEvent && m_marketEvent->isActive()) {
        eventText = m_marketEvent->currentEvent();
        hasEvent = true;
    }

    if (hasEvent) {
        if (eventText != m_lastEventText) {
            m_lastEventText = eventText;
            showEventPopup(eventText);
        }
    } else {
        if (!m_lastEventText.isEmpty()) {
            m_lastEventText.clear();
            hideEventPopup();
        }
    }
}

void MeritHall::onBankClicked()
{
    BankDialog dialog(this);
    dialog.setWallet(m_wallet);
    dialog.exec();
    updateHUD();
}

void MeritHall::onExchangeClicked()
{
    ExchangeDialog dialog(this);
    dialog.setWallet(m_wallet);
    dialog.setAssets(m_assets);
    dialog.setMarketEvent(m_marketEvent);
    dialog.exec();
    updateHUD();
}

void MeritHall::onShopClicked()
{
    ShopDialog dialog(this);
    dialog.setWallet(m_wallet);
    connect(&dialog, &ShopDialog::instrumentChanged, [this](const Instrument& instrument) {
        m_currentInstrument = instrument;
        ui->instrumentNameLabel->setText(m_currentInstrument.name());
        updateInstrumentIcon();
    });
    dialog.exec();
    updateHUD();
}

void MeritHall::updateInstrumentIcon()
{
    QPixmap icon(":/images/icon.jpg");
    if (!icon.isNull()) {
        ui->instrumentIconLabel->setPixmap(icon.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        ui->instrumentIconLabel->setText("🪵");
        ui->instrumentIconLabel->setStyleSheet("font-size: 64px;");
    }
}

void MeritHall::onYezhangClicked()
{
    if (!m_wallet) return;
    if (m_wallet->yezhang() > 0) {
        double amount = qMin(100.0, m_wallet->yezhang());
        m_wallet->reduceYezhang(amount);
        updateHUD();
    }
}

void MeritHall::createPavilionButtons()
{
    pavilionBankBtn = new QPushButton("善财司", this);
    pavilionBankBtn->setCursor(Qt::PointingHandCursor);
    connect(pavilionBankBtn, &QPushButton::clicked, this, &MeritHall::onBankClicked);

    pavilionExchangeBtn = new QPushButton("幻缘所", this);
    pavilionExchangeBtn->setCursor(Qt::PointingHandCursor);
    connect(pavilionExchangeBtn, &QPushButton::clicked, this, &MeritHall::onExchangeClicked);

    pavilionShopBtn = new QPushButton("法器阁", this);
    pavilionShopBtn->setCursor(Qt::PointingHandCursor);
    connect(pavilionShopBtn, &QPushButton::clicked, this, &MeritHall::onShopClicked);

    pavilionAchievementBtn = new QPushButton("成就", this);
    pavilionAchievementBtn->setCursor(Qt::PointingHandCursor);
    connect(pavilionAchievementBtn, &QPushButton::clicked, this, &MeritHall::onAchievementClicked);

    // 初始化按钮位置和样式（响应式）
    updatePavilionPositions();

    // 隐藏原来的底部导航按钮
    ui->bankBtn->hide();
    ui->exchangeBtn->hide();
    ui->shopBtn->hide();
    ui->yezhangBtn->hide();
    ui->achievementBtn->hide();
}

void MeritHall::updatePavilionPositions()
{
    int w = this->width();
    int h = this->height();
    
    // 按钮尺寸根据窗口大小自适应
    int btnWidth = qMax(100, w / 6);   // 最小100px，最大约1/6窗口宽度
    int btnHeight = qMax(35, h / 20);  // 最小35px，最大约1/20窗口高度
    int fontSize = qMax(18, qMin(32, w / 28)); // 字体大小18-32px
    
    QString pavilionStyle = 
        QString("QPushButton { "
                "background: transparent; "
                "color: #FFD700; "
                "font-size: %1px; "
                "font-weight: bold; "
                "font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', serif; "
                "border: none; "
                "text-shadow: 0 0 10px #FFD700, 0 0 20px #FFD700, 0 0 30px #FFD700, 0 0 40px #FFD700; "
                "} "
                "QPushButton:hover { "
                "color: #FFFFFF; "
                "text-shadow: 0 0 15px #FFFFFF, 0 0 30px #FFFFFF, 0 0 45px #FFFFFF, 0 0 60px #FFFFFF; "
                "}").arg(fontSize);
    
    pavilionBankBtn->setStyleSheet(pavilionStyle);
    pavilionExchangeBtn->setStyleSheet(pavilionStyle);
    pavilionShopBtn->setStyleSheet(pavilionStyle);
    pavilionAchievementBtn->setStyleSheet(pavilionStyle);
    
    // 四个阁楼门的位置比例（可调整）
    pavilionBankBtn->setGeometry(w * 0.25 - btnWidth/2, h * 0.38 - btnHeight/2, btnWidth, btnHeight);
    pavilionExchangeBtn->setGeometry(w * 0.76 - btnWidth/2, h * 0.38 - btnHeight/2, btnWidth, btnHeight);
    pavilionShopBtn->setGeometry(w * 0.18 - btnWidth/2, h * 0.66 - btnHeight/2, btnWidth, btnHeight);
    pavilionAchievementBtn->setGeometry(w * 0.81 - btnWidth/2, h * 0.66 - btnHeight/2, btnWidth, btnHeight);
}

void MeritHall::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updatePavilionPositions();
    updateFishGlowPosition();
    updateFishClickAreaPosition(m_fishClickArea);
}

void MeritHall::updateFishGlowPosition()
{
    int w = this->width();
    int h = this->height();
    int size = qMax(128, static_cast<int>(qMin(w / 3, h / 3) ));
    m_fishGlowLayer->setGeometry(w/2 - size/2, h/2 - size/2, size, size);
}

void MeritHall::updateFishClickAreaPosition(QPushButton *btn)
{
    int w = this->width();
    int h = this->height();
    int size = qMax(128, static_cast<int>(qMin(w / 3, h / 3) )); 
    btn->setGeometry(w/2 - size/2, h/2 - size/2, size, size);
}

bool MeritHall::eventFilter(QObject *obj, QEvent *event)
{
    if (obj->objectName() == "fishClickArea") {
        if (event->type() == QEvent::Enter) {
            // 鼠标进入时显示发光效果
            m_fishGlowLayer->setStyleSheet(
                "#fishGlowLayer { "
                "background: radial-gradient(circle, rgba(255, 215, 0, 0.5) 0%, rgba(255, 140, 0, 0.3) 30%, rgba(255, 69, 0, 0.1) 60%, transparent 80%); "
                "border-radius: 150px; "
                "opacity: 1; "
                "}"
            );
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开时隐藏发光效果
            m_fishGlowLayer->setStyleSheet(
                "#fishGlowLayer { "
                "background: radial-gradient(circle, rgba(255, 215, 0, 0.5) 0%, rgba(255, 140, 0, 0.3) 30%, rgba(255, 69, 0, 0.1) 60%, transparent 80%); "
                "border-radius: 150px; "
                "opacity: 0; "
                "}"
            );
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MeritHall::onAchievementClicked()
{
    if (!m_achievementManager) return;
    AchievementDialog dialog(m_achievementManager, this);
    dialog.exec();
}
