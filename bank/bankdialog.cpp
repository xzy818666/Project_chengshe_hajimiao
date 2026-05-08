#include "bankdialog.h"
#include "ui_bankdialog.h"

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

    connect(ui->depositBtn, &QPushButton::clicked, this, &BankDialog::onDepositSavings);
    connect(ui->withdrawBtn, &QPushButton::clicked, this, &BankDialog::onWithdrawSavings);
    connect(ui->buyFD7Btn, &QPushButton::clicked, [this]() { onBuyFixedDeposit(7, FIXED_RATE_7DAY); });
    connect(ui->buyFD30Btn, &QPushButton::clicked, [this]() { onBuyFixedDeposit(30, FIXED_RATE_30DAY); });
    connect(ui->buyFD90Btn, &QPushButton::clicked, [this]() { onBuyFixedDeposit(90, FIXED_RATE_90DAY); });
    connect(ui->withdrawFDBtn, &QPushButton::clicked, this, &BankDialog::onWithdrawFixedDeposit);
    connect(ui->borrowBtn, &QPushButton::clicked, this, &BankDialog::onBorrow);
    connect(ui->repayBtn, &QPushButton::clicked, this, &BankDialog::onRepay);
    connect(ui->confessBtn, &QPushButton::clicked, this, &BankDialog::onConfess);
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
    updateInfo();
    updateFixedDeposits();
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

void BankDialog::updateInfo()
{
    if (!m_wallet) return;

    ui->savingsBalance->setText(QString("%1").arg(m_wallet->savingsBalance(), 0, 'f', 2));
    ui->debtBalance->setText(QString("%1").arg(m_wallet->totalDebt(), 0, 'f', 2));
    ui->creditScore->setText(QString("%1").arg((int)m_wallet->creditScore()));
    ui->maxBorrow->setText(QString("%1").arg(m_wallet->maxBorrow(), 0, 'f', 2));
    ui->yezhangValue->setText(QString("%1").arg(m_wallet->yezhang(), 0, 'f', 0));

    double efficiency = m_wallet->efficiency() * 100;
    ui->efficiencyValue->setText(QString("%1%").arg(efficiency, 0, 'f', 1));

    ui->yezhangBar->setMaximum(2000);
    ui->yezhangBar->setValue((int)m_wallet->yezhang());
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
