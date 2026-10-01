# ==========================================
# FYDELISOFFICE — COMPILAÇÃO COMPLETA
# FydelisTech OS v2.2 · Salvador • Bahia 🇧🇷
# ==========================================
QT       += core gui widgets
CONFIG   += c++17 c++20 strict_c++
DEFINES  += QT_NO_DEPRECATED_WARNINGS

TEMPLATE = subdirs
SUBDIRS  = fydelis_writer \
           fydelis_calc \
           fydelis_slide

# Caminho comum para todos
INCLUDEPATH += $$PWD/../../include
DEPENDPATH  += $$PWD/../../include

# Ordem de compilação (garante que faz um após o outro)
fydelis_writer.subdir = fydelis_writer
fydelis_calc.subdir   = fydelis_calc
fydelis_slide.subdir  = fydelis_slide