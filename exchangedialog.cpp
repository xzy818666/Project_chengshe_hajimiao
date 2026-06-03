#include "exchangedialog.h"
#include "ui_exchangedialog.h"
#include <QDateTime>
#include <QMessageBox>
#include <QLabel>
#include <QVBoxLayout>
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
    , m_calPortfolio(nullptr)
    , m_advisor(new PortfolioAdvisor(this))
    , m_scanner(new ArbitrageScanner(this))
    , m_autoInvestor(new AutoInvestor(this))
    , m_adviceLabel(nullptr)
    , m_arbitrageLabel(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("幻缘所");

    // 将 1674x940 的 UI 缩放到 1600x900（与功德堂主窗口一致）
    const double sx = 1600.0 / 1674.0;
    const double sy = 900.0 / 940.0;
    for (QWidget* w : findChildren<QWidget*>()) {
        QRect g = w->geometry();
        w->setGeometry(qRound(g.x() * sx), qRound(g.y() * sy),
                       qRound(g.width() * sx), qRound(g.height() * sy));
    }

    // 背景图 scaled 到 1600x900
    QPixmap bg(":/images/stock_exchange.png");
    bg = bg.scaled(1600, 900, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QBrush(bg));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    resize(1600, 900);
    setMinimumSize(800, 450);

    // ---- 控件古风样式 ----
    // 列表：完全透明背景 + 深色文字，直接融入背景面板
    ui->assetList->setStyleSheet(R"(
        QListWidget {
            background-color: transparent;
            border: none;
            color: #1A0F08;
            outline: none;
        }
        QListWidget::item {
            padding: 6px;
            border-bottom: 1px solid rgba(139, 90, 43, 0.15);
        }
        QListWidget::item:selected {
            background-color: rgba(139, 90, 43, 0.28);
            color: #1A0F08;
            border-radius: 4px;
        }
        QListWidget::item:hover {
            background-color: rgba(139, 90, 43, 0.15);
        }
    )");

    // 按钮：木褐色底 + 米白文字
    QString btnStyle = R"(
        QPushButton {
            background-color: rgba(139, 90, 43, 0.85);
            color: #FFF8E7;
            border: 1px solid rgba(100, 60, 20, 0.7);
            border-radius: 6px;
            padding: 4px;
        }
        QPushButton:hover {
            background-color: rgba(160, 110, 55, 0.95);
        }
        QPushButton:pressed {
            background-color: rgba(120, 75, 35, 0.95);
        }
        QPushButton:disabled {
            background-color: rgba(139, 90, 43, 0.35);
            color: #D7CCC8;
        }
    )";
    ui->buyBtn->setStyleSheet(btnStyle);
    ui->sellBtn->setStyleSheet(btnStyle);
    ui->tradingTabBtn->setStyleSheet(btnStyle);
    ui->portfolioTabBtn->setStyleSheet(btnStyle);

    // 输入框/下拉框：透明底 + 细边框，保留系统默认箭头按钮
    QString inputStyle = R"(
        QDoubleSpinBox, QComboBox {
            background-color: transparent;
            border: 1px solid rgba(139, 90, 43, 0.4);
            border-radius: 4px;
            color: #1A0F08;
            padding: 2px 4px;
        }
        QComboBox::drop-down {
            width: 20px;
            border-left: 1px solid rgba(139, 90, 43, 0.3);
        }
        QComboBox QAbstractItemView {
            background-color: rgba(250, 245, 235, 0.95);
            color: #1A0F08;
            selection-background-color: rgba(139, 90, 43, 0.25);
        }
    )";
    ui->tradeAmount->setStyleSheet(inputStyle);
    ui->leverageCombo->setStyleSheet(inputStyle);

    // 标签统一深色，确保在浅色面板上清晰可读
    QString labelStyle = "color: #1A0F08; font-size: 13px;";
    ui->currentPrice->setStyleSheet(labelStyle);
    ui->change24h->setStyleSheet(labelStyle);
    ui->shares->setStyleSheet(labelStyle);
    ui->value->setStyleSheet(labelStyle);
    ui->leverageLabel->setStyleSheet(labelStyle);
    ui->totalPortfolio->setStyleSheet(labelStyle);
    ui->portfolioRisk->setStyleSheet(labelStyle);
    ui->eventTicker->setStyleSheet("color: #1A0F08; font-size: 18px; font-weight: bold;");

    // 图表视图透明
    ui->chartView->setStyleSheet("background: transparent;");
    ui->pieView->setStyleSheet("background: transparent;");

    connect(ui->assetList, &QListWidget::currentRowChanged, this, &ExchangeDialog::onAssetSelected);
    connect(ui->buyBtn, &QPushButton::clicked, this, &ExchangeDialog::onBuy);
    connect(ui->sellBtn, &QPushButton::clicked, this, &ExchangeDialog::onSell);
    connect(ui->tradingTabBtn, &QPushButton::clicked, this, [this]() { setTradingMode(true); });
    connect(ui->portfolioTabBtn, &QPushButton::clicked, this, [this]() { setTradingMode(false); });

    ui->tradeAmount->setDecimals(4);
    ui->tradeAmount->setSingleStep(0.05);
    ui->tradeAmount->setLocale(QLocale::C);

    // 初始化杠杆选择（默认只有1×，等setMaxLeverage后更新）
    ui->leverageCombo->addItem("1×", 1.0);

    // 添加交易提示标签（放在 shares/value 与 tradeAmount 之间）
    QLabel* hintLabel = new QLabel("💡 买入时输入功德金额，卖出时输入持仓份额", ui->detailWidget);
    hintLabel->setGeometry(qRound(10 * sx), qRound(224 * sy), qRound(413 * sx), qRound(16 * sy));
    hintLabel->setAlignment(Qt::AlignCenter);
    hintLabel->setStyleSheet("color: #1A0F08; font-size: 11px;");

    // 显式确保 chartView 高度足够，并优化图表内部边距
    ui->chartView->setGeometry(qRound(10 * sx), qRound(22 * sy), qRound(413 * sx), qRound(180 * sy));
    ui->chartView->setMinimumHeight(qRound(180 * sy));
    ui->pieView->setMinimumHeight(qRound(200 * sy));

    m_priceSeries = new QLineSeries();
    m_chart = new QChart();
    m_chart->addSeries(m_priceSeries);
    m_chart->legend()->hide();
    m_chart->setTitle("价格走势");
    m_chart->setAnimationOptions(QChart::NoAnimation);
    m_chart->setBackgroundBrush(QBrush(QColor(0, 0, 0, 0)));
    m_chart->setPlotAreaBackgroundBrush(QBrush(QColor(245, 240, 230, 40)));
    m_chart->setPlotAreaBackgroundVisible(true);
    m_chart->setMargins(QMargins(8, 8, 8, 8));
    QFont chartTitleFont = m_chart->titleFont();
    chartTitleFont.setPointSize(10);
    m_chart->setTitleFont(chartTitleFont);
    ui->chartView->setChart(m_chart);

    m_pieSeries = new QPieSeries();
    m_pieChart = new QChart();
    m_pieChart->addSeries(m_pieSeries);
    m_pieChart->legend()->setAlignment(Qt::AlignRight);
    m_pieChart->setTitle("资产配置");
    m_pieChart->setBackgroundBrush(QBrush(QColor(0, 0, 0, 0)));
    m_pieChart->setPlotAreaBackgroundBrush(QBrush(QColor(245, 240, 230, 40)));
    m_pieChart->setPlotAreaBackgroundVisible(true);
    m_pieChart->setMargins(QMargins(8, 8, 8, 8));
    QFont pieTitleFont = m_pieChart->titleFont();
    pieTitleFont.setPointSize(10);
    m_pieChart->setTitleFont(pieTitleFont);
    ui->pieView->setChart(m_pieChart);

    // CAL 风险-收益图
    m_calScatter = new QScatterSeries();
    m_calScatter->setMarkerSize(10);
    m_calScatter->setColor(QColor("#1565C0"));
    m_calPortfolio = new QScatterSeries();
    m_calPortfolio->setMarkerSize(14);
    m_calPortfolio->setColor(QColor("#C62828"));
    m_calPortfolio->setBorderColor(QColor("#FFFFFF"));
    m_calLine = new QLineSeries();
    m_calLine->setColor(QColor("#C62828"));
    m_calLine->setPen(QPen(QColor("#C62828"), 2, Qt::DashLine));
    m_calChart = new QChart();
    m_calChart->addSeries(m_calScatter);
    m_calChart->addSeries(m_calPortfolio);
    m_calChart->addSeries(m_calLine);
    m_calChart->legend()->hide();
    m_calChart->setTitle("风险-收益 (CAL)");
    m_calChart->setBackgroundBrush(QBrush(QColor(0, 0, 0, 0)));
    m_calChart->setPlotAreaBackgroundBrush(QBrush(QColor(245, 240, 230, 40)));
    m_calChart->setPlotAreaBackgroundVisible(true);
    m_calChart->setMargins(QMargins(8, 8, 8, 8));
    QFont calTitleFont = m_calChart->titleFont();
    calTitleFont.setPointSize(10);
    m_calChart->setTitleFont(calTitleFont);
    ui->calView->setStyleSheet("background: transparent;");
    ui->calView->setChart(m_calChart);

    // 智能推荐与套利标签 — 最下面面板（横着排列）
    m_adviceLabel = new QLabel(" 智能推荐: 计算中...", this);
    m_adviceLabel->setGeometry(qRound(361 * sx), qRound(722 * sy), qRound(470 * sx), qRound(22 * sy));
    m_adviceLabel->setStyleSheet("color: #2E7D32; font-weight: bold;");

    m_arbitrageLabel = new QLabel(" 套利检测: 扫描中...", this);
    m_arbitrageLabel->setGeometry(qRound(850 * sx), qRound(722 * sy), qRound(470 * sx), qRound(22 * sy));
    m_arbitrageLabel->setStyleSheet("color: #1565C0;");

    // 自动定投控件 — 最下面面板
    QWidget* autoWidget = new QWidget(this);
    autoWidget->setGeometry(qRound(361 * sx), qRound(750 * sy), qRound(959 * sx), qRound(35 * sy));
    QHBoxLayout* autoLayout = new QHBoxLayout(autoWidget);
    autoLayout->setContentsMargins(0, 0, 0, 0);
    QCheckBox* autoCheck = new QCheckBox("自动定投", autoWidget);
    QSpinBox* intervalSpin = new QSpinBox(autoWidget);
    intervalSpin->setRange(10, 3600);
    intervalSpin->setSuffix("秒");
    intervalSpin->setValue(60);
    intervalSpin->setEnabled(false);
    QDoubleSpinBox* amountSpin = new QDoubleSpinBox(autoWidget);
    amountSpin->setRange(1, 100000);
    amountSpin->setValue(100);
    amountSpin->setSuffix("功德");
    amountSpin->setEnabled(false);

    QString autoLabelStyle = "color: #1A0F08; font-size: 13px;";
    QString autoInputStyle = R"(
        QSpinBox, QDoubleSpinBox {
            background-color: transparent;
            border: 1px solid rgba(139, 90, 43, 0.4);
            border-radius: 4px;
            color: #1A0F08;
            padding: 2px 4px;
        }
    )";
    QString checkStyle = R"(
        QCheckBox {
            color: #1A0F08;
            font-size: 13px;
            font-weight: bold;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid rgba(139, 90, 43, 0.7);
            border-radius: 4px;
            background-color: transparent;
        }
        QCheckBox::indicator:checked {
            background-color: rgba(139, 90, 43, 0.8);
        }
    )";
    autoCheck->setStyleSheet(checkStyle);
    intervalSpin->setStyleSheet(autoInputStyle);
    amountSpin->setStyleSheet(autoInputStyle);

    QLabel* intervalLabel = new QLabel("间隔", autoWidget);
    intervalLabel->setStyleSheet(autoLabelStyle);
    QLabel* amountLabel = new QLabel("金额", autoWidget);
    amountLabel->setStyleSheet(autoLabelStyle);

    autoLayout->addWidget(autoCheck);
    autoLayout->addWidget(intervalLabel);
    autoLayout->addWidget(intervalSpin);
    autoLayout->addWidget(amountLabel);
    autoLayout->addWidget(amountSpin);

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

    // 默认显示交易模式
    setTradingMode(true);
}

ExchangeDialog::~ExchangeDialog()
{
    delete ui;
}

void ExchangeDialog::setWallet(Wallet* wallet)
{
    m_wallet = wallet;
}

static QWidget* createAssetItemWidget(Asset* asset)
{
    QWidget* row = new QWidget();
    row->setAutoFillBackground(false);
    row->setAttribute(Qt::WA_TransparentForMouseEvents);
    QVBoxLayout* layout = new QVBoxLayout(row);
    layout->setContentsMargins(10, 6, 10, 6);
    layout->setSpacing(3);

    QLabel* nameLabel = new QLabel(asset->name());
    nameLabel->setStyleSheet("font-weight: bold; font-size: 15px; color: #1A0F08;");

    QString abstract;
    QString specs;
    if (asset->id() == "merit_index") {
        abstract = QStringLiteral("诸行无常，缘起性空。大千世界，众生共业，潮起潮落，皆为我缘。");
        specs = QStringLiteral("风险：中等 | 漂移：0.02 | 波动：0.15 | 初始价：100");
    } else if (asset->id() == "karma_bond") {
        abstract = QStringLiteral("积善之家，必有余庆。不求功德无量，但求灵台安宁。");
        specs = QStringLiteral("风险：低 | 价格锚定：1.0 | 年化收益：4%（按秒结息）");
    } else if (asset->id() == "dharma_fund") {
        abstract = QStringLiteral("诸法因缘生，诸法因缘灭。左手托众生，右手抚私德，对冲于冥冥之中。");
        specs = QStringLiteral("风险：中低 | 初始价：100 | 合成：60%众生法缘+40%善行福缘+噪声");
    } else if (asset->id() == "samsara_futures") {
        abstract = QStringLiteral("一念天堂，一念地狱。携两倍因果业火，于无常涅槃重生，赌尽前世来生。");
        specs = QStringLiteral("风险：高 | 初始价：100 | 自带2×杠杆 | 维持率<30%强制平仓 | 亏损先扣下世功德");
    } else {
        abstract = QStringLiteral("未知资产");
        specs = QStringLiteral("—");
    }

    QLabel* descLabel = new QLabel(abstract);
    descLabel->setStyleSheet("font-size: 11px; color: #5D4037;");
    descLabel->setWordWrap(true);

    QLabel* specLabel = new QLabel(specs);
    specLabel->setStyleSheet("font-size: 10px; color: #6D4C41;");
    specLabel->setWordWrap(true);

    layout->addWidget(nameLabel);
    layout->addWidget(descLabel);
    layout->addWidget(specLabel);
    layout->addStretch();

    return row;
}

void ExchangeDialog::setAssets(QList<Asset*> assets)
{
    m_assets = assets;
    ui->assetList->clear();

    int listH = ui->assetList->height();
    int itemH = listH / qMax(1, assets.size());

    for (Asset* asset : assets) {
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(Qt::UserRole, QVariant::fromValue((void*)asset));
        item->setSizeHint(QSize(ui->assetList->viewport()->width(), itemH));
        ui->assetList->addItem(item);
        ui->assetList->setItemWidget(item, createAssetItemWidget(asset));
    }
    if (!assets.isEmpty()) {
        onAssetSelected(0);
    }
}

void ExchangeDialog::setMarketEvent(MarketEvent* marketEvent)
{
    m_marketEvent = marketEvent;
}

void ExchangeDialog::setMaxLeverage(double maxLeverage)
{
    m_maxLeverage = maxLeverage;
    updateLeverageCombo();
}

void ExchangeDialog::setTradingMode(bool trading)
{
    // 左面板
    ui->assetList->setVisible(trading);
    ui->pieView->setVisible(!trading);

    // 右面板
    ui->detailWidget->setVisible(trading);
    ui->portfolioRightWidget->setVisible(!trading);

    // 标签按钮状态
    ui->tradingTabBtn->setEnabled(!trading);
    ui->portfolioTabBtn->setEnabled(trading);

    // 切换到资产配置时更新饼图和 CAL 图
    if (!trading) {
        showPortfolioView();
        updateCalChart();
    }
}

void ExchangeDialog::updateLeverageCombo()
{
    ui->leverageCombo->clear();
    ui->leverageCombo->addItem("1×", 1.0);
    if (m_maxLeverage >= 1.5) ui->leverageCombo->addItem("1.5×", 1.5);
    if (m_maxLeverage >= 2.0) ui->leverageCombo->addItem("2×", 2.0);
    if (m_maxLeverage >= 3.0) ui->leverageCombo->addItem("3×", 3.0);
}

void ExchangeDialog::onAssetSelected(int index)
{
    if (index < 0 || index >= m_assets.size()) return;
    m_selectedAsset = m_assets[index];
    updateChart();
    updatePortfolio();

    // 轮回孽缘不支持本杠杆系统，隐藏杠杆选择
    bool isSamsara = (m_selectedAsset->id() == "samsara_futures");
    ui->leverageLabel->setVisible(!isSamsara);
    ui->leverageCombo->setVisible(!isSamsara);
}

void ExchangeDialog::onBuy()
{
    if (!m_wallet || !m_selectedAsset) return;

    double amount = ui->tradeAmount->value();
    if (amount <= 0) {
        QMessageBox::information(this, "买入失败", "请输入有效的买入金额");
        return;
    }

    double leverage = ui->leverageCombo->currentData().toDouble();
    if (leverage <= 0) leverage = 1.0;

    // 轮回孽缘不支持杠杆系统
    if (m_selectedAsset->id() == "samsara_futures") {
        leverage = 1.0;
    }

    double price = m_selectedAsset->price();
    double shares = amount / price;

    if (leverage > 1.0 && m_selectedAsset->id() != "samsara_futures") {
        // 杠杆买入
        double principal = amount / leverage;
        double borrowed = amount - principal;

        if (m_wallet->merit() < principal) {
            QMessageBox::information(this, "买入失败", "功德不足以支付保证金");
            return;
        }
        if (borrowed > m_wallet->maxBorrow() * 0.5) {
            QMessageBox::information(this, "买入失败",
                QString("杠杆借款 %1 超过可用信用额度的50%%上限（%2）")
                    .arg(borrowed, 0, 'f', 2).arg(m_wallet->maxBorrow() * 0.5, 0, 'f', 2));
            return;
        }

        m_wallet->spend(principal);
        m_wallet->openLeveragePosition(m_selectedAsset->id(), shares, principal, borrowed, leverage);
        m_wallet->recordAssetBuy(m_selectedAsset->id(), shares, amount);
        updatePortfolio();
        QMessageBox::information(this, "杠杆买入成功",
            QString("以 %1× 杠杆买入 %2 %3 份\n本金: %4 功德 | 借款: %5 功德")
                .arg(leverage, 0, 'f', 1).arg(m_selectedAsset->name()).arg(shares, 0, 'f', 4)
                .arg(principal, 0, 'f', 2).arg(borrowed, 0, 'f', 2));
    } else {
        // 普通买入
        if (m_wallet->merit() >= amount) {
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

    if (available < shares) {
        QMessageBox::information(this, "卖出失败",
            QString("持仓不足，当前持有 %1 份").arg(available, 0, 'f', 4));
        return;
    }

    double price = m_selectedAsset->price();

    // 先处理杠杆持仓（如果有）
    double leverageShares = 0;
    for (const auto& pos : m_wallet->leveragePositions()) {
        if (pos.assetId == m_selectedAsset->id()) {
            leverageShares = pos.shares;
            break;
        }
    }

    double sharesToSell = shares;
    if (leverageShares > 0 && m_selectedAsset->id() != "samsara_futures") {
        double levSell = qMin(sharesToSell, leverageShares);
        m_wallet->closeLeveragePosition(m_selectedAsset->id(), levSell, price);
        sharesToSell -= levSell;
    }

    // 剩余部分按普通持仓卖出
    if (sharesToSell > 0.0001) {
        double amount = sharesToSell * price;

        // 轮回孽缘：亏损优先扣减下世功德
        if (m_selectedAsset->id() == "samsara_futures") {
            double costBasis = m_wallet->getAssetCostBasis(m_selectedAsset->id());
            if (available > 0 && costBasis > 0) {
                double avgCost = costBasis / available;
                double myCost = avgCost * sharesToSell;
                if (amount < myCost) {
                    m_wallet->applySamsaraLoss(myCost - amount);
                }
            }
        }

        m_wallet->removeAsset(m_selectedAsset->id(), sharesToSell);
        m_wallet->recordAssetSell(m_selectedAsset->id(), sharesToSell, amount);
        m_wallet->earn(amount);
    }

    updatePortfolio();
    QMessageBox::information(this, "卖出成功",
        QString("卖出 %1 %2 份，当前价格 %3").arg(m_selectedAsset->name()).arg(shares, 0, 'f', 4).arg(price, 0, 'f', 2));
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
    if (!m_wallet) return;

    if (m_selectedAsset) {
        double shares = m_wallet->getAssetShares(m_selectedAsset->id());
        double price = m_selectedAsset->price();
        double value = shares * price;

        ui->shares->setText(QString("持仓份额: %1").arg(shares, 0, 'f', 4));
        ui->value->setText(QString("市值: %1").arg(value, 0, 'f', 2));
    }

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
        ui->eventTicker->setStyleSheet("color: #E65100; font-size: 18px; font-weight: bold;");
    } else {
        ui->eventTicker->setText("市场平稳运行中...");
        ui->eventTicker->setStyleSheet("color: #1A0F08; font-size: 18px; font-weight: bold;");
    }
}

void ExchangeDialog::showPortfolioView()
{
    if (!m_wallet) return;

    m_pieSeries->clear();

    for (Asset* asset : m_assets) {
        double value = m_wallet->getAssetShares(asset->id()) * asset->price();
        if (value > 0) {
            m_pieSeries->append(asset->name(), value);
        }
    }

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
    if (text == " 智能推荐: ") {
        text += "暂无明确建议";
    }
    m_adviceLabel->setText(text);
}

void ExchangeDialog::updateArbitrage()
{
    if (!m_scanner || m_assets.isEmpty()) return;
    auto ops = m_scanner->scan(m_assets);
    if (ops.isEmpty()) {
        m_arbitrageLabel->setText(" 套利检测: 未发现机会");
        m_arbitrageLabel->setStyleSheet("color: #1565C0;");
    } else {
        QString text = " 套利检测: ";
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
    // 保留接口，实际值通过信号自动传递
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
            axisX->setLabelsColor(QColor("#1A0F08"));
        }
        if (axisY) {
            axisY->setLabelFormat("%.2f");
            axisY->setLabelsColor(QColor("#1A0F08"));
        }
    } else {
        auto* axisX = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Horizontal).first());
        auto* axisY = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Vertical).first());
        if (axisX) {
            axisX->setRange(0, DAY_TICKS - 1);
            axisX->setLabelsColor(QColor("#1A0F08"));
        }
        if (axisY) {
            axisY->setLabelsColor(QColor("#1A0F08"));
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

void ExchangeDialog::updateCalChart()
{
    if (!m_advisor || m_assets.isEmpty() || !m_wallet) return;

    auto advice = m_advisor->advise(m_assets);
    m_calScatter->clear();
    m_calPortfolio->clear();
    m_calLine->clear();

    // 清理旧标签
    for (QGraphicsItem* item : m_calLabels) {
        if (item->scene()) item->scene()->removeItem(item);
        delete item;
    }
    m_calLabels.clear();

    const double rfRate = 0.04 / 86400.0;
    const double SCALE = 10000.0;
    double maxSharpe = -999.0;
    double bestRisk = 0.0;
    double bestReturn = rfRate;

    double minX = 0.0, maxX = 0.0;
    double minY = rfRate * SCALE, maxY = rfRate * SCALE;

    double totalValue = 0.0;
    for (Asset* asset : m_assets) {
        totalValue += m_wallet->getAssetShares(asset->id()) * asset->price();
    }

    double portReturn = 0.0;
    double portRiskSq = 0.0;
    QMap<QString, double> assetPrices;
    for (Asset* asset : m_assets) {
        assetPrices[asset->id()] = asset->price();
    }

    for (const auto& a : advice) {
        if (a.risk > 1e-10) {
            double sx = a.risk * SCALE;
            double sy = a.expectedReturn * SCALE;
            m_calScatter->append(sx, sy);
            minX = qMin(minX, sx);
            maxX = qMax(maxX, sx);
            minY = qMin(minY, sy);
            maxY = qMax(maxY, sy);

            double sharpe = (a.expectedReturn - rfRate) / a.risk;
            if (sharpe > maxSharpe) {
                maxSharpe = sharpe;
                bestRisk = a.risk;
                bestReturn = a.expectedReturn;
            }

            // 计算当前组合点（简化：假设资产间不相关）
            if (totalValue > 0) {
                double shares = m_wallet->getAssetShares(a.assetId);
                double price = assetPrices.value(a.assetId, 0.0);
                double assetValue = shares * price;
                double w = assetValue / totalValue;
                portReturn += w * a.expectedReturn;
                portRiskSq += w * w * a.risk * a.risk;
            }
        }
    }

    // 当前资产配置点
    if (totalValue > 0 && portRiskSq > 0) {
        double portRisk = qSqrt(portRiskSq);
        m_calPortfolio->append(portRisk * SCALE, portReturn * SCALE);
        minX = qMin(minX, portRisk * SCALE);
        maxX = qMax(maxX, portRisk * SCALE);
        minY = qMin(minY, portReturn * SCALE);
        maxY = qMax(maxY, portReturn * SCALE);
    }

    // 坐标轴范围先确定（包含 CAL 射线延长点）
    double xPad = (maxX - minX) * 0.15 + 0.1;
    double yPad = (maxY - minY) * 0.15 + 0.1;
    double axisXMax = maxX + xPad;
    double axisYMin = minY - yPad;
    double axisYMax = maxY + yPad;

    // CAL 射线：从无风险利率点出发，穿过最优资产点，延伸到右边界
    double calStartX = 0.0;
    double calStartY = rfRate * SCALE;
    double calEndX = axisXMax;
    double calEndY = calStartY;
    if (bestRisk > 1e-10) {
        double slope = (bestReturn * SCALE - calStartY) / (bestRisk * SCALE);
        calEndY = calStartY + slope * calEndX;
        axisYMax = qMax(axisYMax, calEndY + yPad);
    }
    m_calLine->append(calStartX, calStartY);
    m_calLine->append(calEndX, calEndY);

    if (m_calChart->axes(Qt::Horizontal).isEmpty()) {
        m_calChart->createDefaultAxes();
    }
    auto* axisX = qobject_cast<QValueAxis*>(m_calChart->axes(Qt::Horizontal).first());
    auto* axisY = qobject_cast<QValueAxis*>(m_calChart->axes(Qt::Vertical).first());
    if (axisX) {
        axisX->setTitleText("风险 (标准差 ×10⁴)");
        axisX->setLabelsColor(QColor("#1A0F08"));
        axisX->setLabelFormat("%.2f");
        axisX->setRange(qMax(0.0, minX - xPad), axisXMax);
    }
    if (axisY) {
        axisY->setTitleText("期望收益 (×10⁴)");
        axisY->setLabelsColor(QColor("#1A0F08"));
        axisY->setLabelFormat("%.2f");
        axisY->setRange(axisYMin, axisYMax);
    }

    // 添加资产名称标签（mapToPosition 转 scene 坐标）
    QGraphicsScene* scene = ui->calView->scene();
    if (!scene) return;

    for (const auto& a : advice) {
        if (a.risk > 1e-10) {
            QPointF chartPos = m_calChart->mapToPosition(QPointF(a.risk * SCALE, a.expectedReturn * SCALE), m_calScatter);
            QPointF scenePos = m_calChart->mapToScene(chartPos);
            QGraphicsTextItem* label = new QGraphicsTextItem(a.assetName);
            label->setDefaultTextColor(QColor("#1A0F08"));
            QFont font = label->font();
            font.setPointSize(8);
            label->setFont(font);
            label->setPos(scenePos.x() + 4, scenePos.y() - 6);
            scene->addItem(label);
            m_calLabels.append(label);
        }
    }

    // 组合点标签
    if (totalValue > 0 && portRiskSq > 0) {
        double portRisk = qSqrt(portRiskSq);
        QPointF chartPos = m_calChart->mapToPosition(QPointF(portRisk * SCALE, portReturn * SCALE), m_calPortfolio);
        QPointF scenePos = m_calChart->mapToScene(chartPos);
        QGraphicsTextItem* label = new QGraphicsTextItem("当前配置");
        label->setDefaultTextColor(QColor("#C62828"));
        QFont font = label->font();
        font.setPointSize(8);
        font.setBold(true);
        label->setFont(font);
        label->setPos(scenePos.x() + 4, scenePos.y() - 6);
        scene->addItem(label);
        m_calLabels.append(label);
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
