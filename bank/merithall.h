#ifndef MERITHALL_H
#define MERITHALL_H

#include <QMainWindow>
#include <QDate>
#include "wallet.h"
#include "instrument.h"
#include "asset.h"
#include "marketevent.h"
#include "achievementmanager.h"

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

private slots:
    void onInstrumentClicked();
    void onInstrumentPressed();
    void onInstrumentReleased();
    void updateHUD();
    void updateAutoIncome();
    void updateMarketEvent();
    void updateDateDisplay();
    void onBankClicked();
    void onExchangeClicked();
    void onShopClicked();
    void onYezhangClicked();
    void onAchievementClicked();

private:
    Ui::MeritHall *ui;
    Wallet* m_wallet;
    Instrument m_currentInstrument;
    QList<Asset*> m_assets;
    MarketEvent* m_marketEvent = nullptr;
    AchievementManager* m_achievementManager = nullptr;
    QTimer* m_updateTimer = nullptr;
    int m_clickCount;
    double m_autoIncomePerSec;
    QDate m_currentDate;
    int m_dayTickCounter;
};

#endif // MERITHALL_H
