#include "merithall.h"
#include "wallet.h"
#include "meritindex.h"
#include "karmabond.h"
#include "dharmafund.h"
#include "samsarafutures.h"
#include "marketevent.h"
#include "achievementmanager.h"
#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Wallet wallet;
    MarketEvent marketEvent;
    AchievementManager achievementManager;

    MeritIndex meritIndex;
    KarmaBond karmaBond;
    DharmaFund dharmaFund;
    SamsaraFutures samsaraFutures;

    QList<Asset*> assets = {&meritIndex, &karmaBond, &dharmaFund, &samsaraFutures};

    // 连接成就系统到 Wallet 信号
    QObject::connect(&wallet, &Wallet::meritChanged, &achievementManager, &AchievementManager::onMeritChanged);
    QObject::connect(&wallet, &Wallet::debtChanged, &achievementManager, [&](double) {
        achievementManager.onNetWorthChanged(
            wallet.merit() + wallet.savingsBalance() - wallet.totalDebt()
        );
    });
    QObject::connect(&wallet, &Wallet::yezhangChanged, &achievementManager, &AchievementManager::onYezhangChanged);
    QObject::connect(&wallet, &Wallet::assetBought, &achievementManager, &AchievementManager::onAssetBought);
    QObject::connect(&wallet, &Wallet::assetSold, &achievementManager, &AchievementManager::onAssetSold);
    QObject::connect(&wallet, &Wallet::borrowed, &achievementManager, &AchievementManager::onBorrowed);

    MeritHall window;
    window.setWallet(&wallet);
    window.setAssets(assets);
    window.setMarketEvent(&marketEvent);
    window.setAchievementManager(&achievementManager);
    window.show();

    QTimer updateTimer;
    QObject::connect(&updateTimer, &QTimer::timeout, [&]() {
        double dt = 0.1;
        for (Asset* asset : assets) {
            asset->update(dt, marketEvent.driftModifier(), marketEvent.volatilityModifier());
        }
        marketEvent.update(dt);
        wallet.updateDebtInterest();
        wallet.updateSavingsInterest();
        wallet.applyInflation(dt);
    });
    updateTimer.start(100);

    return app.exec();
}
