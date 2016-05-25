QT      *= core gui sql
CONFIG *= widgets

TEMPLATE = lib
CONFIG += staticlib

# nullptr and other new C++ features
CONFIG += c++11

include(qst/qst.pri)

