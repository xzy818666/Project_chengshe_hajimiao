#include <QApplication>
#include <QTimer>
#include <QObject>
#include <cstdlib>
#include <ctime>
#include "startmenu.h"
#include "merithall.h"
#include "wallet.h"
#include "meritindex.h"
#include "karmabond.h"
#include "dharmafund.h"
#include "samsarafutures.h"
#include "marketevent.h"
#include "achievementmanager.h"
#include "savemanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    std::srand(std::time(nullptr));

    StartMenu startMenu;
    startMenu.show();

    QObject::connect(&startMenu, &StartMenu::startGame, [&](GameDifficulty diff) {
        startMenu.hide();

        GameConfig config = configForDifficulty(diff);

        MeritHall *window = new MeritHall;
        window->setAttribute(Qt::WA_DeleteOnClose);

        double lastNextLifeLoss = SaveManager::loadNextLifeLoss();
        double nextLifePool = qMax(0.0, config.nextLifePool - lastNextLifeLoss);

        Wallet *wallet = new Wallet(window, config.initialMerit, nextLifePool,
                                    config.inflationRate, config.creditScore);
        MarketEvent *marketEvent = new MarketEvent(window);
        AchievementManager *achievementManager = new AchievementManager(window);

        MeritIndex *meritIndex = new MeritIndex(window);
        KarmaBond *karmaBond = new KarmaBond(window);
        DharmaFund *dharmaFund = new DharmaFund(window);
        SamsaraFutures *samsaraFutures = new SamsaraFutures(window);

        QList<Asset*> assets = {meritIndex, karmaBond, dharmaFund, samsaraFutures};

        QTimer *updateTimer = new QTimer(window);
        QObject::connect(updateTimer, &QTimer::timeout, [wallet, marketEvent, assets]() {
            double dt = 0.1;
            for (Asset* asset : assets) {
                asset->update(dt, marketEvent->driftModifier(), marketEvent->volatilityModifier());
            }
            marketEvent->update(dt);
            wallet->updateDebtInterest();
            wallet->updateSavingsInterest();
            wallet->applyInflation(dt);
        });

        window->setWallet(wallet);
        window->setAssets(assets);
        window->setMarketEvent(marketEvent);
        window->setAchievementManager(achievementManager);
        window->setGameTimer(updateTimer);
        window->setLeverageParams(config.maxLeverage, config.maintenanceMarginRate);
        window->show();
        updateTimer->start(100);
    });

    return app.exec();
}
