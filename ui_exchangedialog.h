/********************************************************************************
** Form generated from reading UI file 'exchangedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXCHANGEDIALOG_H
#define UI_EXCHANGEDIALOG_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExchangeDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *eventTicker;
    QTabWidget *tabWidget;
    QWidget *tradingTab;
    QHBoxLayout *tradingLayout;
    QListWidget *assetList;
    QWidget *detailWidget;
    QVBoxLayout *detailLayout;
    QLabel *currentPrice;
    QLabel *change24h;
    QChartView *chartView;
    QLabel *shares;
    QLabel *value;
    QDoubleSpinBox *tradeAmount;
    QHBoxLayout *tradeButtons;
    QPushButton *buyBtn;
    QPushButton *sellBtn;
    QWidget *portfolioTab;
    QVBoxLayout *portfolioLayout;
    QLabel *totalPortfolio;
    QLabel *portfolioRisk;
    QChartView *pieView;
    QPushButton *portfolioBtn;

    void setupUi(QDialog *ExchangeDialog)
    {
        if (ExchangeDialog->objectName().isEmpty())
            ExchangeDialog->setObjectName("ExchangeDialog");
        ExchangeDialog->resize(800, 600);
        verticalLayout = new QVBoxLayout(ExchangeDialog);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        eventTicker = new QLabel(ExchangeDialog);
        eventTicker->setObjectName("eventTicker");

        verticalLayout->addWidget(eventTicker);

        tabWidget = new QTabWidget(ExchangeDialog);
        tabWidget->setObjectName("tabWidget");
        tradingTab = new QWidget();
        tradingTab->setObjectName("tradingTab");
        tradingLayout = new QHBoxLayout(tradingTab);
        tradingLayout->setSpacing(6);
        tradingLayout->setContentsMargins(11, 11, 11, 11);
        tradingLayout->setObjectName("tradingLayout");
        assetList = new QListWidget(tradingTab);
        assetList->setObjectName("assetList");

        tradingLayout->addWidget(assetList);

        detailWidget = new QWidget(tradingTab);
        detailWidget->setObjectName("detailWidget");
        detailLayout = new QVBoxLayout(detailWidget);
        detailLayout->setSpacing(6);
        detailLayout->setContentsMargins(11, 11, 11, 11);
        detailLayout->setObjectName("detailLayout");
        detailLayout->setContentsMargins(0, 0, 0, 0);
        currentPrice = new QLabel(detailWidget);
        currentPrice->setObjectName("currentPrice");

        detailLayout->addWidget(currentPrice);

        change24h = new QLabel(detailWidget);
        change24h->setObjectName("change24h");

        detailLayout->addWidget(change24h);

        chartView = new QChartView(detailWidget);
        chartView->setObjectName("chartView");

        detailLayout->addWidget(chartView);

        shares = new QLabel(detailWidget);
        shares->setObjectName("shares");

        detailLayout->addWidget(shares);

        value = new QLabel(detailWidget);
        value->setObjectName("value");

        detailLayout->addWidget(value);

        tradeAmount = new QDoubleSpinBox(detailWidget);
        tradeAmount->setObjectName("tradeAmount");
        tradeAmount->setMinimum(0.010000000000000);
        tradeAmount->setMaximum(100000.000000000000000);

        detailLayout->addWidget(tradeAmount);

        tradeButtons = new QHBoxLayout();
        tradeButtons->setSpacing(6);
        tradeButtons->setObjectName("tradeButtons");
        buyBtn = new QPushButton(detailWidget);
        buyBtn->setObjectName("buyBtn");

        tradeButtons->addWidget(buyBtn);

        sellBtn = new QPushButton(detailWidget);
        sellBtn->setObjectName("sellBtn");

        tradeButtons->addWidget(sellBtn);


        detailLayout->addLayout(tradeButtons);


        tradingLayout->addWidget(detailWidget);

        tabWidget->addTab(tradingTab, QString());
        portfolioTab = new QWidget();
        portfolioTab->setObjectName("portfolioTab");
        portfolioLayout = new QVBoxLayout(portfolioTab);
        portfolioLayout->setSpacing(6);
        portfolioLayout->setContentsMargins(11, 11, 11, 11);
        portfolioLayout->setObjectName("portfolioLayout");
        totalPortfolio = new QLabel(portfolioTab);
        totalPortfolio->setObjectName("totalPortfolio");

        portfolioLayout->addWidget(totalPortfolio);

        portfolioRisk = new QLabel(portfolioTab);
        portfolioRisk->setObjectName("portfolioRisk");

        portfolioLayout->addWidget(portfolioRisk);

        pieView = new QChartView(portfolioTab);
        pieView->setObjectName("pieView");

        portfolioLayout->addWidget(pieView);

        tabWidget->addTab(portfolioTab, QString());

        verticalLayout->addWidget(tabWidget);

        portfolioBtn = new QPushButton(ExchangeDialog);
        portfolioBtn->setObjectName("portfolioBtn");

        verticalLayout->addWidget(portfolioBtn);


        retranslateUi(ExchangeDialog);

        QMetaObject::connectSlotsByName(ExchangeDialog);
    } // setupUi

    void retranslateUi(QDialog *ExchangeDialog)
    {
        ExchangeDialog->setWindowTitle(QCoreApplication::translate("ExchangeDialog", "\345\271\273\347\274\230\346\211\200", nullptr));
        eventTicker->setText(QCoreApplication::translate("ExchangeDialog", "\345\270\202\345\234\272\345\271\263\347\250\263\350\277\220\350\241\214\344\270\255...", nullptr));
        currentPrice->setText(QCoreApplication::translate("ExchangeDialog", "\344\273\267\346\240\274: 0", nullptr));
        change24h->setText(QCoreApplication::translate("ExchangeDialog", "24h: 0%", nullptr));
        shares->setText(QCoreApplication::translate("ExchangeDialog", "\346\214\201\344\273\223\344\273\275\351\242\235: 0", nullptr));
        value->setText(QCoreApplication::translate("ExchangeDialog", "\345\270\202\345\200\274: 0", nullptr));
        buyBtn->setText(QCoreApplication::translate("ExchangeDialog", "\344\271\260\345\205\245", nullptr));
        sellBtn->setText(QCoreApplication::translate("ExchangeDialog", "\345\215\226\345\207\272", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tradingTab), QCoreApplication::translate("ExchangeDialog", "\344\272\244\346\230\223", nullptr));
        totalPortfolio->setText(QCoreApplication::translate("ExchangeDialog", "\346\200\273\345\270\202\345\200\274: 0", nullptr));
        portfolioRisk->setText(QCoreApplication::translate("ExchangeDialog", "\351\243\216\351\231\251\350\257\204\347\272\247: \344\277\235\345\256\210", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(portfolioTab), QCoreApplication::translate("ExchangeDialog", "\350\265\204\344\272\247\351\205\215\347\275\256", nullptr));
        portfolioBtn->setText(QCoreApplication::translate("ExchangeDialog", "\346\237\245\347\234\213\350\265\204\344\272\247\351\205\215\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExchangeDialog: public Ui_ExchangeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXCHANGEDIALOG_H
