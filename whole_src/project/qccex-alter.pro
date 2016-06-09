#-------------------------------------------------
#
# Project created by QtCreator 2016-04-03T13:46:02
#
# Goal: Get experience in cross-compiling
#-------------------------------------------------

# Simple informs if correct _QMAKE_CACHE_ is used
message('_QMAKE_CACHE_' for $$_FILE_: $$_QMAKE_CACHE_)

PROJECT_ROOT = $$PWD

# Adds feature 'minqtversion' for checking minimal reqiured Qt version
# Let's 5.4.0 for example )
CONFIG += minqtversion
MGS_MIN_QT_VERSION = 5.4.0

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qccex-alter
TEMPLATE = app

# nullptr and other new C++ features
CONFIG += c++11

# In windows* it is headache. remove this.
CONFIG -= debug_and_release debug_and_release_target

## For unit-testing
#CONFIG(test) {
#    # This works in mode CONFIG+=test
#    $$t_message(Test build)
#    DEFINES += DO_TESTS
#    TARGET = qccex-test
#    QT += testlib
#    INCLUDEPATH += ./tests

#    SOURCES += $$files(tests/*.cpp)

#    HEADERS += $$files(tests/*.h)


#} else {
#    $$t_message(Normal build)
#    DEFINES -= DO_TESTS
#}

INCLUDEPATH += ./src ./shared

DEFINES += PROGRAM_VERSION=\\\"$$VERSION\\\"
DEFINES += PROFILE_DIR=\\\"$$_PRO_FILE_PWD_\\\"
DEFINES += TARGET_BASE_NAME=\\\"$$TARGET_BASE\\\"


OBJECTS_DIR = build
UI_DIR = build
MOC_DIR = build
RCC_DIR = build

# including sub-module with QsT ORM
# include(qst/qst.pri)

SOURCES += \
    $$files(src/*.cpp) \
    shared/shared_def.cpp \
    shared/notemodel.cpp  \
    shared/note.cpp \
    shared/sqlitestorage.cpp \
    shared/base_storage.cpp \
    shared/sqlitestorage_elements.cpp \
    shared/author.cpp \
    shared/authormodel.cpp\
    shared/shared_result.cpp \
    shared/storage_globals.cpp \
    shared/storagefactory.cpp \
    shared/progressinfo.cpp \
    shared/sqlstorageelement.cpp \
    shared/shared_const.cpp


HEADERS  += \
    $$files(src/*.h) \
    shared/shared_def.cpp \
    shared/notemodel.h \
    shared/note.h \
    shared/storage_globals.h \
    shared/sqlitestorage.h \
    shared/base_storage.h \
    shared/sqlitestorage_elements.h \
    shared/author.h \
    shared/authormodel.h \
    shared/shared_result.h \
    shared/storagefactory.h \
    shared/progressinfo.h \
    shared/sqlstorageelement.h \
    shared/shared_const.h

RESOURCES += \
    qccex-alter.qrc \
    translations.qrc

#-----------------------------------------------------------------------------------------------------------------------
#   Generates RESOURCES for *.QM
CONFIG += my_translations

# Adds system strings localization
QT_QM = \
        $$[QT_INSTALL_TRANSLATIONS]/qt*_ru.qm \
        $$[QT_INSTALL_TRANSLATIONS]/qt*_uk.qm

TRANSLATIONS = \
                locale/qccex_ru.ts \
                locale/qccex_uk.ts
