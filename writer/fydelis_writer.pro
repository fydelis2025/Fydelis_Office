QT       += core gui widgets
CONFIG   += c++17 c++20
DEFINES  += QT_NO_DEPRECATED_WARNINGS
TARGET = FydelisWriter
TEMPLATE = app

SOURCES += main.cpp fydelis_writer.cpp
HEADERS += fydelis_writer.h

# ✅ Recursos — caminho correto
RESOURCES += $$PWD/../resources.qrc

INCLUDEPATH += $$PWD/../..

# ✅ Ícone — caminho absoluto, sem erro
win32 {
    RC_ICONS = $$PWD/../resources/icon_writer.ico
}