#-------------------------------------------------
#
# Project created by QtCreator 2016-04-03T13:46:02
#
# Goal: Get experience in cross-compiling
#-------------------------------------------------

# Simple informs if correct _QMAKE_CACHE_ is used
message('_QMAKE_CACHE_' for $$_FILE_: $$_QMAKE_CACHE_)

# Adds feature 'minqtversion' for checking minimal reqiured Qt version
# Let's 5.4.0 for example )
CONFIG += minqtversion
MGS_MIN_QT_VERSION = 5.4.0

QT       += core gui

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

INCLUDEPATH += ./src

DEFINES += PROGRAM_VERSION=\\\"$$VERSION\\\"
DEFINES += PROFILE_DIR=\\\"$$_PRO_FILE_PWD_\\\"
DEFINES += TARGET_BASE_NAME=\\\"$$TARGET_BASE\\\"


OBJECTS_DIR = build
UI_DIR = build
MOC_DIR = build
RCC_DIR = build

SOURCES += $$files(src/*.cpp)

HEADERS  += $$files(src/*.h)

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
