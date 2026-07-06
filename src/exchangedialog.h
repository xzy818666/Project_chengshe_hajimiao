#ifndef EXCHANGEDIALOG_H
#define EXCHANGEDIALOG_H

#include <QDialog>
#include <QMap>
#include <QRegularExpression>
#include <QResizeEvent>
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

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateLayout();
    Ui::ExchangeDialog *ui;
    Wallet* m_wallet;
    QList<Asset*> m_assets;
    MarketEvent* m_marketEvent;
    Asset* m_selectedAsset;
    QLineSeries* m_priceSeries;
    QChart* m_chart;
    QPieSeries* m_pieSeries;
    QChart* m_pieChart;
    QScatterSeries* m_calScatter;
    QScatterSeries* m_calPortfolio;
    QLineSeries* m_calLine;
    QChart* m_calChart;
    QList<QGraphicsItem*> m_calLabels;
    QTimer* m_updateTimer;

    PortfolioAdvisor* m_advisor;
    ArbitrageScanner* m_scanner;
    AutoInvestor* m_autoInvestor;
    QLabel* m_adviceLabel;
    QLabel* m_arbitrageLabel;

    void updateChart();
    void updateCalChart();
    void updateLeverageCombo();
    QString riskLevelToString(Asset::RiskLevel level);
    void setTradingMode(bool trading);
    QString getPortfolioRiskRating();
    void showToast(const QString& title, const QString& message, const QString& type = "info");

    double m_maxLeverage = 1.0;

    QSize m_baseSize;
    QMap<QWidget*, QRect> m_baseGeometries;
    QMap<QWidget*, int> m_baseFontSizes;

    void scaleFonts(double scale);
};

#endif // EXCHANGEDIALOG_H
