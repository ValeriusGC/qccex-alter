#-------------------------------------------------
#
# Project created by QtCreator 2016-04-03T13:46:02
#
# Goal: Get experience in cross-compiling
#-------------------------------------------------

# Simply informs if correct _QMAKE_CACHE_ is used
message('_QMAKE_CACHE_' for $$_FILE_: $$_QMAKE_CACHE_)

# Adds feature 'minqtversion' for checking minimal reqiured Qt version
# Let's 5.4.0 for example )
CONFIG += minqtversion
MGS_MIN_QT_VERSION = 5.4.0

QT  *= core gui sql testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Where project *.pro is located
PROJECT_ROOT = $$PWD/..

TARGET = qccex-alter_tests
TEMPLATE = app
# Can be used in code to check if we in TEST_MODE
DEFINES += DO_TESTS
INCLUDEPATH *= $$PROJECT_ROOT/src
INCLUDEPATH *= $$PROJECT_ROOT/shared
INCLUDEPATH *= $$PROJECT_ROOT/sub/orm

# It can be found not in relative PRO-dir, but relative to build dir
LIBS += -L../sub/orm -lorm

SOURCES += $$files(*.cpp) \
            ../src/settings.cpp \
            ../shared/shared_result.cpp \
            ../shared/shared_def.cpp

HEADERS += $$files(*.h) \
            ../src/settings.h  \
            ../shared/shared_result.h \
            ../shared/shared_def.h

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
    $$PROJECT_ROOT/qccex-alter.qrc \
    $$PROJECT_ROOT/translations.qrc

#-----------------------------------------------------------------------------------------------------------------------
#   Generates RESOURCES for *.QM
CONFIG += my_translations

TRANSLATIONS = \
                $$PROJECT_ROOT/locale/qccex_ru.ts \
                $$PROJECT_ROOT/locale/qccex_uk.ts
