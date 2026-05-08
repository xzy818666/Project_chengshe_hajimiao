#ifndef EXCHANGEDIALOG_H
#define EXCHANGEDIALOG_H

#include <QDialog>
#include <QtCharts>
#include "wallet.h"
#include "asset.h"
#include "marketevent.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ExchangeDialog; }
QT_END_NAMESPACE

class ExchangeDialog : public QDialog
{
    Q_OBJECT

public:
    ExchangeDialog(QWidget *parent = nullptr);
    ~ExchangeDialog();

    void setWallet(Wallet* wallet);
    void setAssets(QList<Asset*> assets);
    void setMarketEvent(MarketEvent* marketEvent);

private slots:
    void onAssetSelected(int index);
    void onBuy();
    void onSell();
    void updatePrices();
    void updatePortfolio();
    void updateEventTicker();
    void showPortfolioView();

private:
    Ui::ExchangeDialog *ui;
    Wallet* m_wallet;
    QList<Asset*> m_assets;
    MarketEvent* m_marketEvent;
    Asset* m_selectedAsset;
    QLineSeries* m_priceSeries;
    QChart* m_chart;
    QPieSeries* m_pieSeries;
    QChart* m_pieChart;
    QTimer* m_updateTimer;

    void updateChart();
    QString riskLevelToString(Asset::RiskLevel level);
    QString getPortfolioRiskRating();
};

#endif // EXCHANGEDIALOG_H
