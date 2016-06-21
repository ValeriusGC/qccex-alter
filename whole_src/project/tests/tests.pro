#-------------------------------------------------
#
# Project created by QtCreator 2016-04-03T13:46:02
#
# Goal: Get experience in cross-compiling
#-------------------------------------------------

# Simply informs if correct _QMAKE_CACHE_ is used
$$t_message($$_FILE_[_QMAKE_CACHE_]: $$_QMAKE_CACHE_)

# Adds feature 'minqtversion' for checking minimal reqiured Qt version
# Let's 5.4.0 for example )
CONFIG += minqtversion
MGS_MIN_QT_VERSION = 5.4.0

QT  *= core
QT  *= gui
QT  *= sql
QT  *= testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Where project *.pro is located
PROJECT_ROOT = $$PWD/..
$$t_message($$_FILE_[!!!START!!!])
$$t_message($$_FILE_[PROJECT_ROOT]: $$PROJECT_ROOT)

TARGET = qccex-alter_tests
TEMPLATE = app
# Can be used in code to check if we in TEST_MODE
DEFINES += DO_TESTS
INCLUDEPATH *= $$PROJECT_ROOT/src
INCLUDEPATH *= $$PROJECT_ROOT/shared
#INCLUDEPATH *= $$PROJECT_ROOT/sub/orm

# It can be found not in relative PRO-dir, but relative to build dir
#LIBS += -L../sub/orm -lorm

SOURCES *= $$files(*.cpp) \
            ../src/settings.cpp \
            ../shared/dbmanager.cpp \
            ../shared/shared_result.cpp \
            ../shared/shared_def.cpp \
    ../shared/base_storage.cpp \
    ../shared/storage_globals.cpp \
    ../shared/sqlitestorage.cpp \
    test_sqlitestorage.cpp \
    ../shared/base_table.cpp \
    ../shared/sqlite_storage_v1.cpp \
    ../shared/sqlite_storage_v2.cpp \
    ../shared/sqlitestorage_elements.cpp \
    ../shared/note.cpp \
    ../shared/author.cpp \
    test_records.cpp \
    ../shared/taskgetnotes.cpp \
    ../shared/notemodel.cpp \
    ../shared/authormodel.cpp \
    ../shared/progressinfo.cpp \
    ../shared/sqlstorageelement.cpp \
    ../shared/shared_const.cpp \
    ../shared/shared_utils.cpp \
    ../shared/sqlengine.cpp

HEADERS *= $$files(*.h) \
            ../src/settings.h  \
            ../shared/dbmanager.h \
            ../shared/shared_result.h \
            ../shared/shared_def.h \
            ../shared/sb_def.h \
    ../shared/istorageversioner.h \
    ../shared/itable.h \
    ../shared/base_storage.h \
    ../shared/storage_globals.h \
    ../shared/sqlitestorage.h \
    test_sqlitestorage.h \
    ../shared/base_table.h \
    ../shared/sqlite_storage_v1.h \
    ../shared/sqlite_storage_v2.h \
    ../shared/sqlitestorage_elements.h \
    ../shared/note.h \
    ../shared/author.h \
    test_records.h \
    ../shared/taskgetnotes.h \
    ../shared/notemodel.h \
    ../shared/authormodel.h \
    ../shared/progressinfo.h \
    ../shared/sqlstorageelement.h \
    ../shared/shared_const.h \
    ../shared/shared_utils.h \
    ../shared/sqlengine.h

CONFIG += debug
# nullptr and other new C++ features
CONFIG += c++11

# In windows* it is headache. remove this.
CONFIG -= debug_and_release debug_and_release_target

DEFINES += PROGRAM_VERSION=\\\"$$VERSION\\\"
DEFINES += PROFILE_DIR=\\\"$$_PRO_FILE_PWD_\\\"
DEFINES += TARGET_BASE_NAME=\\\"$$TARGET_BASE\\\"

OBJECTS_DIR = build
UI_DIR = build
MOC_DIR = build
RCC_DIR = build

# Where one should place binary
release: DESTDIR = $$PROJECT_ROOT/../../build/tests
debug:   DESTDIR = $$PROJECT_ROOT/../../build/tests

RESOURCES += \
#    qccex-alter.qrc \
    $$PROJECT_ROOT/translations.qrc

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
