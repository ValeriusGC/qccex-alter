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

QT  *= core gui testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Where project *.pro is located
PROJECT_ROOT = $$PWD/

TARGET = qccex-alter_tests
TEMPLATE = app
# Can be used in code to check if we in TEST_MODE
DEFINES += DO_TESTS
INCLUDEPATH += ./tests ./src

SOURCES += $$files(tests/*.cpp) \
            src/settings.cpp \
            src/note_defines.cpp

HEADERS += $$files(tests/*.h) \
            src/settings.h  \
            src/note_defines.h

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
release: DESTDIR = $$PROJECT_ROOT/../build/tests
debug:   DESTDIR = $$PROJECT_ROOT/../build/tests

RESOURCES += \
    qccex-alter.qrc \
    translations.qrc

#-----------------------------------------------------------------------------------------------------------------------
#   Generates RESOURCES for *.QM
CONFIG += my_translations

TRANSLATIONS = \
                locale/qccex_ru.ts \
                locale/qccex_uk.ts
