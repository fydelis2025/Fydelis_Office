QT       += core gui widgets
CONFIG   += c++17 c++20
DEFINES  += QT_NO_DEPRECATED_WARNINGS
TARGET = FydelisCalc
TEMPLATE = app

SOURCES += main.cpp fydelis_calc.cpp
HEADERS += fydelis_calc.h

RESOURCES += $$PWD/../resources.qrc

win32 {
    RC_ICONS = $$PWD/../resources/icon_calc.ico
}
unix {
    RC_ICONS = $$PWD/../resources/icon_calc.ico
}