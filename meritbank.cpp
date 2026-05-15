#include <QApplication>
#include <QTimer>
#include <QObject>
#include "merithall.h"
#include "wallet.h"
#include "meritindex.h"
#include "karmabond.h"
#include "dharmafund.h"
#include "samsarafutures.h"
#include "marketevent.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Wallet wallet;
    MarketEvent marketEvent;

    MeritIndex meritIndex;
    KarmaBond karmaBond;
    DharmaFund dharmaFund;
    SamsaraFutures samsaraFutures;

    QList<Asset*> assets = {&meritIndex, &karmaBond, &dharmaFund, &samsaraFutures};

    MeritHall window;
    window.setWallet(&wallet);
    window.setAssets(assets);
    window.setMarketEvent(&marketEvent);
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
