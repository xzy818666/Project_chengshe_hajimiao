#ifndef SHOPDIALOG_H
#define SHOPDIALOG_H

#include <QDialog>
#include "wallet.h"
#include "instrument.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ShopDialog; }
QT_END_NAMESPACE

class ShopDialog : public QDialog
{
    Q_OBJECT

public:
    ShopDialog(QWidget *parent = nullptr);
    ~ShopDialog();

    void setWallet(Wallet* wallet);
    Instrument currentInstrument() const;

signals:
    void instrumentChanged(const Instrument& instrument);

private slots:
    void onBuy();
    void onEquip();
    void updateWallet();

private:
    Ui::ShopDialog *ui;
    Wallet* m_wallet;
    Instrument m_currentInstrument;
    Instrument m_selectedInstrument;

    void populateShop();
};

#endif // SHOPDIALOG_H
