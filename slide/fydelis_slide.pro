QT       += core gui widgets
CONFIG   += c++17 c++20
DEFINES  += QT_NO_DEPRECATED_WARNINGS

TARGET = FydelisSlide
TEMPLATE = app

SOURCES += main.cpp fydelis_slide.cpp
HEADERS += fydelis_slide.h
FORMS   += fydelis_slide.ui

RESOURCES += ../resources.qrc

INCLUDEPATH += $$PWD/../..

win32 {
    RC_ICONS = $$PWD/../resources/icon_slide.ico
}
unix {
    RC_ICONS = $$PWD/../resources/icon_slide.ico
}