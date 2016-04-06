#ifndef NOTE_DEFINES
#define NOTE_DEFINES

//======================================================================================================================
// Debug stuff

#ifdef QT_DEBUG

#include <QTime>
#include <QDebug>
#include <QHash>
#include <QObject>

// Helper for LOG-defines
#define _T QTime::currentTime().toString(QString("hh:mm:ss.zzz"))

// Simple log-marker for passing some point in some function
#define LOG { qDebug() << Q_FUNC_INFO; }

// Log for passing some point in some function with TIME
#define LOG_T { qDebug() << _T << Q_FUNC_INFO; }

// Log with parameter and TIME
#define LOG_TP(p) { qDebug() << _T << Q_FUNC_INFO << "-->" << #p << p; }

// Log with parameter, TIME, FILE and LINE
#define LOG_TPFL(p) { qDebug() << _T << Q_FUNC_INFO << "-->" << #p << p << __FILE__ << __LINE__; }

//----------------------------------------------------------------------------------------------------------------------
// Memory leak control.
// Use it in ctr/dtr to check if all objects free memory

// Debug common map to control memory leak
QHash<QString, QString> &g_objMap();

// Place in CTR. If show=true there will debug string in terminal
#define INC_THIS(show) \
    {   \
        const quint64 key = reinterpret_cast<quint64>(this); \
        const QString keyS = QString::number(key, 16).sprintf("0x%08x", (uint)key); \
        const QString name = this->metaObject()->className(); \
        if(g_objMap().contains(keyS)) {    \
            Q_ASSERT_X(false, "DEBUG_OBJ_HASH", (QString("Object %1 (%2) already exists").arg(keyS).arg(name).toLatin1())); \
        }   \
        g_objMap()[keyS] = name;  \
        if(show) {   \
            const QString s = QString("DEBUG_OBJ_HASH: add %1 (%2). Total = %3").arg(keyS).arg(name).arg(g_objMap().size()); \
            qDebug() << s;  \
        }   \
    }

// Place in DTR when in CTR INC_PTR were placed. If show=true there will debug string in terminal
#define DEC_THIS(show) \
    {               \
        const quint64 key = reinterpret_cast<quint64>(this); \
        const QString keyS = QString::number(key, 16).sprintf("0x%08x", (uint)key); \
        const QString name = this->metaObject()->className(); \
        if(g_objMap().contains(keyS)) {    \
            g_objMap().remove(keyS);       \
            if(show) {   \
                const QString s = QString("DEBUG_OBJ_HASH: del %1 (%2) .Total = %3").arg(keyS).arg(name).arg(g_objMap().size()); \
                qDebug() << s; \
            } \
        }  \
    }

// Shows how many objects is in memory yet
#define CHECK_PTR qDebug()  << "--------------------\nDEBUG_OBJ_HASH:" << g_objMap().size() << g_objMap() \
                            << "\n--------------------";

// Substitute for 'show'
#define SHOW false   // or true if not_show / show

// ~Memory leak control
//----------------------------------------------------------------------------------------------------------------------


#else

// Stub - helper for LOG-defines
#define _T
// Stub - simple log-marker for passing some point in some function
#define LOG
// Stub - log for passing some point in some function with TIME
#define LOG_T
// Stub - Log with parameter and TIME
#define LOG_TP
// Stub - Log with parameter, TIME, FILE and LINE
#define LOG_TPFL

#define INC_THIS(show)
#define DEC_THIS(show)
#define CHECK_PTR

#endif

// ~Debug stuff
//======================================================================================================================

#endif // NOTE_DEFINES

