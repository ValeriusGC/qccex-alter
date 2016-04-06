#include <QDir>
#include <QLocale>
#include <QApplication>
#include <QWidgetList>
#include <QWidget>
#include <QTranslator>
#include <QSettings>
#include <QFileInfo>
#include <QDirIterator>

#include "settings.h"
#include "note_defines.h"

using namespace nq;

/**
 * @brief getFileName
 * @return
 *
 * Inner method to get FileName for config-file
 */
QString getFileName() {
#ifndef QT_DEBUG
    const QFileInfo inf(QCoreApplication::applicationFilePath());
    const QString fileName = QString("%1/%2.conf").arg(inf.absolutePath()).arg(inf.baseName());
#else
    const QFileInfo inf(QCoreApplication::applicationFilePath());
    const QString fileName = QString("%1/%2.conf").arg(PROFILE_DIR).arg(inf.baseName());
#endif
    return fileName;
}

//======================================================================================================================

const QString Settings::Keys::General::Locale = "local";
const QString Settings::Keys::General::Geometry = "geometry";
const QString Settings::Keys::General::WindowState = "windowState";

//======================================================================================================================

Settings &Settings::instance()
{
    static Settings singleton;
    return singleton;
}

QHash<QString, QString> Settings::enumLanguages()
{
    QHash<QString, QString> result;
//#if defined(Q_WS_X11) or defined(Q_OS_LINUX)
//    QDir dir(QString(PROGRAM_DATA_DIR) + "locale");
//#else
//    QDir dir(qApp->applicationDirPath() + "/locale");
//#endif

    QDir dir(":/translations");
//    QDirIterator it(":/translations/locale", QDirIterator::Subdirectories);
//    while (it.hasNext()) {
//        qDebug() << it.next();
//    }


    QStringList qms = dir.entryList(QStringList() << "qccex_*.qm", QDir::Files, QDir::Name);

    QRegExp rx("^qccex_([-_a-zA-Z]+)\\.qm$");
    for (int i = 0; i < qms.size(); ++i) {
        if(rx.indexIn(qms[i]) >= 0) {
            QString localeName = rx.cap(1);
            QLocale loc(localeName);
            result.insert(localeName, loc.nativeLanguageName());
        }
    }
    return result;
}

void Settings::setApplicationLocale(const QString &localeName)
{
    QLocale locale(localeName);
    m_localeName = locale.languageToString(locale.language());
    QLocale::setDefault(locale);
    static QTranslator *qtTranslator = NULL;
    static QTranslator *qtbaseTranslator = NULL;
    static QTranslator *myappTranslator = NULL;

    if(!qtbaseTranslator) {
        qApp->removeTranslator(qtbaseTranslator);
        delete qtbaseTranslator;
        qtbaseTranslator = new QTranslator;
    }
    if(!qtTranslator) {
        qApp->removeTranslator(qtTranslator);
        delete qtTranslator;
        qtTranslator = new QTranslator;
    }
    if(!myappTranslator) {
        qApp->removeTranslator(myappTranslator);
        delete myappTranslator;
        myappTranslator = new QTranslator;
    }

//#if defined(Q_WS_X11) or defined(Q_OS_LINUX)
//    qtbaseTranslator->load(locale, "qtbase", "_", QString(PROGRAM_DATA_DIR) + "locale", ".qm");
//#else
//    qtbaseTranslator->load(locale, "qtbase", "_", qApp->applicationDirPath() + "/locale",  ".qm");
//#endif
//    qApp->installTranslator(qtbaseTranslator);
    {
        const QFile f(QString(":/translations/qtbase_%1.qm").arg(locale.name()));
        qtbaseTranslator->load(f.fileName());
        qApp->installTranslator(qtbaseTranslator);
    }

//#if defined(Q_WS_X11) or defined(Q_OS_LINUX)
//    qtTranslator->load(locale, "qt", "_", QString(PROGRAM_DATA_DIR) + "locale", ".qm");
//#else
//    qtTranslator->load(locale, "qt", "_", qApp->applicationDirPath() + "/locale",  ".qm");
//#endif
    {
        const QFile f(QString(":/translations/qt_%1.qm").arg(locale.name()));
        qtTranslator->load(f.fileName());
        qApp->installTranslator(qtTranslator);
    }

//#if defined(Q_WS_X11) or defined(Q_OS_LINUX)
//    myappTranslator->load("note_qt_" + locale.name() + ".qm", QString(PROGRAM_DATA_DIR) + "locale");
//#else
//    myappTranslator->load(locale, "note_qt", "_", qApp->applicationDirPath() + "/locale", ".qm");
//#endif

//    QFile f2(":/translations/locale/note_qt_cs.qm");
//    LOG1_EX(f2.exists());
//    QString f = ":/translations/locale/note_qt_" + locale.name() + ".qm";
    {
        const QFile f(QString(":/translations/qccex_%1.qm").arg(locale.name()));
        myappTranslator->load(f.fileName());
        qApp->installTranslator(myappTranslator);
    }

//    qDebug() << "Detected system locale: " << QLocale::system().name();
//    qDebug() << "Set locale: " << locale.name();
//    qDebug() << qApp->applicationDirPath();

    // For what it need, if i should call retranslateUI???
//    QWidgetList wl = QApplication::allWidgets();
//    for(int i = 0; i < wl.size(); ++i) {
//        wl[i]->setLocale(locale);
//        LOG1(wl[i]->objectName());
//    }

    setValue(Settings::Keys::General::Locale, localeName);
}

QString Settings::localeName() const
{
    return m_localeName;
}

QString Settings::makeAppTitle(const QString &extraString)
{
    QString title;
    const QString base = qApp->translate("Application", "ShortyNoty", "application title");
    if(extraString.isEmpty()) {
        title = base;
    }else{
        title = QString("%1: %2").arg(base).arg(extraString);
    }
    if(title != m_title) {
        m_title = title;
        emit fireTitleChanged(m_title);
    }
    return m_title;
}


Settings::Settings() : QSettings(getFileName(), QSettings::IniFormat)
{
    INC_THIS(true);
}

Settings::~Settings()
{
    DEC_THIS(true);
}

