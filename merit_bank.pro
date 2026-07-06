QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/meritbank.cpp \
    src/wallet.cpp \
    src/asset.cpp \
    src/meritindex.cpp \
    src/karmabond.cpp \
    src/dharmafund.cpp \
    src/samsarafutures.cpp \
    src/instrument.cpp \
    src/merithall.cpp \
    src/bankdialog.cpp \
    src/exchangedialog.cpp \
    src/shopdialog.cpp \
    src/marketevent.cpp \
    src/achievementmanager.cpp \
    src/achievementdialog.cpp \
    src/portfolioadvisor.cpp \
    src/arbitragescanner.cpp \
    src/autoinvestor.cpp \
    src/startmenu.cpp \
    src/savemanager.cpp \
    src/tutorialdialog.cpp \
    src/cultivationlevel.cpp \
    src/instrumentresonance.cpp \
    src/creditrating.cpp \
    src/tutorialmanager.cpp \
    src/tutorialoverlay.cpp

HEADERS += \
    src/wallet.h \
    src/asset.h \
    src/meritindex.h \
    src/karmabond.h \
    src/dharmafund.h \
    src/samsarafutures.h \
    src/instrument.h \
    src/merithall.h \
    src/bankdialog.h \
    src/exchangedialog.h \
    src/shopdialog.h \
    src/marketevent.h \
    src/achievementmanager.h \
    src/achievementdialog.h \
    src/portfolioadvisor.h \
    src/arbitragescanner.h \
    src/autoinvestor.h \
    src/startmenu.h \
    src/savemanager.h \
    src/tutorialdialog.h \
    src/cultivationlevel.h \
    src/instrumentresonance.h \
    src/creditrating.h \
    src/tutorialmanager.h \
    src/tutorialoverlay.h

FORMS += \
    src/merithall.ui \
    src/bankdialog.ui \
    src/exchangedialog.ui \
    src/shopdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
