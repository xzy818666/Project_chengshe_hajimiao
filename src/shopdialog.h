#ifndef SHOPDIALOG_H
#define SHOPDIALOG_H

#include <QDialog>
#include <QSet>
#include <QLabel>
#include "wallet.h"
#include "instrument.h"

class QPushButton;

class ShopDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShopDialog(QWidget *parent = nullptr);
    ~ShopDialog();

    void setWallet(Wallet* wallet);

signals:
    void cloudInstrumentChanged(const Instrument& instrument);
    void lotusInstrumentToggled(const Instrument& instrument);

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onBuy();
    void onEquip();

private:
    Wallet* m_wallet;
    Instrument m_currentCloudInstrument;
    Instrument m_selectedInstrument;
    QSet<int> m_ownedTypes;
    QSet<int> m_equippedLotusTypes;
    bool m_centerVisible = false;

    // 界面控件
    QLabel* m_centerGlow;     // radiance 发光背景
    QLabel* m_centerImage;
    QLabel* m_nameLabel;
    QLabel* m_descLabel;
    QLabel* m_priceLabel;
    QPushButton* m_buyBtn;
    QPushButton* m_equipBtn;
    QWidget* m_bottomPanel;

    struct ThumbItem {
        Instrument::Type type;
        QLabel* label;
    };
    QList<ThumbItem> m_leftThumbs;
    QList<ThumbItem> m_rightThumbs;

    void setupUI();
    void updateLayout();
    void updateCenterDisplay();
    void updateBottomPanel();
    QString thumbPath(Instrument::Type type) const;
    QString fullPath(Instrument::Type type) const;
};

#endif // SHOPDIALOG_H
