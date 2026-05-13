#include "exchangedialog.h"
#include "ui_exchangedialog.h"
#include <QDateTime>
#include <QMessageBox>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include "portfolioadvisor.h"
#include "arbitragescanner.h"
#include "autoinvestor.h"

ExchangeDialog::ExchangeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ExchangeDialog)
    , m_wallet(nullptr)
    , m_selectedAsset(nullptr)
    , m_priceSeries(nullptr)
    , m_chart(nullptr)
    , m_pieSeries(nullptr)
    , m_pieChart(nullptr)
    , m_advisor(new PortfolioAdvisor(this))
    , m_scanner(new ArbitrageScanner(this))
    , m_autoInvestor(new AutoInvestor(this))
    , m_adviceLabel(nullptr)
    , m_arbitrageLabel(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("幻缘所");

    connect(ui->assetList, &QListWidget::currentRowChanged, this, &ExchangeDialog::onAssetSelected);
    connect(ui->buyBtn, &QPushButton::clicked, this, &ExchangeDialog::onBuy);
    connect(ui->sellBtn, &QPushButton::clicked, this, &ExchangeDialog::onSell);
    connect(ui->portfolioBtn, &QPushButton::clicked, this, &ExchangeDialog::showPortfolioView);

    ui->tradeAmount->setDecimals(4);
    ui->tradeAmount->setSingleStep(0.0001);

    // 添加交易提示标签
    QLabel* hintLabel = new QLabel("💡 买入时输入功德金额，卖出时输入持仓份额", this);
    hintLabel->setAlignment(Qt::AlignCenter);
    hintLabel->setStyleSheet("color: #666; font-size: 11px;");
    int btnIdx = ui->detailLayout->indexOf(ui->tradeButtons);
    ui->detailLayout->insertWidget(btnIdx, hintLabel);

    ui->chartView->setMinimumHeight(200);
    ui->pieView->setMinimumHeight(200);

    m_priceSeries = new QLineSeries();
    m_chart = new QChart();
    m_chart->addSeries(m_priceSeries);
    m_chart->legend()->hide();
    m_chart->setTitle("价格走势");
    m_chart->setAnimationOptions(QChart::NoAnimation);
    ui->chartView->setChart(m_chart);

    m_pieSeries = new QPieSeries();
    m_pieChart = new QChart();
    m_pieChart->addSeries(m_pieSeries);
    m_pieChart->legend()->setAlignment(Qt::AlignRight);
    m_pieChart->setTitle("资产配置");
    ui->pieView->setChart(m_pieChart);

    // 智能推荐与套利标签
    m_adviceLabel = new QLabel("📊 智能推荐: 计算中...", this);
    m_adviceLabel->setStyleSheet("color: #2E7D32; font-weight: bold;");
    m_arbitrageLabel = new QLabel("🔍 套利检测: 扫描中...", this);
    m_arbitrageLabel->setStyleSheet("color: #1565C0;");
    int eventIdx = ui->verticalLayout->indexOf(ui->eventTicker);
    ui->verticalLayout->insertWidget(eventIdx + 1, m_adviceLabel);
    ui->verticalLayout->insertWidget(eventIdx + 2, m_arbitrageLabel);

    // 自动定投控件
    QWidget* autoWidget = new QWidget(this);
    QHBoxLayout* autoLayout = new QHBoxLayout(autoWidget);
    autoLayout->setContentsMargins(0, 0, 0, 0);
    QCheckBox* autoCheck = new QCheckBox("自动定投", this);
    QSpinBox* intervalSpin = new QSpinBox(this);
    intervalSpin->setRange(10, 3600);
    intervalSpin->setSuffix("秒");
    intervalSpin->setValue(60);
    intervalSpin->setEnabled(false);
    QDoubleSpinBox* amountSpin = new QDoubleSpinBox(this);
    amountSpin->setRange(1, 100000);
    amountSpin->setValue(100);
    amountSpin->setSuffix("功德");
    amountSpin->setEnabled(false);
    autoLayout->addWidget(autoCheck);
    autoLayout->addWidget(new QLabel("间隔", this));
    autoLayout->addWidget(intervalSpin);
    autoLayout->addWidget(new QLabel("金额", this));
    autoLayout->addWidget(amountSpin);
    ui->verticalLayout->insertWidget(ui->verticalLayout->count() - 1, autoWidget);

    connect(autoCheck, &QCheckBox::toggled, intervalSpin, &QSpinBox::setEnabled);
    connect(autoCheck, &QCheckBox::toggled, amountSpin, &QDoubleSpinBox::setEnabled);
    connect(autoCheck, &QCheckBox::toggled, this, &ExchangeDialog::onAutoInvestToggled);
    connect(intervalSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &ExchangeDialog::onAutoInvestSettingsChanged);
    connect(amountSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ExchangeDialog::onAutoInvestSettingsChanged);

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &ExchangeDialog::updatePrices);
    connect(m_updateTimer, &QTimer::timeout, this, &ExchangeDialog::updatePortfolio);
    connect(m_updateTimer, &QTimer::timeout, this, &ExchangeDialog::updateEventTicker);
    connect(m_updateTimer, &QTimer::timeout, this, &ExchangeDialog::updateAdvice);
    connect(m_updateTimer, &QTimer::timeout, this, &ExchangeDialog::updateArbitrage);
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
    if (amount <= 0) {
        QMessageBox::information(this, "买入失败", "请输入有效的买入金额");
        return;
    }
    if (m_wallet->merit() >= amount) {
        double price = m_selectedAsset->price();
        double shares = amount / price;
        m_wallet->spend(amount);
        m_wallet->addAsset(m_selectedAsset->id(), shares);
        m_wallet->recordAssetBuy(m_selectedAsset->id(), shares, amount);
        updatePortfolio();
        QMessageBox::information(this, "买入成功",
            QString("以 %1 功德买入 %2 %3 份").arg(amount, 0, 'f', 2).arg(m_selectedAsset->name()).arg(shares, 0, 'f', 4));
    } else {
        QMessageBox::information(this, "买入失败", "功德不足");
    }
}

void ExchangeDialog::onSell()
{
    if (!m_wallet || !m_selectedAsset) return;

    double shares = ui->tradeAmount->value();
    if (shares <= 0) {
        QMessageBox::information(this, "卖出失败", "请输入有效的卖出份额");
        return;
    }

    double available = m_wallet->getAssetShares(m_selectedAsset->id());
    // 处理浮点精度：输入值非常接近持仓时视为全部卖出
    if (qFuzzyCompare(shares, available) || (shares > available && shares - available < 0.0001)) {
        shares = available;
    }

    if (available >= shares) {
        double price = m_selectedAsset->price();
        double amount = shares * price;
        m_wallet->removeAsset(m_selectedAsset->id(), shares);
        m_wallet->recordAssetSell(m_selectedAsset->id(), shares, amount);
        m_wallet->earn(amount);
        updatePortfolio();
        QMessageBox::information(this, "卖出成功",
            QString("卖出 %1 %2 份，获得 %3 功德").arg(m_selectedAsset->name()).arg(shares, 0, 'f', 4).arg(amount, 0, 'f', 2));
    } else {
        QMessageBox::information(this, "卖出失败",
            QString("持仓不足，当前持有 %1 份").arg(available, 0, 'f', 4));
    }
}

void ExchangeDialog::updatePrices()
{
    if (m_selectedAsset) {
        ui->currentPrice->setText(QString("价格: %1").arg(m_selectedAsset->price(), 0, 'f', 2));
        ui->change24h->setText(QString("24h: %1%").arg(m_selectedAsset->change24h(), 0, 'f', 2));
        updateChart();
    }
    if (m_autoInvestor && m_wallet) {
        m_autoInvestor->update(0.1, m_wallet, m_assets, m_advisor);
    }
}

void ExchangeDialog::updatePortfolio()
{
    if (!m_wallet || !m_selectedAsset) return;

    double shares = m_wallet->getAssetShares(m_selectedAsset->id());
    double price = m_selectedAsset->price();
    double value = shares * price;

    ui->shares->setText(QString("持仓份额: %1").arg(shares, 0, 'f', 4));
    ui->value->setText(QString("市值: %1").arg(value, 0, 'f', 2));

    double totalValue = 0;
    for (Asset* asset : m_assets) {
        totalValue += m_wallet->getAssetShares(asset->id()) * asset->price();
    }
    ui->totalPortfolio->setText(QString("总市值: %1").arg(totalValue, 0, 'f', 2));
}

void ExchangeDialog::updateEventTicker()
{
    if (m_marketEvent && m_marketEvent->isActive()) {
        ui->eventTicker->setText(m_marketEvent->currentEvent());
        ui->eventTicker->setStyleSheet("color: orange; font-weight: bold;");
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
    ui->portfolioRisk->setText(QString("风险评级: %1").arg(getPortfolioRiskRating()));
}

void ExchangeDialog::updateAdvice()
{
    if (!m_advisor || m_assets.isEmpty()) return;
    auto advice = m_advisor->advise(m_assets);
    QString text = "📊 智能推荐: ";
    for (const auto& a : advice) {
        if (a.weight > 0.01) {
            text += QString("%1(%2%) ").arg(a.assetName).arg(a.weight * 100, 0, 'f', 0);
        }
    }
    if (text == "📊 智能推荐: ") {
        text += "暂无明确建议";
    }
    m_adviceLabel->setText(text);
}

void ExchangeDialog::updateArbitrage()
{
    if (!m_scanner || m_assets.isEmpty()) return;
    auto ops = m_scanner->scan(m_assets);
    if (ops.isEmpty()) {
        m_arbitrageLabel->setText("🔍 套利检测: 未发现机会");
        m_arbitrageLabel->setStyleSheet("color: #1565C0;");
    } else {
        QString text = "⚡ 套利检测: ";
        for (const auto& op : ops) {
            text += op.description + " ";
        }
        m_arbitrageLabel->setText(text);
        m_arbitrageLabel->setStyleSheet("color: #C62828; font-weight: bold;");
    }
}

void ExchangeDialog::onAutoInvestToggled(bool checked)
{
    m_autoInvestor->setEnabled(checked);
    onAutoInvestSettingsChanged();
}

void ExchangeDialog::onAutoInvestSettingsChanged()
{
    if (QSpinBox* interval = findChild<QSpinBox*>()) {
        // 可能找到多个 QSpinBox，我们需要通过 objectName 区分
        // 但上面的 lambda 已经通过 sender 处理了。这个方法其实不需要了。
        // 不，connect 已经把值传给了 AutoInvestor。这里只是保留接口。
    }
}

void ExchangeDialog::updateChart()
{
    if (!m_selectedAsset) return;

    const QVector<double>& history = m_selectedAsset->priceHistory();
    if (history.isEmpty()) return;

    const int DAY_TICKS = 100; // display last 10 seconds (100 ticks at 100ms)
    int window = qMin(history.size(), DAY_TICKS);
    int start = history.size() - window;

    // 全量替换窗口内的点，确保滑动窗口正确左移
    QVector<QPointF> points;
    points.reserve(window);
    for (int i = 0; i < window; ++i) {
        points.append(QPointF(i, history[start + i]));
    }
    m_priceSeries->replace(points);

    if (m_chart->axes(Qt::Horizontal).isEmpty()) {
        m_chart->createDefaultAxes();
        auto* axisX = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Horizontal).first());
        auto* axisY = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Vertical).first());
        if (axisX) {
            axisX->setLabelFormat("%d");
            axisX->setRange(0, DAY_TICKS - 1);
        }
        if (axisY) {
            axisY->setLabelFormat("%.2f");
        }
    } else {
        auto* axisX = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Horizontal).first());
        auto* axisY = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Vertical).first());
        if (axisX) {
            axisX->setRange(0, DAY_TICKS - 1);
        }
        if (axisY) {
            double minPrice = *std::min_element(history.begin() + start, history.end());
            double maxPrice = *std::max_element(history.begin() + start, history.end());
            if (qFuzzyCompare(minPrice, maxPrice)) {
                axisY->setRange(minPrice * 0.95, maxPrice * 1.05);
            } else {
                double padding = (maxPrice - minPrice) * 0.1;
                axisY->setRange(minPrice - padding, maxPrice + padding);
            }
        }
    }
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
