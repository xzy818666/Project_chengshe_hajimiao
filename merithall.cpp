#include "merithall.h"
#include "ui_merithall.h"
#include "bankdialog.h"
#include "exchangedialog.h"
#include "shopdialog.h"
#include "achievementdialog.h"
#include "samsarafutures.h"
#include "savemanager.h"
#include <cstdlib>
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QLabel>
#include <QGridLayout>
#include <QWidget>
#include <QFrame>
#include <QMessageBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QProgressBar>

MeritHall::MeritHall(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MeritHall)
    , m_wallet(nullptr)
    , m_cloudInstrument(Instrument::BasicWoodenFish)
    , m_clickCount(0)
    , m_autoIncomePerSec(0)
    , m_currentDate(2026, 1, 1)
    , m_dayTickCounter(0)
    , m_resonance(new InstrumentResonance(this))
{
    ui->setupUi(this);
    setWindowTitle("赛博功德银行 - 功德堂");

    // HUD 适配仙宫明亮风格：半透明深色底 + 金色文字
    QString hudStyle =
        "background-color: rgba(30, 40, 60, 0.65); "
        "border: 1px solid rgba(255, 215, 100, 0.4); "
        "border-radius: 10px; "
        "padding: 4px;";
    ui->hudWidget->setStyleSheet(hudStyle);
    ui->rightHudWidget->setStyleSheet(hudStyle);

    setAutoFillBackground(true);

    // 创建云海发光层（悬停时显示）
    m_fishGlowLayer = new QLabel(this);
    m_fishGlowLayer->setObjectName("fishGlowLayer");
    updateFishGlowPosition();
    m_fishGlowLayer->setStyleSheet(
        "#fishGlowLayer { "
        "background: radial-gradient(circle at 50% 50%, rgba(255, 255, 255, 0.6) 0%, rgba(200, 230, 255, 0.4) 40%, rgba(150, 200, 255, 0.15) 70%, transparent 100%); "
        "border-radius: 50%; "
        "clip-path: circle(50%); "
        "opacity: 0; "
        "}"
    );
    m_fishGlowLayer->show();

    // 木鱼图标暂不显示（用户要求不加木鱼图层）
    m_fishIconLabel = new QLabel(this);
    m_fishIconLabel->setObjectName("fishIconLabel");
    m_fishIconLabel->setPixmap(QPixmap(":/images/wooden_fish.png"));
    m_fishIconLabel->setScaledContents(true);
    updateFishIconPosition();
    m_fishIconLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_fishIconLabel->hide();

    // 创建透明点击区域（覆盖木鱼位置）- 带悬停发光效果
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
        "border: 2px solid rgba(255, 255, 255, 0.6); "
        "border-radius: 50%; "
        "}"
    );
    m_fishClickArea->setCursor(Qt::PointingHandCursor);
    
    // 使用事件过滤器实现悬停发光效果
    m_fishClickArea->installEventFilter(this);

    // 连接点击事件（功能逻辑沿用 main：released 触发点击）
    connect(m_fishClickArea, &QPushButton::pressed, this, &MeritHall::onInstrumentPressed);
    connect(m_fishClickArea, &QPushButton::released, this, &MeritHall::onInstrumentReleased);

    // 创建云上法器贴图标签（初始显示基础木鱼）
    m_cloudInstrumentLabel = new QLabel(this);
    m_cloudInstrumentLabel->setObjectName("cloudInstrumentLabel");
    m_cloudInstrumentLabel->setScaledContents(true);
    m_cloudInstrumentLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    updateCloudInstrumentPosition();
    m_cloudInstrumentLabel->raise();
    m_cloudInstrumentLabel->show();

    // 初始化法器显示
    updateCloudInstrumentDisplay();
    updateLotusInstrumentDisplay();

    // 隐藏原来的法器相关控件和默认事件标签
    ui->instrumentIconLabel->hide();
    ui->instrumentNameLabel->hide();
    ui->instrumentButton->hide();
    ui->feedbackLabel->hide();
    ui->eventLabel->hide();

    // 创建建筑门口光芒层（hover 时从门内射出仙光）
    m_bankGlowLabel = createDoorGlowLabel();
    m_exchangeGlowLabel = createDoorGlowLabel();
    m_shopGlowLabel = createDoorGlowLabel();
    m_achievementGlowLabel = createDoorGlowLabel();

    // 创建四栋阁楼按钮
    createPavilionButtons();
    createEndSamsaraButton();

    setupEventPopup();
    setupCultivationDisplay();

    // 连接共鸣信号
    connect(m_resonance, &InstrumentResonance::resonanceActivated,
            this, &MeritHall::showResonancePopup);

    connect(ui->bankBtn, &QPushButton::clicked, this, &MeritHall::onBankClicked);
    connect(ui->exchangeBtn, &QPushButton::clicked, this, &MeritHall::onExchangeClicked);
    connect(ui->shopBtn, &QPushButton::clicked, this, &MeritHall::onShopClicked);
    connect(ui->yezhangBtn, &QPushButton::clicked, this, &MeritHall::onYezhangClicked);
    connect(ui->achievementBtn, &QPushButton::clicked, this, &MeritHall::onAchievementClicked);

    // 适配背景图 16:9 比例
    resize(1600, 900);
    setMinimumSize(800, 450);

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &MeritHall::updateAutoIncome);
    connect(m_updateTimer, &QTimer::timeout, this, &MeritHall::updateMarketEvent);
    m_updateTimer->start(1000);

    // 初始化教程系统
    setupTutorial();
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
        "  background-color: rgba(255, 250, 240, 0.92);"
        "  border: 2px solid rgba(200, 160, 80, 0.8);"
        "  border-radius: 8px;"
        "}"
    );
    m_eventPopup->setFixedHeight(50);
    m_eventPopup->setGeometry(20, -60, width() - 40, 50);

    QHBoxLayout* popupLayout = new QHBoxLayout(m_eventPopup);
    popupLayout->setContentsMargins(12, 4, 12, 4);

    m_eventPopupLabel = new QLabel(m_eventPopup);
    m_eventPopupLabel->setAlignment(Qt::AlignCenter);
    m_eventPopupLabel->setStyleSheet("color: #8B4513; font-weight: bold; font-size: 14px;");
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

    m_eventHideTimer->stop();

    // 如果已经在隐藏状态，无需重复动画
    if (m_eventPopup->geometry().y() <= -60) {
        return;
    }

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

void MeritHall::setGameTimer(QTimer* timer)
{
    m_gameTimer = timer;
}

void MeritHall::setLeverageParams(double maxLeverage, double maintenanceMarginRate)
{
    m_maxLeverage = maxLeverage;
    m_maintenanceMarginRate = maintenanceMarginRate;
}

void MeritHall::onInstrumentPressed()
{
    // 云上法器切换到敲击状态贴图
    QString path = m_cloudInstrument.strikedImagePath();
    if (!path.isEmpty()) {
        m_cloudInstrumentLabel->setPixmap(QPixmap(path));
    }
}

void MeritHall::onInstrumentReleased()
{
    // 云上法器恢复未敲击状态贴图
    QString path = m_cloudInstrument.unstrikedImagePath();
    if (!path.isEmpty()) {
        m_cloudInstrumentLabel->setPixmap(QPixmap(path));
    }

    onInstrumentClicked();
}

void MeritHall::onInstrumentClicked()
{
    if (!m_wallet) return;

    // 通知教程系统：用户点击了云海
    TutorialManager::instance()->notifyAreaClicked("fishClickArea");

    m_clickCount++;
    double reward = m_cloudInstrument.clickReward() + totalLotusClickBonus();
    double efficiency = m_wallet->efficiency() + totalLotusEfficiencyBonus();

    // 修炼等级效率加成
    if (m_wallet->cultivationLevel()) {
        efficiency += m_wallet->cultivationLevel()->efficiencyBonus();
    }
    // 法器共鸣效率加成
    if (m_resonance) {
        efficiency += m_resonance->totalEfficiencyBonus();
    }
    efficiency = qMin(efficiency, 2.0); // 效率上限提升至 200%

    // 法器共鸣点击倍率
    double clickMult = 1.0;
    if (m_resonance) {
        clickMult = m_resonance->totalClickMultiplier();
    }
    double actualReward = reward * efficiency * clickMult;
    bool isCrit = false;

    double totalCritChance = m_cloudInstrument.critChance() + totalLotusCritBonus();
    // 修炼等级暴击率加成
    if (m_wallet->cultivationLevel()) {
        totalCritChance += m_wallet->cultivationLevel()->critRateBonus();
    }
    // 法器共鸣暴击率加成
    if (m_resonance) {
        totalCritChance += m_resonance->totalCritRateBonus();
    }
    if (totalCritChance > 0 && (rand() / (double)RAND_MAX) < totalCritChance) {
        int multiplier = m_cloudInstrument.critMultiplier();
        // 法器共鸣暴击倍数加成
        if (m_resonance) {
            multiplier += m_resonance->totalCritMultiplierBonus();
        }
        if (multiplier > 1) {
            actualReward *= multiplier;
            isCrit = true;
        }
    }

    m_wallet->earn(actualReward);

    // 获取修炼经验：基础1点，暴击额外5点
    int expGain = 1;
    if (isCrit) expGain += 5;
    m_wallet->addCultivationExp(expGain);

    // 更新法器组合共鸣
    updateResonance();

    // 浮动数字动画（移到点击收益计算后，显示实际数值）
    QLabel *rewardLabel = new QLabel(this);
    if (isCrit) {
        rewardLabel->setText(QString("功德 +%1 (暴击!)").arg(actualReward, 0, 'f', 1));
        rewardLabel->setStyleSheet(
            "QLabel { "
            "color: #FF4500; "
            "font-size: 72px; "
            "font-weight: bold; "
            "font-family: 'STKaiti', 'STXingkai', 'LiSu', 'KaiTi', serif; "
            "background: transparent; "
            "text-shadow: 0 0 10px #FF4500, 0 0 20px #FF8C00, 0 0 30px #FFD700; "
            "}"
        );
    } else {
        rewardLabel->setText(QString("功德 +%1").arg(actualReward, 0, 'f', 1));
        rewardLabel->setStyleSheet(
            "QLabel { "
            "color: #FFD700; "
            "font-size: 36px; "
            "font-weight: bold; "
            "font-family: 'STKaiti', 'STXingkai', 'LiSu', 'KaiTi', serif; "
            "background: transparent; "
            "text-shadow: 0 0 10px #FFD700, 0 0 20px #FFD700, 0 0 30px #FFD700, "
            "             0 0 40px #FF8C00, 0 0 50px #FF8C00, 0 0 60px #FF4500; "
            "}"
        );
    }

    rewardLabel->adjustSize();

    // 根据金鱼位置动态居中计算起点和终点
    int w = this->width();
    int h = this->height();
    int size = qMax(128, static_cast<int>(qMin(w / 3, h / 3)));
    int endY = 50; // 原结束位置
    int startY = qMax(h / 2 - size / 2 - 20, endY + 150); // 金鱼上方一点，但保证足够上升空间
    int centerX = w / 2 - rewardLabel->width() / 2;

    rewardLabel->move(centerX, startY);
    rewardLabel->raise();
    rewardLabel->show();

    QTimer::singleShot(1500, rewardLabel, &QLabel::deleteLater);

    QPropertyAnimation *posAnim = new QPropertyAnimation(rewardLabel, "pos");
    posAnim->setDuration(1500);
    posAnim->setStartValue(QPoint(centerX, startY));
    posAnim->setEndValue(QPoint(centerX, endY));

    QPropertyAnimation *scaleAnim = new QPropertyAnimation(rewardLabel, "scale");
    scaleAnim->setDuration(1500);
    scaleAnim->setStartValue(0.5);
    scaleAnim->setKeyValueAt(0.2, 1.5);
    scaleAnim->setKeyValueAt(0.5, 1.2);
    scaleAnim->setEndValue(0.8);

    QPropertyAnimation *opacityAnim = new QPropertyAnimation(rewardLabel, "windowOpacity");
    opacityAnim->setDuration(1500);
    opacityAnim->setStartValue(1.0);
    opacityAnim->setKeyValueAt(0.7, 1.0);
    opacityAnim->setEndValue(0.0);

    posAnim->start(QPropertyAnimation::DeleteWhenStopped);
    scaleAnim->start(QPropertyAnimation::DeleteWhenStopped);
    opacityAnim->start(QPropertyAnimation::DeleteWhenStopped);

    if (m_achievementManager) {
        m_achievementManager->onClickReport(m_clickCount);
    }

    updateHUD();
}

void MeritHall::updateAutoIncome()
{
    if (!m_wallet) return;

    checkSamsaraLiquidation();
    checkLeverageMargin();

    // 推进游戏日期：每 30 秒为一天
    m_dayTickCounter++;
    if (m_dayTickCounter >= 1) {
        m_dayTickCounter = 0;
        m_currentDate = m_currentDate.addDays(1);
        updateDateDisplay();
    }

    // 更新烧香法器倒计时
    updateLotusDurations();

    // 累加所有辅助法器的自动收益与维护费
    double totalAutoReward = 0.0;
    double totalMaintenance = 0.0;
    for (const Instrument& inst : m_lotusInstruments) {
        totalAutoReward += inst.autoReward();
        totalMaintenance += inst.maintenanceCost();
    }

    // 法器组合共鸣加成
    if (m_resonance) {
        totalAutoReward += m_resonance->totalAutoIncomeBonus();
    }

    // 修炼等级自动收益加成
    double cultivationBonus = 1.0;
    if (m_wallet->cultivationLevel()) {
        cultivationBonus += m_wallet->cultivationLevel()->autoIncomeBonus();
    }
    totalAutoReward *= cultivationBonus;

    double netIncome = totalAutoReward - totalMaintenance;

    if (netIncome > 0) {
        m_wallet->earn(netIncome);
        m_autoIncomePerSec = netIncome;
    } else if (totalMaintenance > 0 && m_wallet->merit() >= totalMaintenance) {
        m_wallet->spend(totalMaintenance);
        m_autoIncomePerSec = -totalMaintenance;
    }

    // 每日记录信用活跃度（每 30 秒 tick 一次，约每天一次）
    if (m_dayTickCounter == 0 && m_wallet->creditRating()) {
        m_wallet->creditRating()->recordDailyActivity();
    }

    // 业障影响信用等级
    if (m_wallet->creditRating()) {
        m_wallet->creditRating()->recordYezhangImpact(static_cast<int>(m_wallet->yezhang()));
    }

    updateHUD();
}

void MeritHall::updateHUD()
{
    if (!m_wallet) return;

    ui->meritLabel->setText(QString("功德: %1").arg(m_wallet->merit(), 0, 'f', 2));
    ui->meritLabel->setStyleSheet("color: #FFD700; font-weight: bold; font-size: 15px;");

    double assetValue = 0;
    for (Asset* asset : m_assets) {
        assetValue += m_wallet->getAssetShares(asset->id()) * asset->price();
    }
    ui->assetLabel->setText(QString("资产市值: %1").arg(assetValue, 0, 'f', 2));
    ui->assetLabel->setStyleSheet("color: #A5D6A7; font-weight: bold; font-size: 15px;");

    double netWorth = m_wallet->merit() + assetValue + m_wallet->savingsBalance() - m_wallet->totalDebt();
    ui->networthLabel->setText(QString("净资产: %1").arg(netWorth, 0, 'f', 2));
    ui->networthLabel->setStyleSheet("color: #81D4FA; font-weight: bold; font-size: 15px;");

    double yezhang = m_wallet->yezhang();
    ui->yezhangLabel->setText(QString("业障: %1").arg(yezhang, 0, 'f', 0));
    ui->yezhangLabel->setStyleSheet("color: #FF8A80; font-weight: bold; font-size: 15px;");

    ui->dateLabel->setStyleSheet("color: #FFF8E1; font-weight: bold; font-size: 15px;");

    ui->clickCountLabel->setText(QString("今日点击: %1").arg(m_clickCount));
    ui->clickCountLabel->setStyleSheet("color: #E0E0E0; font-size: 13px;");
    ui->autoIncomeLabel->setText(QString("自动收益: %1/秒").arg(m_autoIncomePerSec, 0, 'f', 1));
    ui->autoIncomeLabel->setStyleSheet("color: #E0E0E0; font-size: 13px;");
    ui->efficiencyLabel->setText(QString("效率系数: %1%").arg((m_wallet->efficiency() + totalLotusEfficiencyBonus()) * 100, 0, 'f', 1));
    ui->efficiencyLabel->setStyleSheet("color: #E0E0E0; font-size: 13px;");
    ui->inflationLabel->setText(QString("通胀: %1%/日").arg(m_wallet->dailyInflationRate() * 100, 0, 'f', 2));
    if (m_wallet->dailyInflationRate() > 0.02) {
        ui->inflationLabel->setStyleSheet("color: #FFAB91; font-weight: bold; font-size: 13px;");
    } else if (m_wallet->dailyInflationRate() > 0.005) {
        ui->inflationLabel->setStyleSheet("color: #FFCC80; font-size: 13px;");
    } else {
        ui->inflationLabel->setStyleSheet("color: #B0BEC5; font-size: 13px;");
    }

    // 显示下世功德池
    QLabel *nextLifeLabel = findChild<QLabel*>("nextLifeLabel");
    if (!nextLifeLabel) {
        nextLifeLabel = new QLabel(this);
        nextLifeLabel->setObjectName("nextLifeLabel");
        nextLifeLabel->setAlignment(Qt::AlignCenter);
        // 插入到 rightHudLayout 第2行
        ui->rightHudLayout->addWidget(nextLifeLabel, 2, 0, 1, 2);
    }
    nextLifeLabel->setText(QString("下世储备: %1 | 已消耗: %2")
        .arg(m_wallet->nextLifeMeritPool(), 0, 'f', 0)
        .arg(m_wallet->nextLifeLoss(), 0, 'f', 0));
    nextLifeLabel->setStyleSheet("color: #FFB74D; font-weight: bold; font-size: 13px;");

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

    // 更新修炼等级显示
    updateCultivationDisplay();

    // 更新共鸣显示（在 HUD 中显示当前共鸣）
    QLabel *resonanceLabel = findChild<QLabel*>("resonanceLabel");
    if (!resonanceLabel) {
        resonanceLabel = new QLabel(this);
        resonanceLabel->setObjectName("resonanceLabel");
        resonanceLabel->setStyleSheet(
            "color: #E1BEE7; font-size: 13px; font-weight: bold;"
            "font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
        );
        resonanceLabel->setAlignment(Qt::AlignCenter);
        ui->rightHudLayout->addWidget(resonanceLabel, 3, 0, 1, 2);
    }
    if (m_resonance) {
        resonanceLabel->setText(m_resonance->getActiveResonanceText());
    }
}

void MeritHall::updateDateDisplay()
{
    ui->dateLabel->setText(m_currentDate.toString("yyyy年MM月dd日"));
}

void MeritHall::updateMarketEvent()
{
    // 传递业障值给市场事件系统
    if (m_marketEvent && m_wallet) {
        m_marketEvent->setYezhang(m_wallet->yezhang());
    }

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
    // 通知教程系统：用户点击了善财司建筑
    TutorialManager::instance()->notifyAreaClicked("pavilionBankBtn");
    BankDialog dialog(this);
    dialog.setWallet(m_wallet);
    dialog.setAssets(m_assets);
    TutorialManager::instance()->notifyDialogOpened(&dialog, "BankDialog");
    dialog.exec();

    TutorialManager::instance()->notifyDialogClosed("BankDialog");
    checkTutorialOnShow();
    updateHUD();
}

void MeritHall::onExchangeClicked()
{
    TutorialManager::instance()->notifyAreaClicked("pavilionExchangeBtn");
    ExchangeDialog dialog(this);
    dialog.setWallet(m_wallet);
    dialog.setAssets(m_assets);
    dialog.setMarketEvent(m_marketEvent);
    dialog.setMaxLeverage(m_maxLeverage);
    TutorialManager::instance()->notifyDialogOpened(&dialog, "ExchangeDialog");
    dialog.exec();

    TutorialManager::instance()->notifyDialogClosed("ExchangeDialog");
    checkTutorialOnShow();
    updateHUD();
}

void MeritHall::onShopClicked()
{
    TutorialManager::instance()->notifyAreaClicked("pavilionShopBtn");
    ShopDialog dialog(this);
    dialog.setWallet(m_wallet);
    connect(&dialog, &ShopDialog::cloudInstrumentChanged, [this](const Instrument& instrument) {
        m_cloudInstrument = instrument;
        ui->instrumentNameLabel->setText(m_cloudInstrument.name());
        updateInstrumentIcon();
        updateCloudInstrumentDisplay();
    });
    connect(&dialog, &ShopDialog::lotusInstrumentToggled, [this](const Instrument& instrument) {
        bool found = false;
        for (int i = 0; i < m_lotusInstruments.size(); ++i) {
            if (m_lotusInstruments[i].type() == instrument.type()) {
                m_lotusInstruments.removeAt(i);
                m_lotusDurations.remove(instrument.type());
                found = true;
                break;
            }
        }
        if (!found) {
            m_lotusInstruments.append(instrument);
            if (instrument.duration() > 0) {
                m_lotusDurations[instrument.type()] = instrument.duration();
            }
        }
        updateLotusInstrumentDisplay();
    });
    TutorialManager::instance()->notifyDialogOpened(&dialog, "ShopDialog");
    dialog.exec();

    TutorialManager::instance()->notifyDialogClosed("ShopDialog");
    checkTutorialOnShow();
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

QLabel* MeritHall::createDoorGlowLabel()
{
    QLabel *label = new QLabel(this);
    QPixmap glow(":/images/door_glow.png");
    if (!glow.isNull()) {
        label->setPixmap(glow);
        label->setScaledContents(true);
    }
    label->setAttribute(Qt::WA_TransparentForMouseEvents);

    // 外发光效果，让小光圈也能明显可见（完全不透明）
    QGraphicsDropShadowEffect *glowEffect = new QGraphicsDropShadowEffect(label);
    glowEffect->setBlurRadius(30);
    glowEffect->setColor(QColor(255, 240, 150, 255));
    glowEffect->setOffset(0, 0);
    label->setGraphicsEffect(glowEffect);

    label->raise();
    label->hide();
    return label;
}

void MeritHall::createPavilionButtons()
{
    // 无文字透明热区按钮，覆盖整个阁楼建筑
    pavilionBankBtn = new QPushButton(this);
    pavilionBankBtn->setObjectName("pavilionBankBtn");
    pavilionBankBtn->setCursor(Qt::PointingHandCursor);
    pavilionBankBtn->installEventFilter(this);
    connect(pavilionBankBtn, &QPushButton::clicked, this, &MeritHall::onBankClicked);

    pavilionExchangeBtn = new QPushButton(this);
    pavilionExchangeBtn->setObjectName("pavilionExchangeBtn");
    pavilionExchangeBtn->setCursor(Qt::PointingHandCursor);
    pavilionExchangeBtn->installEventFilter(this);
    connect(pavilionExchangeBtn, &QPushButton::clicked, this, &MeritHall::onExchangeClicked);

    pavilionShopBtn = new QPushButton(this);
    pavilionShopBtn->setObjectName("pavilionShopBtn");
    pavilionShopBtn->setCursor(Qt::PointingHandCursor);
    pavilionShopBtn->installEventFilter(this);
    connect(pavilionShopBtn, &QPushButton::clicked, this, &MeritHall::onShopClicked);

    pavilionAchievementBtn = new QPushButton(this);
    pavilionAchievementBtn->setObjectName("pavilionAchievementBtn");
    pavilionAchievementBtn->setCursor(Qt::PointingHandCursor);
    pavilionAchievementBtn->installEventFilter(this);
    connect(pavilionAchievementBtn, &QPushButton::clicked, this, &MeritHall::onAchievementClicked);

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

    // 透明热区样式：无文字，hover 时不显示矩形遮罩
    QString pavilionStyle =
        QString("QPushButton { "
                "background: transparent; "
                "border: none; "
                "} "
                "QPushButton:hover { "
                "background: transparent; "
                "border: none; "
                "}");

    pavilionBankBtn->setStyleSheet(pavilionStyle);
    pavilionExchangeBtn->setStyleSheet(pavilionStyle);
    pavilionShopBtn->setStyleSheet(pavilionStyle);
    pavilionAchievementBtn->setStyleSheet(pavilionStyle);

    // 四个阁楼热区覆盖对应建筑区域（无文字，与背景牌匾自然融合）
    // 右上：善财司
    pavilionBankBtn->setGeometry(w * 0.66, h * 0.10, w * 0.34, h * 0.30);
    // 左上：幻缘所
    pavilionExchangeBtn->setGeometry(0, h * 0.10, w * 0.34, h * 0.30);
    // 左下：法器阁
    pavilionShopBtn->setGeometry(0, h * 0.45, w * 0.30, h * 0.27);
    // 右下：成就
    pavilionAchievementBtn->setGeometry(w * 0.70, h * 0.45, w * 0.30, h * 0.27);

    // 更新门口光芒层位置（基于 2560x1440 背景图的精确像素坐标换算）
    // 背景图缩放比例
    double scale = static_cast<double>(w) / 2560.0;

    // 法器阁：中心(709,924)，大小 50x150
    int gw1 = static_cast<int>(50 * scale);
    int gh1 = static_cast<int>(150 * scale);
    m_shopGlowLabel->setGeometry(static_cast<int>(709 * scale) - gw1/2,
                                  static_cast<int>(924 * scale) - gh1/2,
                                  gw1, gh1);

    // 幻缘所：中心(821,556)，大小 30x90
    int gw2 = static_cast<int>(30 * scale);
    int gh2 = static_cast<int>(90 * scale);
    m_exchangeGlowLabel->setGeometry(static_cast<int>(821 * scale) - gw2/2,
                                      static_cast<int>(556 * scale) - gh2/2,
                                      gw2, gh2);

    // 善财司：中心(1777,557)，大小 30x90
    int gw3 = static_cast<int>(30 * scale);
    int gh3 = static_cast<int>(90 * scale);
    m_bankGlowLabel->setGeometry(static_cast<int>(1777 * scale) - gw3/2,
                                  static_cast<int>(557 * scale) - gh3/2,
                                  gw3, gh3);

    // 成就：中心(1872,921)，大小 50x150
    int gw4 = static_cast<int>(50 * scale);
    int gh4 = static_cast<int>(150 * scale);
    m_achievementGlowLabel->setGeometry(static_cast<int>(1872 * scale) - gw4/2,
                                         static_cast<int>(921 * scale) - gh4/2,
                                         gw4, gh4);
}

void MeritHall::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    // 强制保持 16:9 比例，以宽度为准
    int w = width();
    int expectedH = w * 9 / 16;
    if (height() != expectedH) {
        QTimer::singleShot(0, this, [this, w]() { resize(w, w * 9 / 16); });
        return;
    }

    // 重新设置背景图，避免平铺
    QPixmap background(":/images/modified_main_background_new.jpeg");
    QPalette palette;
    palette.setBrush(QPalette::Window, background.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    setPalette(palette);

    updatePavilionPositions();
    updateFishGlowPosition();
    updateFishIconPosition();
    updateFishClickAreaPosition(m_fishClickArea);
    updateCloudInstrumentPosition();
    updateLotusInstrumentPosition();

    // 更新教程覆盖层大小
    if (m_tutorialOverlay) {
        m_tutorialOverlay->setGeometry(0, 0, width(), height());
        m_tutorialOverlay->raise();
    }
}

void MeritHall::updateFishGlowPosition()
{
    int w = this->width();
    int h = this->height();
    int size = qMax(120, static_cast<int>(qMin(w / 4, h / 4) ));
    // 云上海域位置：画面正中云海区域
    m_fishGlowLayer->setGeometry(w * 0.5 - size/2, h * 0.48 - size/2, size, size);
}

void MeritHall::updateFishIconPosition()
{
    int w = this->width();
    int h = this->height();
    int size = qMax(120, static_cast<int>(qMin(w / 4, h / 4) ));
    int iconH = size;
    int iconW = static_cast<int>(iconH * 1.5);
    // 云海中心位置（木鱼图标已隐藏，仅保持位置一致）
    m_fishIconLabel->setGeometry(w * 0.5 - iconW/2, h * 0.48 - iconH/2, iconW, iconH);
}

void MeritHall::updateFishClickAreaPosition(QPushButton *btn)
{
    int w = this->width();
    int h = this->height();
    int size = qMax(120, static_cast<int>(qMin(w / 4, h / 4) ));
    // 云海点击区域：画面正中云上
    btn->setGeometry(w * 0.5 - size/2, h * 0.48 - size/2, size, size);
}

void MeritHall::updateCloudInstrumentDisplay()
{
    QString path = m_cloudInstrument.unstrikedImagePath();
    if (!path.isEmpty()) {
        QPixmap pix(path);
        if (!pix.isNull()) {
            m_cloudInstrumentLabel->setPixmap(pix);
        }
    }
    m_cloudInstrumentLabel->raise();
    m_cloudInstrumentLabel->show();
    if (TutorialManager::instance()->isActive() && m_tutorialOverlay) {
        m_tutorialOverlay->raise();
    }
}

static QString lotusImagePath(Instrument::Type type)
{
    switch (type) {
    case Instrument::BasicIncense:
        return ":/images/basic_joss_stick.png";
    case Instrument::ElectronicIncense:
        return ":/images/electronic_incense.png";
    case Instrument::AIChantingMachine:
        return ":/images/ai_chanting.png";
    case Instrument::Cassock:
        return ":/images/cassock.png";
    case Instrument::Compass:
        return ":/images/compass.png";
    case Instrument::Fuchen:
        return ":/images/fuchen.png";
    case Instrument::Ruyi:
        return ":/images/ruyi.png";
    default:
        return "";
    }
}

void MeritHall::updateLotusInstrumentDisplay()
{
    // 同步 labels 数量与辅助法器数量
    while (m_lotusLabels.size() > m_lotusInstruments.size()) {
        delete m_lotusLabels.takeLast();
    }
    while (m_lotusLabels.size() < m_lotusInstruments.size()) {
        QLabel* label = new QLabel(this);
        label->setScaledContents(true);
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
        m_lotusLabels.append(label);
    }
    for (int i = 0; i < m_lotusInstruments.size(); ++i) {
        QString path = lotusImagePath(m_lotusInstruments[i].type());
        if (!path.isEmpty()) {
            QPixmap pix(path);
            if (!pix.isNull()) {
                m_lotusLabels[i]->setPixmap(pix);
            }
        }
        m_lotusLabels[i]->raise();
        m_lotusLabels[i]->show();
    }
    updateLotusInstrumentPosition();
    if (TutorialManager::instance()->isActive() && m_tutorialOverlay) {
        m_tutorialOverlay->raise();
    }
}

void MeritHall::updateCloudInstrumentPosition()
{
    int w = this->width();
    int h = this->height();
    int size = qMax(100, static_cast<int>(qMin(w / 3, h / 3) ));
    // 云上法器位置：与云海点击区同中心，略小一点
    m_cloudInstrumentLabel->setGeometry(w * 0.5 - size/2, h * 0.48 - size/2, size, size);
}

void MeritHall::updateLotusInstrumentPosition()
{
    int w = this->width();
    int h = this->height();
    int size = qMax(60, static_cast<int>(qMin(w / 6, h / 6) ));
    int spacing = qMin(30, w / 30);
    int totalWidth = m_lotusLabels.size() * size + qMax(0, m_lotusLabels.size() - 1) * spacing;
    int startX = w / 2 - totalWidth / 2;
    int y = h * 0.83 - size / 2;
    for (int i = 0; i < m_lotusLabels.size(); ++i) {
        m_lotusLabels[i]->setGeometry(startX + i * (size + spacing), y, size, size);
    }
}

bool MeritHall::eventFilter(QObject *obj, QEvent *event)
{
    if (obj->objectName() == "fishClickArea") {
        if (event->type() == QEvent::Enter) {
            // 鼠标进入时显示发光效果（仙宫白光）
            m_fishGlowLayer->setStyleSheet(
                "#fishGlowLayer { "
                "background: radial-gradient(circle, rgba(255, 255, 255, 0.6) 0%, rgba(200, 230, 255, 0.4) 30%, rgba(150, 200, 255, 0.15) 60%, transparent 80%); "
                "border-radius: 150px; "
                "opacity: 1; "
                "}"
            );
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开时隐藏发光效果
            m_fishGlowLayer->setStyleSheet(
                "#fishGlowLayer { "
                "background: radial-gradient(circle, rgba(255, 255, 255, 0.6) 0%, rgba(200, 230, 255, 0.4) 30%, rgba(150, 200, 255, 0.15) 60%, transparent 80%); "
                "border-radius: 150px; "
                "opacity: 0; "
                "}"
            );
        }
    }

    // 建筑按钮 hover：在门口显示光芒射出效果
    QString objName = obj->objectName();
    if (event->type() == QEvent::Enter) {
        if (objName == "pavilionBankBtn") {
            m_bankGlowLabel->show();
        } else if (objName == "pavilionExchangeBtn") {
            m_exchangeGlowLabel->show();
        } else if (objName == "pavilionShopBtn") {
            m_shopGlowLabel->show();
        } else if (objName == "pavilionAchievementBtn") {
            m_achievementGlowLabel->show();
        }
    } else if (event->type() == QEvent::Leave) {
        if (objName == "pavilionBankBtn") {
            m_bankGlowLabel->hide();
        } else if (objName == "pavilionExchangeBtn") {
            m_exchangeGlowLabel->hide();
        } else if (objName == "pavilionShopBtn") {
            m_shopGlowLabel->hide();
        } else if (objName == "pavilionAchievementBtn") {
            m_achievementGlowLabel->hide();
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

void MeritHall::onAchievementClicked()
{
    if (!m_achievementManager) return;

    TutorialManager::instance()->notifyAreaClicked("pavilionAchievementBtn");

    AchievementDialog dialog(m_achievementManager, this);
    TutorialManager::instance()->notifyDialogOpened(&dialog, "AchievementDialog");
    dialog.exec();

    TutorialManager::instance()->notifyDialogClosed("AchievementDialog");
    checkTutorialOnShow();
}


void MeritHall::createEndSamsaraButton()
{
    QPushButton *endBtn = new QPushButton("☸ 结束本轮轮回", this);
    endBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #B71C1C;"
        "  color: #FFFFFF;"
        "  font-size: 12px;"
        "  font-weight: bold;"
        "  border: 1px solid #FF5252;"
        "  border-radius: 4px;"
        "  padding: 2px 8px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #D32F2F;"
        "}"
    );
    endBtn->setCursor(Qt::PointingHandCursor);
    endBtn->setGeometry(width() - 140, 10, 120, 28);
    connect(endBtn, &QPushButton::clicked, this, &MeritHall::onEndSamsaraClicked);
    endBtn->raise();
    endBtn->show();
}

void MeritHall::onEndSamsaraClicked()
{
    if (m_updateTimer) {
        m_updateTimer->stop();
    }
    if (m_gameTimer) {
        m_gameTimer->stop();
    }
    showGameOverDialog();
}

void MeritHall::showGameOverDialog()
{
    if (!m_wallet) return;

    QDialog dialog(this, Qt::Dialog | Qt::FramelessWindowHint);
    dialog.setAttribute(Qt::WA_TranslucentBackground);
    dialog.setFixedSize(480, 380);

    // 创建遮罩层
    QWidget *mask = new QWidget(&dialog);
    mask->setGeometry(0, 0, 480, 380);
    mask->setStyleSheet(
        "QWidget {"
        "  background-color: rgba(20, 15, 10, 0.94);"
        "  border: 3px solid rgba(200, 160, 80, 0.8);"
        "  border-radius: 18px;"
        "}"
    );

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setSpacing(14);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(30, 24, 30, 24);

    QLabel *title = new QLabel("☸ 本轮轮回已结束 ☸", &dialog);
    title->setStyleSheet(
        "font-size: 26px; font-weight: bold; color: #FFD700;"
        "font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;"
    );
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    // 分隔线
    QFrame *line = new QFrame(&dialog);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: rgba(200, 160, 80, 0.5);");
    line->setFixedHeight(2);
    layout->addWidget(line);

    double assetValue = 0;
    for (Asset* asset : m_assets) {
        assetValue += m_wallet->getAssetShares(asset->id()) * asset->price();
    }
    double netWorth = m_wallet->merit() + assetValue + m_wallet->savingsBalance() - m_wallet->totalDebt();

    QLabel *netWorthLabel = new QLabel(QString("最终净资产: %1 功德").arg(netWorth, 0, 'f', 2), &dialog);
    netWorthLabel->setStyleSheet(
        "font-size: 18px; color: #4FC3F7;"
        "font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
    );
    netWorthLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(netWorthLabel);

    QLabel *nextLifeLabel = new QLabel(QString("下世功德消耗: %1").arg(m_wallet->nextLifeLoss(), 0, 'f', 2), &dialog);
    nextLifeLabel->setStyleSheet(
        "font-size: 16px; color: #FF8C00;"
        "font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
    );
    nextLifeLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(nextLifeLabel);

    QLabel *yezhangLabel = new QLabel(QString("业障值: %1").arg(m_wallet->yezhang(), 0, 'f', 0), &dialog);
    yezhangLabel->setStyleSheet(
        "font-size: 16px; color: #EF5350;"
        "font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
    );
    yezhangLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(yezhangLabel);

    if (m_wallet->nextLifeLoss() > 0) {
        QLabel *flavor = new QLabel("\"上辈子造了什么孽...\"", &dialog);
        flavor->setStyleSheet(
            "font-size: 14px; color: #9E9E9E; font-style: italic;"
            "font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
        );
        flavor->setAlignment(Qt::AlignCenter);
        layout->addWidget(flavor);
    }

    layout->addSpacing(10);

    // 自动存档
    bool autoSaved = SaveManager::saveFinal(m_wallet->nextLifeLoss(), m_wallet->yezhang(), "");

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(16);
    btnLayout->addStretch();

    QPushButton *saveBtn = new QPushButton(autoSaved ? "已自动存档 ✓" : "存档失败 ✗", &dialog);
    saveBtn->setEnabled(false);
    saveBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #4A3B2A;"
        "  color: #FFD700;"
        "  border: 1px solid #FFD700;"
        "  border-radius: 8px;"
        "  padding: 8px 20px;"
        "  font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
        "  font-size: 14px;"
        "}"
    );

    QPushButton *exitBtn = new QPushButton("退出轮回", &dialog);
    exitBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #5D4037;"
        "  color: #FFD700;"
        "  border: 2px solid #FFD700;"
        "  border-radius: 8px;"
        "  padding: 8px 28px;"
        "  font-weight: bold;"
        "  font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;"
        "  font-size: 15px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #FFD700;"
        "  color: #3E2723;"
        "}"
    );
    exitBtn->setCursor(Qt::PointingHandCursor);

    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(exitBtn);
    btnLayout->addStretch();
    layout->addLayout(btnLayout);

    connect(exitBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
    close();
}

void MeritHall::checkSamsaraLiquidation()
{
    if (!m_wallet) return;

    double shares = m_wallet->getAssetShares("samsara_futures");
    if (shares <= 0) return;

    Asset* samsara = nullptr;
    for (Asset* a : m_assets) {
        if (a->id() == "samsara_futures") {
            samsara = a;
            break;
        }
    }
    if (!samsara) return;

    double price = samsara->price();
    double costBasis = m_wallet->getAssetCostBasis("samsara_futures");
    double value = shares * price;

    if (costBasis <= 0) return;

    SamsaraFutures* futures = qobject_cast<SamsaraFutures*>(samsara);
    if (!futures) return;

    if (value < costBasis * futures->maintenanceMargin()) {
        double loss = costBasis - value;
        if (loss < 0) loss = 0;

        m_wallet->applySamsaraLoss(loss);
        m_wallet->removeAsset("samsara_futures", shares);
        m_wallet->clearCostBasis("samsara_futures");

        // 爆仓降低信用等级
        if (m_wallet->creditRating()) {
            m_wallet->creditRating()->recordLeverageLiquidation();
        }

        showLiquidationAlert(loss);
        updateHUD();
    }
}

void MeritHall::showLiquidationAlert(double loss)
{
    QDialog dialog(this, Qt::Dialog | Qt::FramelessWindowHint);
    dialog.setAttribute(Qt::WA_TranslucentBackground);
    dialog.setFixedSize(420, 240);

    QWidget *mask = new QWidget(&dialog);
    mask->setGeometry(0, 0, 420, 240);
    mask->setStyleSheet(
        "QWidget {"
        "  background-color: rgba(25, 10, 10, 0.95);"
        "  border: 3px solid rgba(200, 50, 50, 0.8);"
        "  border-radius: 16px;"
        "}"
    );

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setSpacing(12);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(24, 20, 24, 20);

    QLabel *title = new QLabel("⚠ 爆仓警告 ⚠", &dialog);
    title->setStyleSheet(
        "font-size: 24px; font-weight: bold; color: #FF5252;"
        "font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;"
    );
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QFrame *line = new QFrame(&dialog);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: rgba(200, 50, 50, 0.5);");
    line->setFixedHeight(2);
    layout->addWidget(line);

    QLabel *msgLabel = new QLabel(
        QString("轮回孽缘维持率跌破 30%，已强制平仓！\n"
                "本次亏损: %1 功德\n"
                "优先从下世功德扣除，不足部分计入债务。")
            .arg(loss, 0, 'f', 2), &dialog);
    msgLabel->setStyleSheet(
        "font-size: 15px; color: #F5F5DC;"
        "font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
    );
    msgLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(msgLabel);

    QPushButton *okBtn = new QPushButton("阿弥陀佛", &dialog);
    okBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #7A1F1F;"
        "  color: #FFFFFF;"
        "  border: 2px solid #FF5252;"
        "  border-radius: 10px;"
        "  padding: 8px 28px;"
        "  font-weight: bold;"
        "  font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;"
        "  font-size: 15px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #FF5252;"
        "  color: #1A1A1A;"
        "}"
    );
    okBtn->setCursor(Qt::PointingHandCursor);
    layout->addWidget(okBtn, 0, Qt::AlignCenter);

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}

void MeritHall::checkLeverageMargin()
{
    if (!m_wallet) return;

    for (const auto& pos : m_wallet->leveragePositions()) {
        Asset* asset = nullptr;
        for (Asset* a : m_assets) {
            if (a->id() == pos.assetId) {
                asset = a;
                break;
            }
        }
        if (!asset) continue;

        double price = asset->price();
        double marginRate = m_wallet->marginRate(pos.assetId, price);

        if (marginRate < m_maintenanceMarginRate) {
            double marketValue = pos.shares * price;
            double equity = marketValue - pos.borrowed;
            double requiredEquity = marketValue * m_maintenanceMarginRate;
            double shortfall = requiredEquity - equity; // 亏多少才到维持线
            if (shortfall < 0) shortfall = 0;
            // 每 tick 按缺口 × 0.5% 产生业障
            double yezhangAdd = shortfall * 0.005;
            if (yezhangAdd > 0.001) {
                m_wallet->addYezhang(yezhangAdd);
            }
        }
    }
}

// --- 辅助法器加成与倒计时管理 ---

double MeritHall::totalLotusClickBonus() const
{
    double bonus = 0.0;
    for (const Instrument& inst : m_lotusInstruments) {
        bonus += inst.clickBonus();
    }
    return bonus;
}

double MeritHall::totalLotusCritBonus() const
{
    double bonus = 0.0;
    for (const Instrument& inst : m_lotusInstruments) {
        bonus += inst.critBonus();
    }
    return bonus;
}

double MeritHall::totalLotusEfficiencyBonus() const
{
    double bonus = 0.0;
    for (const Instrument& inst : m_lotusInstruments) {
        bonus += inst.efficiencyBonus();
    }
    return bonus;
}

void MeritHall::updateLotusDurations()
{
    if (m_lotusDurations.isEmpty()) return;

    QList<Instrument::Type> expired;
    for (auto it = m_lotusDurations.begin(); it != m_lotusDurations.end(); ++it) {
        it.value()--;
        if (it.value() <= 0) {
            expired.append(it.key());
        }
    }

    for (Instrument::Type type : expired) {
        m_lotusDurations.remove(type);
        // 从装备列表中移除
        for (int i = 0; i < m_lotusInstruments.size(); ++i) {
            if (m_lotusInstruments[i].type() == type) {
                m_lotusInstruments.removeAt(i);
                break;
            }
        }
        QString name = Instrument::typeToString(type);
        showEventPopup(QString("%1 已燃尽熄灭，自动卸下。").arg(name));
    }

    if (!expired.isEmpty()) {
        updateLotusInstrumentDisplay();
    }
}

// --- 修炼等级与法器共鸣显示 ---

void MeritHall::setupCultivationDisplay()
{
    // 在 HUD 区域下方添加修炼等级显示
    m_cultivationLabel = new QLabel(this);
    m_cultivationLabel->setStyleSheet(
        "QLabel {"
        "  color: #FFD700;"
        "  font-size: 14px;"
        "  font-weight: bold;"
        "  font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;"
        "  background-color: rgba(30, 40, 60, 0.65);"
        "  border: 1px solid rgba(255, 215, 100, 0.4);"
        "  border-radius: 8px;"
        "  padding: 4px 10px;"
        "}"
    );
    m_cultivationLabel->setAlignment(Qt::AlignCenter);
    m_cultivationLabel->setText("凡人 Lv1");
    m_cultivationLabel->adjustSize();

    // 经验条
    m_expBar = new QProgressBar(this);
    m_expBar->setStyleSheet(
        "QProgressBar {"
        "  background-color: rgba(30, 40, 60, 0.65);"
        "  border: 1px solid rgba(255, 215, 100, 0.4);"
        "  border-radius: 6px;"
        "  text-align: center;"
        "  font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
        "  font-size: 11px;"
        "  color: #F5F5DC;"
        "}"
        "QProgressBar::chunk {"
        "  background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "    stop:0 #5D4037, stop:1 #FFD700);"
        "  border-radius: 6px;"
        "}"
    );
    m_expBar->setMaximum(100);
    m_expBar->setValue(0);
    m_expBar->setTextVisible(true);
    m_expBar->setFormat("%v/%m");
    m_expBar->setFixedSize(140, 18);

    // 位置：HUD 下方
    m_cultivationLabel->move(20, ui->hudWidget->y() + ui->hudWidget->height() + 8);
    m_expBar->move(20, m_cultivationLabel->y() + m_cultivationLabel->height() + 4);

    m_cultivationLabel->show();
    m_expBar->show();
}

void MeritHall::updateCultivationDisplay()
{
    if (!m_wallet || !m_cultivationLabel || !m_expBar) return;

    CultivationLevel* cl = m_wallet->cultivationLevel();
    if (!cl) return;

    m_cultivationLabel->setText(
        QString("%1 Lv%2").arg(cl->levelTitle()).arg(cl->level())
    );
    m_cultivationLabel->adjustSize();

    m_expBar->setMaximum(cl->maxExp());
    m_expBar->setValue(cl->exp());
}

void MeritHall::updateResonance()
{
    if (!m_resonance) return;
    m_resonance->updateResonance(m_cloudInstrument, m_lotusInstruments);
}

void MeritHall::showResonancePopup(const QString& name, const QString& description)
{
    showEventPopup(QString("✦ %1 ✦ %2").arg(name).arg(description));
}

// --- 内嵌教程系统 ---

void MeritHall::setupTutorial()
{
    m_tutorialOverlay = new TutorialOverlay(this);
    m_tutorialOverlay->setGeometry(0, 0, width(), height());
    m_tutorialOverlay->hide();

    // 连接 TutorialManager 步骤变化信号，更新高亮区域
    connect(TutorialManager::instance(), &TutorialManager::stepChanged,
            this, [this](int stepIndex) {
        Q_UNUSED(stepIndex)
        const TutorialManager::Step* step = TutorialManager::instance()->currentStep();
        if (!step) return;

        if (!step->targetWidget.isEmpty()) {
            // 查找目标控件并获取其几何位置
            QWidget* target = findChild<QWidget*>(step->targetWidget);
            if (target) {
                QRect globalRect = target->geometry();
                m_tutorialOverlay->setHighlightRect(globalRect);
            } else {
                m_tutorialOverlay->clearHighlight();
            }
        } else {
            m_tutorialOverlay->clearHighlight();
        }
    });

    // 首次进入且未完成过教程，自动启动
    if (!TutorialManager::instance()->hasCompletedBefore()) {
        QTimer::singleShot(800, this, [this]() {
            m_tutorialOverlay->startTutorial();
        });
    }
}

void MeritHall::checkTutorialOnShow()
{
    if (TutorialManager::instance()->isActive() && m_tutorialOverlay) {
        m_tutorialOverlay->show();
        m_tutorialOverlay->raise();
    }
}
