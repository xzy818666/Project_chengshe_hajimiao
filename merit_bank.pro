QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    meritbank.cpp \
    wallet.cpp \
    asset.cpp \
    meritindex.cpp \
    karmabond.cpp \
    dharmafund.cpp \
    samsarafutures.cpp \
    instrument.cpp \
    merithall.cpp \
    bankdialog.cpp \
    exchangedialog.cpp \
    shopdialog.cpp \
    marketevent.cpp \
    achievementmanager.cpp \
    achievementdialog.cpp \
    portfolioadvisor.cpp \
    arbitragescanner.cpp \
    autoinvestor.cpp \
    startmenu.cpp \
    savemanager.cpp \
    tutorialdialog.cpp \
    cultivationlevel.cpp \
    instrumentresonance.cpp \
    creditrating.cpp \
    tutorialmanager.cpp \
    tutorialoverlay.cpp

HEADERS += \
    wallet.h \
    asset.h \
    meritindex.h \
    karmabond.h \
    dharmafund.h \
    samsarafutures.h \
    instrument.h \
    merithall.h \
    bankdialog.h \
    exchangedialog.h \
    shopdialog.h \
    marketevent.h \
    achievementmanager.h \
    achievementdialog.h \
    portfolioadvisor.h \
    arbitragescanner.h \
    autoinvestor.h \
    startmenu.h \
    savemanager.h \
    tutorialdialog.h \
    cultivationlevel.h \
    instrumentresonance.h \
    creditrating.h \
    tutorialmanager.h \
    tutorialoverlay.h

FORMS += \
    merithall.ui \
    bankdialog.ui \
    exchangedialog.ui \
    shopdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
