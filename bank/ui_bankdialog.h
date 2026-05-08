/********************************************************************************
** Form generated from reading UI file 'bankdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BANKDIALOG_H
#define UI_BANKDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BankDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *savingsTab;
    QVBoxLayout *savingsLayout;
    QLabel *savingsBalance;
    QHBoxLayout *savingsControls;
    QDoubleSpinBox *savingsAmount;
    QPushButton *depositBtn;
    QPushButton *withdrawBtn;
    QLabel *fdLabel;
    QHBoxLayout *fdControls;
    QDoubleSpinBox *fdAmount;
    QPushButton *buyFD7Btn;
    QPushButton *buyFD30Btn;
    QPushButton *buyFD90Btn;
    QListWidget *fdList;
    QPushButton *withdrawFDBtn;
    QWidget *loanTab;
    QVBoxLayout *loanLayout;
    QLabel *debtBalance;
    QLabel *creditScore;
    QLabel *maxBorrow;
    QDoubleSpinBox *borrowAmount;
    QHBoxLayout *loanButtons;
    QPushButton *borrowBtn;
    QDoubleSpinBox *repayAmount;
    QPushButton *repayBtn;
    QWidget *yezhangTab;
    QVBoxLayout *yezhangLayout;
    QLabel *yezhangValue;
    QProgressBar *yezhangBar;
    QLabel *efficiencyValue;
    QDoubleSpinBox *confessAmount;
    QPushButton *confessBtn;

    void setupUi(QDialog *BankDialog)
    {
        if (BankDialog->objectName().isEmpty())
            BankDialog->setObjectName("BankDialog");
        BankDialog->resize(600, 400);
        verticalLayout = new QVBoxLayout(BankDialog);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(BankDialog);
        tabWidget->setObjectName("tabWidget");
        savingsTab = new QWidget();
        savingsTab->setObjectName("savingsTab");
        savingsLayout = new QVBoxLayout(savingsTab);
        savingsLayout->setSpacing(6);
        savingsLayout->setContentsMargins(11, 11, 11, 11);
        savingsLayout->setObjectName("savingsLayout");
        savingsBalance = new QLabel(savingsTab);
        savingsBalance->setObjectName("savingsBalance");

        savingsLayout->addWidget(savingsBalance);

        savingsControls = new QHBoxLayout();
        savingsControls->setSpacing(6);
        savingsControls->setObjectName("savingsControls");
        savingsAmount = new QDoubleSpinBox(savingsTab);
        savingsAmount->setObjectName("savingsAmount");
        savingsAmount->setMinimum(0.010000000000000);
        savingsAmount->setMaximum(100000.000000000000000);

        savingsControls->addWidget(savingsAmount);

        depositBtn = new QPushButton(savingsTab);
        depositBtn->setObjectName("depositBtn");

        savingsControls->addWidget(depositBtn);

        withdrawBtn = new QPushButton(savingsTab);
        withdrawBtn->setObjectName("withdrawBtn");

        savingsControls->addWidget(withdrawBtn);


        savingsLayout->addLayout(savingsControls);

        fdLabel = new QLabel(savingsTab);
        fdLabel->setObjectName("fdLabel");

        savingsLayout->addWidget(fdLabel);

        fdControls = new QHBoxLayout();
        fdControls->setSpacing(6);
        fdControls->setObjectName("fdControls");
        fdAmount = new QDoubleSpinBox(savingsTab);
        fdAmount->setObjectName("fdAmount");
        fdAmount->setMinimum(100.000000000000000);
        fdAmount->setMaximum(100000.000000000000000);

        fdControls->addWidget(fdAmount);

        buyFD7Btn = new QPushButton(savingsTab);
        buyFD7Btn->setObjectName("buyFD7Btn");

        fdControls->addWidget(buyFD7Btn);

        buyFD30Btn = new QPushButton(savingsTab);
        buyFD30Btn->setObjectName("buyFD30Btn");

        fdControls->addWidget(buyFD30Btn);

        buyFD90Btn = new QPushButton(savingsTab);
        buyFD90Btn->setObjectName("buyFD90Btn");

        fdControls->addWidget(buyFD90Btn);


        savingsLayout->addLayout(fdControls);

        fdList = new QListWidget(savingsTab);
        fdList->setObjectName("fdList");

        savingsLayout->addWidget(fdList);

        withdrawFDBtn = new QPushButton(savingsTab);
        withdrawFDBtn->setObjectName("withdrawFDBtn");

        savingsLayout->addWidget(withdrawFDBtn);

        tabWidget->addTab(savingsTab, QString());
        loanTab = new QWidget();
        loanTab->setObjectName("loanTab");
        loanLayout = new QVBoxLayout(loanTab);
        loanLayout->setSpacing(6);
        loanLayout->setContentsMargins(11, 11, 11, 11);
        loanLayout->setObjectName("loanLayout");
        debtBalance = new QLabel(loanTab);
        debtBalance->setObjectName("debtBalance");

        loanLayout->addWidget(debtBalance);

        creditScore = new QLabel(loanTab);
        creditScore->setObjectName("creditScore");

        loanLayout->addWidget(creditScore);

        maxBorrow = new QLabel(loanTab);
        maxBorrow->setObjectName("maxBorrow");

        loanLayout->addWidget(maxBorrow);

        borrowAmount = new QDoubleSpinBox(loanTab);
        borrowAmount->setObjectName("borrowAmount");
        borrowAmount->setMinimum(10.000000000000000);
        borrowAmount->setMaximum(2000.000000000000000);

        loanLayout->addWidget(borrowAmount);

        loanButtons = new QHBoxLayout();
        loanButtons->setSpacing(6);
        loanButtons->setObjectName("loanButtons");
        borrowBtn = new QPushButton(loanTab);
        borrowBtn->setObjectName("borrowBtn");

        loanButtons->addWidget(borrowBtn);

        repayAmount = new QDoubleSpinBox(loanTab);
        repayAmount->setObjectName("repayAmount");
        repayAmount->setMinimum(10.000000000000000);
        repayAmount->setMaximum(10000.000000000000000);

        loanButtons->addWidget(repayAmount);

        repayBtn = new QPushButton(loanTab);
        repayBtn->setObjectName("repayBtn");

        loanButtons->addWidget(repayBtn);


        loanLayout->addLayout(loanButtons);

        tabWidget->addTab(loanTab, QString());
        yezhangTab = new QWidget();
        yezhangTab->setObjectName("yezhangTab");
        yezhangLayout = new QVBoxLayout(yezhangTab);
        yezhangLayout->setSpacing(6);
        yezhangLayout->setContentsMargins(11, 11, 11, 11);
        yezhangLayout->setObjectName("yezhangLayout");
        yezhangValue = new QLabel(yezhangTab);
        yezhangValue->setObjectName("yezhangValue");

        yezhangLayout->addWidget(yezhangValue);

        yezhangBar = new QProgressBar(yezhangTab);
        yezhangBar->setObjectName("yezhangBar");

        yezhangLayout->addWidget(yezhangBar);

        efficiencyValue = new QLabel(yezhangTab);
        efficiencyValue->setObjectName("efficiencyValue");

        yezhangLayout->addWidget(efficiencyValue);

        confessAmount = new QDoubleSpinBox(yezhangTab);
        confessAmount->setObjectName("confessAmount");
        confessAmount->setMinimum(1.000000000000000);
        confessAmount->setMaximum(100.000000000000000);

        yezhangLayout->addWidget(confessAmount);

        confessBtn = new QPushButton(yezhangTab);
        confessBtn->setObjectName("confessBtn");

        yezhangLayout->addWidget(confessBtn);

        tabWidget->addTab(yezhangTab, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(BankDialog);

        QMetaObject::connectSlotsByName(BankDialog);
    } // setupUi

    void retranslateUi(QDialog *BankDialog)
    {
        BankDialog->setWindowTitle(QCoreApplication::translate("BankDialog", "\345\226\204\350\264\242\345\217\270", nullptr));
        savingsBalance->setText(QCoreApplication::translate("BankDialog", "\346\264\273\346\234\237\344\275\231\351\242\235: 0", nullptr));
        depositBtn->setText(QCoreApplication::translate("BankDialog", "\345\255\230\345\205\245", nullptr));
        withdrawBtn->setText(QCoreApplication::translate("BankDialog", "\345\217\226\345\207\272", nullptr));
        fdLabel->setText(QCoreApplication::translate("BankDialog", "\345\256\232\346\234\237\345\255\230\346\254\276", nullptr));
        buyFD7Btn->setText(QCoreApplication::translate("BankDialog", "7\345\244\251 (6%)", nullptr));
        buyFD30Btn->setText(QCoreApplication::translate("BankDialog", "30\345\244\251 (8%)", nullptr));
        buyFD90Btn->setText(QCoreApplication::translate("BankDialog", "90\345\244\251 (12%)", nullptr));
        withdrawFDBtn->setText(QCoreApplication::translate("BankDialog", "\346\217\220\345\217\226\351\200\211\344\270\255", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(savingsTab), QCoreApplication::translate("BankDialog", "\346\264\273\346\234\237\344\270\216\345\256\232\346\234\237", nullptr));
        debtBalance->setText(QCoreApplication::translate("BankDialog", "\345\200\272\345\212\241\346\200\273\351\242\235: 0", nullptr));
        creditScore->setText(QCoreApplication::translate("BankDialog", "\344\277\241\347\224\250\345\210\206: 500", nullptr));
        maxBorrow->setText(QCoreApplication::translate("BankDialog", "\345\217\257\345\200\237\351\242\235\345\272\246: 0", nullptr));
        borrowBtn->setText(QCoreApplication::translate("BankDialog", "\345\200\237\346\254\276", nullptr));
        repayBtn->setText(QCoreApplication::translate("BankDialog", "\350\277\230\346\254\276", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(loanTab), QCoreApplication::translate("BankDialog", "\345\212\237\345\276\267\345\200\237\350\264\267", nullptr));
        yezhangValue->setText(QCoreApplication::translate("BankDialog", "\344\270\232\351\232\234\345\200\274: 0", nullptr));
        efficiencyValue->setText(QCoreApplication::translate("BankDialog", "\346\225\210\347\216\207: 100%", nullptr));
        confessBtn->setText(QCoreApplication::translate("BankDialog", "\345\277\217\346\202\224 (\346\266\210\350\200\227\345\212\237\345\276\267)", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(yezhangTab), QCoreApplication::translate("BankDialog", "\344\270\232\351\232\234\347\212\266\346\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BankDialog: public Ui_BankDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BANKDIALOG_H
