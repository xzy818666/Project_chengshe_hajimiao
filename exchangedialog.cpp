#include "exchangedialog.h"
#include "ui_exchangedialog.h"
#include <QDateTime>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>

ExchangeDialog::ExchangeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ExchangeDialog)
    , m_wallet(nullptr)
    , m_selectedAsset(nullptr)
    , m_priceSeries(nullptr)
    , m_chart(nullptr)
    , m_pieSeries(nullptr)
    , m_pieChart(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("幻缘所");

    connect(ui->assetList, &QListWidget::currentRowChanged, this, &ExchangeDialog::onAssetSelected);
    connect(ui->buyBtn, &QPushButton::clicked, this, &ExchangeDialog::onBuy);
    connect(ui->sellBtn, &QPushButton::clicked, this, &ExchangeDialog::onSell);
    connect(ui->portfolioBtn, &QPushButton::clicked, this, &ExchangeDialog::showPortfolioView);

    m_priceSeries = new QLineSeries();
    m_chart = new QChart();
    m_chart->addSeries(m_priceSeries);
    m_chart->createDefaultAxes();
    m_chart->setTitle("价格走势");
    ui->chartView->setChart(m_chart);

    m_pieSeries = new QPieSeries();
    m_pieChart = new QChart();
    m_pieChart->addSeries(m_pieSeries);
    m_pieChart->setTitle("资产配置");
    ui->pieView->setChart(m_pieChart);

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &ExchangeDialog::updatePrices);
    connect(m_updateTimer, &QTimer::timeout, this, &ExchangeDialog::updatePortfolio);
    connect(m_updateTimer, &QTimer::timeout, this, &ExchangeDialog::updateEventTicker);
    m_updateTimer->start(100);
}

ExchangeDialog::~ExchangeDialog()
{
    delete ui;
}

void ExchangeDialog::setWallet(Wallet* wallet)
{
    m_wallet = wallet;
}

void ExchangeDialog::setAssets(QList<Asset*> assets)
{
    m_assets = assets;
    ui->assetList->clear();
    for (Asset* asset : assets) {
        QListWidgetItem* item = new QListWidgetItem(asset->name());
        item->setData(Qt::UserRole, QVariant::fromValue((void*)asset));
        ui->assetList->addItem(item);
    }
    if (!assets.isEmpty()) {
        onAssetSelected(0);
    }
}

void ExchangeDialog::setMarketEvent(MarketEvent* marketEvent)
{
    m_marketEvent = marketEvent;
}

void ExchangeDialog::onAssetSelected(int index)
{
    if (index < 0 || index >= m_assets.size()) return;
    m_selectedAsset = m_assets[index];
    updateChart();
    updatePortfolio();
}

void ExchangeDialog::onBuy()
{
    if (!m_wallet || !m_selectedAsset) return;

    double amount = ui->tradeAmount->value();
    if (m_wallet->merit() >= amount) {
        double price = m_selectedAsset->price();
        double shares = amount / price;
        m_wallet->spend(amount);
        m_wallet->addAsset(m_selectedAsset->id(), shares);
        updatePortfolio();
    }
}

void ExchangeDialog::onSell()
{
    if (!m_wallet || !m_selectedAsset) return;

    double shares = ui->tradeAmount->value();
    double price = m_selectedAsset->price();
    double amount = shares * price;
    if (m_wallet->getAssetShares(m_selectedAsset->id()) >= shares) {
        m_wallet->removeAsset(m_selectedAsset->id(), shares);
        m_wallet->earn(amount);
        updatePortfolio();
    }
}

void ExchangeDialog::updatePrices()
{
    if (m_selectedAsset) {
        ui->currentPrice->setText(QString("%1").arg(m_selectedAsset->price(), 0, 'f', 2));
        ui->change24h->setText(QString("%1%").arg(m_selectedAsset->change24h(), 0, 'f', 2));
        updateChart();
    }
}

void ExchangeDialog::updatePortfolio()
{
    if (!m_wallet || !m_selectedAsset) return;

    double shares = m_wallet->getAssetShares(m_selectedAsset->id());
    double price = m_selectedAsset->price();
    double value = shares * price;

    ui->shares->setText(QString("%1").arg(shares, 0, 'f', 4));
    ui->value->setText(QString("%1").arg(value, 0, 'f', 2));

    double totalValue = 0;
    for (Asset* asset : m_assets) {
        totalValue += m_wallet->getAssetShares(asset->id()) * asset->price();
    }
    ui->totalPortfolio->setText(QString("%1").arg(totalValue, 0, 'f', 2));
}

void ExchangeDialog::updateEventTicker()
{
    if (m_marketEvent && m_marketEvent->isActive()) {
        ui->eventTicker->setText(m_marketEvent->currentEvent());
        ui->eventTicker->setStyleSheet("color: orange;");
    } else {
        ui->eventTicker->setText("市场平稳运行中...");
        ui->eventTicker->setStyleSheet("");
    }
}

void ExchangeDialog::showPortfolioView()
{
    m_pieSeries->clear();
    double totalValue = 0;

    for (Asset* asset : m_assets) {
        double value = m_wallet->getAssetShares(asset->id()) * asset->price();
        totalValue += value;
        if (value > 0) {
            m_pieSeries->append(asset->name(), value);
        }
    }

    ui->tabWidget->setCurrentIndex(1);
    ui->portfolioRisk->setText(getPortfolioRiskRating());
}

void ExchangeDialog::updateChart()
{
    if (!m_selectedAsset) return;

    m_priceSeries->clear();
    const QVector<double>& history = m_selectedAsset->priceHistory();
    for (int i = 0; i < history.size(); ++i) {
        m_priceSeries->append(i, history[i]);
    }
    m_chart->createDefaultAxes();
}

QString ExchangeDialog::riskLevelToString(Asset::RiskLevel level)
{
    switch (level) {
    case Asset::Low: return "低";
    case Asset::MediumLow: return "中低";
    case Asset::Medium: return "中等";
    case Asset::High: return "高";
    default: return "未知";
    }
}

QString ExchangeDialog::getPortfolioRiskRating()
{
    if (!m_wallet) return "未知";

    double highRiskWeight = 0;
    double mediumRiskWeight = 0;
    double totalValue = 0;

    for (Asset* asset : m_assets) {
        double value = m_wallet->getAssetShares(asset->id()) * asset->price();
        totalValue += value;
        if (asset->riskLevel() == Asset::High) {
            highRiskWeight += value;
        } else if (asset->riskLevel() == Asset::Medium || asset->riskLevel() == Asset::MediumLow) {
            mediumRiskWeight += value;
        }
    }

    if (totalValue == 0) return "保守";

    double highRatio = highRiskWeight / totalValue;
    double mediumRatio = mediumRiskWeight / totalValue;

    if (highRatio > 0.5) return "赌徒";
    if (highRatio > 0.2) return "激进";
    if (mediumRatio > 0.5) return "均衡";
    return "保守";
}
