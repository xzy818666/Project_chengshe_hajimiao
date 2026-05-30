#include "bankdialog.h"
#include "ui_bankdialog.h"
#include <QMessageBox>
#include <QTableWidgetItem>

const double BankDialog::FIXED_RATE_7DAY = 0.06;
const double BankDialog::FIXED_RATE_30DAY = 0.08;
const double BankDialog::FIXED_RATE_90DAY = 0.12;

BankDialog::BankDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BankDialog)
    , m_wallet(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("善财司");

    // 设置 16:9 背景图
    setFixedSize(960, 540);
    QPixmap bg(":/images/merit_bank.jpeg");
    QPalette palette;
    palette.setBrush(QPalette::Window, bg.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    setPalette(palette);
    setAutoFillBackground(true);

    // 让标签页占据右半部分（spacer 与 tabWidget 各占 1:1）
    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);
    ui->horizontalLayout->setStretch(0, 1);
    ui->horizontalLayout->setStretch(1, 1);

    // 标签页半透明，透出背景图
    ui->tabWidget->setStyleSheet(
        "QTabWidget::pane { "
        "  background-color: rgba(30, 40, 60, 0.40); "
        "  border: 1px solid rgba(255, 215, 100, 0.25); "
        "} "
        "QTabBar::tab { "
        "  background-color: rgba(45, 45, 45, 0.50); "
        "  color: #E0E0E0; "
        "  padding: 8px 16px; "
        "} "
        "QTabBar::tab:selected { "
        "  background-color: rgba(30, 40, 60, 0.65); "
        "  color: #FFD700; "
        "} "
        "QTabBar::tab:hover { "
        "  background-color: rgba(60, 60, 60, 0.60); "
        "}"
    );
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        ui->tabWidget->widget(i)->setAutoFillBackground(false);
    }

    connect(ui->depositBtn, &QPushButton::clicked, this, &BankDialog::onDepositSavings);
    connect(ui->withdrawBtn, &QPushButton::clicked, this, &BankDialog::onWithdrawSavings);
    connect(ui->buyFD7Btn, &QPushButton::clicked, [this]() { onBuyFixedDeposit(7, FIXED_RATE_7DAY); });
    connect(ui->buyFD30Btn, &QPushButton::clicked, [this]() { onBuyFixedDeposit(30, FIXED_RATE_30DAY); });
    connect(ui->buyFD90Btn, &QPushButton::clicked, [this]() { onBuyFixedDeposit(90, FIXED_RATE_90DAY); });
    connect(ui->withdrawFDBtn, &QPushButton::clicked, this, &BankDialog::onWithdrawFixedDeposit);
    connect(ui->borrowBtn, &QPushButton::clicked, this, &BankDialog::onBorrow);
    connect(ui->repayBtn, &QPushButton::clicked, this, &BankDialog::onRepay);
    connect(ui->confessBtn, &QPushButton::clicked, this, &BankDialog::onConfess);
    connect(ui->addMarginBtn, &QPushButton::clicked, this, &BankDialog::onAddMargin);
    connect(ui->closeAllLeverageBtn, &QPushButton::clicked, this, &BankDialog::onCloseAllLeverage);
}

BankDialog::~BankDialog()
{
    delete ui;
}

void BankDialog::setWallet(Wallet* wallet)
{
    m_wallet = wallet;
    connect(m_wallet, &Wallet::meritChanged, this, &BankDialog::updateInfo);
    connect(m_wallet, &Wallet::debtChanged, this, &BankDialog::updateInfo);
    connect(m_wallet, &Wallet::yezhangChanged, this, &BankDialog::updateInfo);
    connect(m_wallet, &Wallet::leverageChanged, this, &BankDialog::updateLeverageTable);
    updateInfo();
    updateFixedDeposits();
    updateLeverageTable();
}

void BankDialog::setAssets(QList<Asset*> assets)
{
    m_assets = assets;
}

void BankDialog::onDepositSavings()
{
    if (!m_wallet) return;
    double amount = ui->savingsAmount->value();
    m_wallet->addSavings(amount);
    updateInfo();
}

void BankDialog::onWithdrawSavings()
{
    if (!m_wallet) return;
    double amount = ui->savingsAmount->value();
    m_wallet->withdrawSavings(amount);
    updateInfo();
}

void BankDialog::onBuyFixedDeposit(int days, double rate)
{
    if (!m_wallet) return;
    double amount = ui->fdAmount->value();
    QString id = m_wallet->addFixedDeposit(amount, days, rate);
    if (!id.isEmpty()) {
        updateFixedDeposits();
        updateInfo();
    }
}

void BankDialog::onWithdrawFixedDeposit()
{
    if (!m_wallet) return;
    QListWidgetItem* item = ui->fdList->currentItem();
    if (item) {
        QString id = item->data(Qt::UserRole).toString();
        m_wallet->withdrawFixedDeposit(id);
        updateFixedDeposits();
        updateInfo();
    }
}

void BankDialog::onBorrow()
{
    if (!m_wallet) return;
    double amount = ui->borrowAmount->value();
    m_wallet->borrow(amount);
    updateInfo();
}

void BankDialog::onRepay()
{
    if (!m_wallet) return;
    double amount = ui->repayAmount->value();
    m_wallet->repay(amount);
    updateInfo();
}

void BankDialog::onConfess()
{
    if (!m_wallet) return;
    double amount = ui->confessAmount->value();
    m_wallet->reduceYezhang(amount);
    updateInfo();
}

void BankDialog::onAddMargin()
{
    if (!m_wallet) return;
    int row = ui->leverageTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "提示", "请先选中一条杠杆持仓");
        return;
    }
    QString assetId = ui->leverageTable->item(row, 0)->data(Qt::UserRole).toString();
    double amount = ui->savingsAmount->value(); // 复用活期金额输入框
    if (amount <= 0) {
        QMessageBox::information(this, "提示", "请输入有效的追加金额");
        return;
    }
    if (m_wallet->addMarginToPosition(assetId, amount)) {
        updateLeverageTable();
        updateInfo();
        QMessageBox::information(this, "追加成功", "保证金已追加");
    } else {
        QMessageBox::information(this, "追加失败", "功德不足或无该持仓");
    }
}

void BankDialog::onCloseAllLeverage()
{
    if (!m_wallet) return;
    auto positions = m_wallet->leveragePositions();
    if (positions.isEmpty()) {
        QMessageBox::information(this, "提示", "当前没有杠杆持仓");
        return;
    }

    int closed = 0;
    for (const auto& pos : positions) {
        Asset* asset = nullptr;
        for (Asset* a : m_assets) {
            if (a->id() == pos.assetId) {
                asset = a;
                break;
            }
        }
        if (asset) {
            m_wallet->closeLeveragePosition(pos.assetId, pos.shares, asset->price());
            closed++;
        }
    }
    updateLeverageTable();
    updateInfo();
    QMessageBox::information(this, "平仓完成", QString("已平仓 %1 条杠杆持仓").arg(closed));
}

void BankDialog::updateInfo()
{
    if (!m_wallet) return;

    ui->savingsBalance->setText(QString("活期余额: %1 功德").arg(m_wallet->savingsBalance(), 0, 'f', 2));

    double debt = m_wallet->totalDebt();
    ui->debtBalance->setText(QString("债务总额: %1 功德").arg(debt, 0, 'f', 2));
    if (debt > m_wallet->maxBorrow()) {
        ui->debtBalance->setStyleSheet("font-size: 14px; color: #C62828; font-weight: bold;");
    } else if (debt > m_wallet->maxBorrow() * 0.5) {
        ui->debtBalance->setStyleSheet("font-size: 14px; color: #E65100; font-weight: bold;");
    } else {
        ui->debtBalance->setStyleSheet("font-size: 14px; color: #2E7D32; font-weight: bold;");
    }

    int score = (int)m_wallet->creditScore();
    ui->creditScore->setText(QString("信用分: %1 / 1000").arg(score));
    if (score < 300) {
        ui->creditScore->setStyleSheet("font-size: 14px; color: #C62828; font-weight: bold;");
    } else if (score < 500) {
        ui->creditScore->setStyleSheet("font-size: 14px; color: #E65100; font-weight: bold;");
    } else {
        ui->creditScore->setStyleSheet("font-size: 14px; color: #2E7D32; font-weight: bold;");
    }

    ui->maxBorrow->setText(QString("可借额度: %1 功德").arg(m_wallet->maxBorrow(), 0, 'f', 2));

    double yz = m_wallet->yezhang();
    ui->yezhangValue->setText(QString("业障值: %1 / 2000").arg(yz, 0, 'f', 0));
    if (yz > 1000) {
        ui->yezhangValue->setStyleSheet("font-size: 14px; color: #C62828; font-weight: bold;");
    } else if (yz > 500) {
        ui->yezhangValue->setStyleSheet("font-size: 14px; color: #E65100; font-weight: bold;");
    } else {
        ui->yezhangValue->setStyleSheet("font-size: 14px; color: #2E7D32; font-weight: bold;");
    }

    double efficiency = m_wallet->efficiency() * 100;
    ui->efficiencyValue->setText(QString("效率系数: %1%").arg(efficiency, 0, 'f', 1));
    if (efficiency < 60) {
        ui->efficiencyValue->setStyleSheet("font-size: 14px; color: #C62828; font-weight: bold;");
    } else if (efficiency < 80) {
        ui->efficiencyValue->setStyleSheet("font-size: 14px; color: #E65100; font-weight: bold;");
    } else {
        ui->efficiencyValue->setStyleSheet("font-size: 14px; color: #2E7D32; font-weight: bold;");
    }

    ui->yezhangBar->setMaximum(2000);
    ui->yezhangBar->setValue((int)yz);
    // 进度条颜色随业障值变化
    if (yz > 1000) {
        ui->yezhangBar->setStyleSheet("QProgressBar::chunk { background-color: #C62828; }");
    } else if (yz > 500) {
        ui->yezhangBar->setStyleSheet("QProgressBar::chunk { background-color: #E65100; }");
    } else {
        ui->yezhangBar->setStyleSheet("QProgressBar::chunk { background-color: #2E7D32; }");
    }
}

void BankDialog::updateFixedDeposits()
{
    if (!m_wallet) return;

    ui->fdList->clear();
    for (const auto& fd : m_wallet->fixedDeposits()) {
        QString status = (QDateTime::currentDateTime() >= fd.dueDate) ? "已到期" : "存期中";
        QString itemText = QString("%1 - %2天期 - %3 (%4)").arg(fd.amount, 0, 'f', 2).arg(fd.dueDate.date().dayOfYear() - QDateTime::currentDateTime().date().dayOfYear()).arg(fd.rate * 100, 0, 'f', 1).arg(status);
        QListWidgetItem* item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, fd.id);
        ui->fdList->addItem(item);
    }
}

void BankDialog::updateLeverageTable()
{
    if (!m_wallet) return;

    double levDebt = m_wallet->leverageDebt();
    ui->leverageDebtLabel->setText(QString("杠杆负债: %1 功德").arg(levDebt, 0, 'f', 2));
    if (levDebt > 0) {
        ui->leverageDebtLabel->setStyleSheet("font-size: 14px; color: #C62828; font-weight: bold;");
    } else {
        ui->leverageDebtLabel->setStyleSheet("font-size: 14px; color: #2E7D32; font-weight: bold;");
    }

    double borrowed = m_wallet->totalLeverageBorrowed();
    ui->totalBorrowedLabel->setText(QString("当前借款: %1 功德").arg(borrowed, 0, 'f', 2));
    if (borrowed > 0) {
        ui->totalBorrowedLabel->setStyleSheet("font-size: 14px; color: #E65100; font-weight: bold;");
    } else {
        ui->totalBorrowedLabel->setStyleSheet("font-size: 14px; color: #2E7D32; font-weight: bold;");
    }

    ui->leverageTable->setRowCount(0);
    int row = 0;
    for (const auto& pos : m_wallet->leveragePositions()) {
        ui->leverageTable->insertRow(row);

        // 查找资产名称和价格
        QString assetName = pos.assetId;
        double price = 0;
        for (Asset* a : m_assets) {
            if (a->id() == pos.assetId) {
                assetName = a->name();
                price = a->price();
                break;
            }
        }

        double marginRate = m_wallet->marginRate(pos.assetId, price);
        QString rateText = QString("%1%").arg(marginRate * 100, 0, 'f', 1);
        if (marginRate < 0.20) {
            rateText += " (危险)";
        } else if (marginRate < 0.35) {
            rateText += " (预警)";
        }

        QTableWidgetItem* nameItem = new QTableWidgetItem(assetName);
        nameItem->setData(Qt::UserRole, pos.assetId);
        ui->leverageTable->setItem(row, 0, nameItem);
        ui->leverageTable->setItem(row, 1, new QTableWidgetItem(QString::number(pos.shares, 'f', 4)));
        ui->leverageTable->setItem(row, 2, new QTableWidgetItem(QString::number(pos.principal, 'f', 2)));
        ui->leverageTable->setItem(row, 3, new QTableWidgetItem(QString::number(pos.borrowed, 'f', 2)));
        ui->leverageTable->setItem(row, 4, new QTableWidgetItem(rateText));
        row++;
    }
}
