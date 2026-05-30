#ifndef SHOPDIALOG_H
#define SHOPDIALOG_H

#include <QDialog>
#include <QSet>
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

signals:
    void cloudInstrumentChanged(const Instrument& instrument);
    void lotusInstrumentToggled(const Instrument& instrument);

private slots:
    void onBuy();
    void onEquip();
    void onSelectionChanged();
    void updateWallet();

private:
    Ui::ShopDialog *ui;
    Wallet* m_wallet;
    Instrument m_currentCloudInstrument;
    Instrument m_selectedInstrument;
    QSet<int> m_ownedTypes;
    QSet<int> m_equippedLotusTypes;

    void populateShop();
    void refreshStatus();
};

#endif // SHOPDIALOG_H
