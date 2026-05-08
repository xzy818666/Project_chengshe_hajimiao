#include "merithall.h"
#include "ui_merithall.h"
#include "bankdialog.h"
#include "exchangedialog.h"
#include "shopdialog.h"
#include "achievementdialog.h"
#include <cstdlib>

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

    setupEventPopup();

    connect(ui->instrumentButton, &QPushButton::pressed, this, &MeritHall::onInstrumentPressed);
    connect(ui->instrumentButton, &QPushButton::released, this, &MeritHall::onInstrumentReleased);
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
    ui->instrumentButton->setStyleSheet(
        "font-size: 24px; border-radius: 12px; background-color: #281815; color: #FFD700;"
    );
}

void MeritHall::onInstrumentReleased()
{
    ui->instrumentButton->setStyleSheet(
        "font-size: 24px; border-radius: 12px; background-color: #3E2723; color: #FFD700;"
    );
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
    if (m_dayTickCounter >= 30) {
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
    if (yezhang > 0) {
        ui->yezhangLabel->setStyleSheet("color: #EF5350; font-weight: bold;");
    } else {
        ui->yezhangLabel->setStyleSheet("color: #EF5350; font-weight: bold;");
    }

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
    });
    dialog.exec();
    updateHUD();
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

void MeritHall::onAchievementClicked()
{
    if (!m_achievementManager) return;
    AchievementDialog dialog(m_achievementManager, this);
    dialog.exec();
}
