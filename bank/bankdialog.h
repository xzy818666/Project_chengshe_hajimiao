#ifndef BANKDIALOG_H
#define BANKDIALOG_H

#include <QDialog>
#include "wallet.h"

QT_BEGIN_NAMESPACE
namespace Ui { class BankDialog; }
QT_END_NAMESPACE

class BankDialog : public QDialog
{
    Q_OBJECT

public:
    BankDialog(QWidget *parent = nullptr);
    ~BankDialog();

    void setWallet(Wallet* wallet);

private slots:
    void onDepositSavings();
    void onWithdrawSavings();
    void onBuyFixedDeposit(int days, double rate);
    void onWithdrawFixedDeposit();
    void onBorrow();
    void onRepay();
    void onConfess();
    void updateInfo();
    void updateFixedDeposits();

private:
    Ui::BankDialog *ui;
    Wallet* m_wallet;

    static const double FIXED_RATE_7DAY;
    static const double FIXED_RATE_30DAY;
    static const double FIXED_RATE_90DAY;
};

#endif // BANKDIALOG_H
