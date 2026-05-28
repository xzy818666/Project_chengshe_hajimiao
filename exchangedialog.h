#ifndef EXCHANGEDIALOG_H
#define EXCHANGEDIALOG_H

#include <QDialog>
#include <QtCharts>
#include "wallet.h"
#include "asset.h"
#include "marketevent.h"

class PortfolioAdvisor;
class ArbitrageScanner;
class AutoInvestor;
class QLabel;

QT_BEGIN_NAMESPACE
namespace Ui { class ExchangeDialog; }
QT_END_NAMESPACE

class ExchangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExchangeDialog(QWidget *parent = nullptr);
    ~ExchangeDialog();

    void setWallet(Wallet* wallet);
    void setAssets(QList<Asset*> assets);
    void setMarketEvent(MarketEvent* marketEvent);
    void setMaxLeverage(double maxLeverage);

private slots:
    void onAssetSelected(int index);
    void onBuy();
    void onSell();
    void updatePrices();
    void updatePortfolio();
    void updateEventTicker();
    void showPortfolioView();
    void updateAdvice();
    void updateArbitrage();
    void onAutoInvestToggled(bool checked);
    void onAutoInvestSettingsChanged();

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

    PortfolioAdvisor* m_advisor;
    ArbitrageScanner* m_scanner;
    AutoInvestor* m_autoInvestor;
    QLabel* m_adviceLabel;
    QLabel* m_arbitrageLabel;

    void updateChart();
    void updateLeverageCombo();
    QString riskLevelToString(Asset::RiskLevel level);
    QString getPortfolioRiskRating();

    double m_maxLeverage = 1.0;
};

#endif // EXCHANGEDIALOG_H
