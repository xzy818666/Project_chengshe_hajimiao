#ifndef MERITHALL_H
#define MERITHALL_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QDate>
#include <QFrame>
#include <QTimer>
#include <QPropertyAnimation>
#include <QProgressBar>
#include "wallet.h"
#include "instrument.h"
#include "asset.h"
#include "marketevent.h"
#include "achievementmanager.h"
#include "instrumentresonance.h"
#include "tutorialmanager.h"
#include "tutorialoverlay.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MeritHall; }
QT_END_NAMESPACE

class MeritHall : public QMainWindow
{
    Q_OBJECT

public:
    MeritHall(QWidget *parent = nullptr);
    ~MeritHall();

    void setWallet(Wallet* wallet);
    void setAssets(QList<Asset*> assets);
    void setMarketEvent(MarketEvent* marketEvent);
    void setAchievementManager(AchievementManager* manager);
    void setGameTimer(QTimer* timer);
    void setLeverageParams(double maxLeverage, double maintenanceMarginRate);

private slots:
    void onInstrumentClicked();
    void onInstrumentPressed();
    void onInstrumentReleased();
    void updateHUD();
    void updateAutoIncome();
    void updateMarketEvent();
    void updateDateDisplay();
    void hideEventPopup();
    void onBankClicked();
    void onExchangeClicked();
    void onShopClicked();
    void onYezhangClicked();
    void onAchievementClicked();
    void onEndSamsaraClicked();
    void checkSamsaraLiquidation();
    void checkLeverageMargin();
    void showLiquidationAlert(double loss);

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::MeritHall *ui;
    Wallet* m_wallet;
    Instrument m_cloudInstrument;              // 主动法器（云上）
    QList<Instrument> m_lotusInstruments;      // 辅助法器（莲台）
    QList<QLabel*> m_lotusLabels;
    QMap<Instrument::Type, int> m_lotusDurations; // 辅助法器剩余秒数（烧香倒计时）
    QList<Asset*> m_assets;
    MarketEvent* m_marketEvent;
    QTimer* m_updateTimer;
    QTimer* m_gameTimer = nullptr;
    int m_clickCount;
    double m_autoIncomePerSec;
    QDate m_currentDate;
    int m_dayTickCounter;
    double m_maxLeverage = 1.0;
    double m_maintenanceMarginRate = 0.20;

    AchievementManager* m_achievementManager = nullptr;

    // 事件弹出通知
    QFrame* m_eventPopup = nullptr;
    QLabel* m_eventPopupLabel = nullptr;
    QPropertyAnimation* m_eventAnim = nullptr;
    QTimer* m_eventHideTimer = nullptr;
    QString m_lastEventText;

    void updateInstrumentIcon();
    void updateCloudInstrumentDisplay();
    void updateLotusInstrumentDisplay();
    void updateCloudInstrumentPosition();
    void updateLotusInstrumentPosition();
    void syncLotusLabels();
    QLabel* createDoorGlowLabel();
    void createPavilionButtons();
    void createEndSamsaraButton();
    void showGameOverDialog();
    void updatePavilionPositions();
    void updateFishGlowPosition();
    void updateFishIconPosition();
    void updateFishClickAreaPosition(QPushButton *btn);
    void setupEventPopup();
    void showEventPopup(const QString& text);

    // 辅助法器加成计算
    double totalLotusClickBonus() const;
    double totalLotusCritBonus() const;
    double totalLotusEfficiencyBonus() const;
    void updateLotusDurations(); // 每秒更新烧香倒计时并移除到期法器

    // 法器组合共鸣
    InstrumentResonance* m_resonance;
    void updateResonance();
    void showResonancePopup(const QString& name, const QString& desc);

    // 修炼等级显示
    QLabel* m_cultivationLabel = nullptr;  // 等级/称号
    QProgressBar* m_expBar = nullptr;      // 经验条
    void setupCultivationDisplay();
    void updateCultivationDisplay();

    QPushButton *pavilionBankBtn;
    QPushButton *pavilionExchangeBtn;
    QPushButton *pavilionShopBtn;
    QPushButton *pavilionAchievementBtn;
    QLabel *m_fishGlowLayer;
    QLabel *m_fishIconLabel;
    QPushButton *m_fishClickArea;

    // 建筑门口光芒层（hover 时显示）
    QLabel *m_bankGlowLabel;
    QLabel *m_exchangeGlowLabel;
    QLabel *m_shopGlowLabel;
    QLabel *m_achievementGlowLabel;

    // 法器贴图显示
    QLabel *m_cloudInstrumentLabel;   // 云上法器（基础木鱼/涡轮/量子佛珠）

    // 内嵌教程系统
    TutorialOverlay* m_tutorialOverlay = nullptr;
    void setupTutorial();
    void checkTutorialOnShow();
};

#endif // MERITHALL_H
