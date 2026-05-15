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

        Wallet *wallet = new Wallet(window, config.initialMerit, config.nextLifePool,
                                    config.inflationRate, config.creditScore);
        MarketEvent *marketEvent = new MarketEvent(window);
        AchievementManager *achievementManager = new AchievementManager(window);

        MeritIndex *meritIndex = new MeritIndex(window);
        KarmaBond *karmaBond = new KarmaBond(window);
        DharmaFund *dharmaFund = new DharmaFund(window);
        SamsaraFutures *samsaraFutures = new SamsaraFutures(window);

        QList<Asset*> assets = {meritIndex, karmaBond, dharmaFund, samsaraFutures};

        window->setWallet(wallet);
        window->setAssets(assets);
        window->setMarketEvent(marketEvent);
        window->setAchievementManager(achievementManager);
        window->show();

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
        updateTimer->start(100);
    });

    return app.exec();
}
