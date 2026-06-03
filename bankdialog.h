#ifndef BANKDIALOG_H
#define BANKDIALOG_H

#include <QDialog>
#include "wallet.h"
#include "asset.h"

QT_BEGIN_NAMESPACE
namespace Ui { class BankDialog; }
QT_END_NAMESPACE

class QLabel;
class QTimer;

class BankDialog : public QDialog
{
    Q_OBJECT

public:
    BankDialog(QWidget *parent = nullptr);
    ~BankDialog();

    void setWallet(Wallet* wallet);
    void setAssets(QList<Asset*> assets);

private slots:
    void onDepositSavings();
    void onWithdrawSavings();
    void onBuyFixedDeposit(int days, double rate);
    void onWithdrawFixedDeposit();
    void onBorrow();
    void onRepay();
    void onConfess();
    void onAddMargin();
    void onCloseAllLeverage();
    void updateInfo();
    void updateFixedDeposits();
    void updateLeverageTable();

private:
    Ui::BankDialog *ui;
    Wallet* m_wallet;
    QList<Asset*> m_assets;

    static const double FIXED_RATE_7DAY;
    static const double FIXED_RATE_30DAY;
    static const double FIXED_RATE_90DAY;

    // 神兽话语系统
    QWidget* m_cloudFrame;
    QLabel* m_beastText;
    QTimer* m_talkTimer;

    void setupBeastTalk();
    void showBeastTalk(const QString& actionType, bool success);
};

#endif // BANKDIALOG_H
