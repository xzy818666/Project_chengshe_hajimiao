#include "merithall.h"
#include "ui_merithall.h"
#include "bankdialog.h"
#include "exchangedialog.h"
#include "shopdialog.h"
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

    // 连接点击事件
    connect(m_fishClickArea, &QPushButton::clicked, this, &MeritHall::onInstrumentClicked);
    connect(m_fishClickArea, &QPushButton::pressed, this, &MeritHall::onInstrumentPressed);
    connect(m_fishClickArea, &QPushButton::released, this, &MeritHall::onInstrumentReleased);

    // 隐藏原来的法器相关控件
    ui->instrumentIconLabel->hide();
    ui->instrumentNameLabel->hide();
    ui->instrumentButton->hide();
    ui->feedbackLabel->hide();

    // 创建四栋阁楼按钮
    createPavilionButtons();

    connect(ui->bankBtn, &QPushButton::clicked, this, &MeritHall::onBankClicked);
    connect(ui->exchangeBtn, &QPushButton::clicked, this, &MeritHall::onExchangeClicked);
    connect(ui->shopBtn, &QPushButton::clicked, this, &MeritHall::onShopClicked);
    connect(ui->yezhangBtn, &QPushButton::clicked, this, &MeritHall::onYezhangClicked);

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &MeritHall::updateAutoIncome);
    connect(m_updateTimer, &QTimer::timeout, this, &MeritHall::updateMarketEvent);
    m_updateTimer->start(1000);
}

MeritHall::~MeritHall()
{
    delete ui;
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

void MeritHall::onInstrumentPressed()
{
    // 显示夸张的"+2"发光文字动画
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

    QTimer::singleShot(500, [this]() {
        ui->feedbackLabel->setText("");
    });

    updateHUD();
}

void MeritHall::updateAutoIncome()
{
    if (!m_wallet) return;

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
        ui->yezhangLabel->setStyleSheet("color: red; font-weight: bold;");
    } else {
        ui->yezhangLabel->setStyleSheet("");
    }
}

void MeritHall::updateMarketEvent()
{
    if (m_marketEvent && m_marketEvent->isActive()) {
        ui->eventLabel->setText(m_marketEvent->currentEvent());
        ui->eventLabel->setStyleSheet("color: orange; font-weight: bold;");
    } else {
        ui->eventLabel->setText("市场平稳运行中...");
        ui->eventLabel->setStyleSheet("");
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

    pavilionYezhangBtn = new QPushButton("忏悔楼", this);
    pavilionYezhangBtn->setCursor(Qt::PointingHandCursor);
    connect(pavilionYezhangBtn, &QPushButton::clicked, this, &MeritHall::onYezhangClicked);

    // 初始化按钮位置和样式（响应式）
    updatePavilionPositions();

    // 隐藏原来的底部导航按钮
    ui->bankBtn->hide();
    ui->exchangeBtn->hide();
    ui->shopBtn->hide();
    ui->yezhangBtn->hide();
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
    pavilionYezhangBtn->setStyleSheet(pavilionStyle);
    
    // 四个阁楼门的位置比例（可调整）
    pavilionBankBtn->setGeometry(w * 0.25 - btnWidth/2, h * 0.38 - btnHeight/2, btnWidth, btnHeight);
    pavilionExchangeBtn->setGeometry(w * 0.76 - btnWidth/2, h * 0.38 - btnHeight/2, btnWidth, btnHeight);
    pavilionShopBtn->setGeometry(w * 0.18 - btnWidth/2, h * 0.66 - btnHeight/2, btnWidth, btnHeight);
    pavilionYezhangBtn->setGeometry(w * 0.81 - btnWidth/2, h * 0.66 - btnHeight/2, btnWidth, btnHeight);
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
