#ifndef MERITHALL_H
#define MERITHALL_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QDate>
#include <QFrame>
#include <QTimer>
#include <QPropertyAnimation>
#include "wallet.h"
#include "instrument.h"
#include "asset.h"
#include "marketevent.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MeritHall; }
QT_END_NAMESPACE

#include "achievementmanager.h"

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
    Instrument m_currentInstrument;
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
    void updateInstrumentDisplay();
    void updateCloudInstrumentPosition();
    void updateLotusInstrumentPosition();
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
    QLabel *m_lotusInstrumentLabel;   // 莲台法器（电子烧香/AI诵经机），初始为空
};

#endif // MERITHALL_H
