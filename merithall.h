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
    int m_clickCount;
    double m_autoIncomePerSec;
    QDate m_currentDate;
    int m_dayTickCounter;

    AchievementManager* m_achievementManager = nullptr;

    // 事件弹出通知
    QFrame* m_eventPopup = nullptr;
    QLabel* m_eventPopupLabel = nullptr;
    QPropertyAnimation* m_eventAnim = nullptr;
    QTimer* m_eventHideTimer = nullptr;
    QString m_lastEventText;

    void updateInstrumentIcon();
    void createPavilionButtons();
    void createEndSamsaraButton();
    void showGameOverDialog();
    void updatePavilionPositions();
    void updateFishGlowPosition();
    void updateFishClickAreaPosition(QPushButton *btn);
    void setupEventPopup();
    void showEventPopup(const QString& text);

    QPushButton *pavilionBankBtn;
    QPushButton *pavilionExchangeBtn;
    QPushButton *pavilionShopBtn;
    QPushButton *pavilionAchievementBtn;
    QLabel *m_fishGlowLayer;
    QPushButton *m_fishClickArea;
};

#endif // MERITHALL_H
