#include <QUrl>

#include "request.h"
#include "shared_const.h"

using namespace storage;

static const QString IDS_HEADER;
static const QString IDS_BODY;
static const QString IDS_VER;
static const QString IDS_CID;
static const QString IDS_PSW;
static const QString IDS_TYPE;
static const QString IDS_ORD;
static const QString IDS_DBG_LVL;

const QString Request::IDS_HEADER   = "hdr";
const QString Request::IDS_BODY     = "body";
const QString Request::IDS_VER      = "ver";
const QString Request::IDS_CID      = "cid";
const QString Request::IDS_PSW      = "psw";
const QString Request::IDS_TYPE     = "type";
const QString Request::IDS_ORD      = "ord";
const QString Request::IDS_DBG_LVL  = "dbg";

Request::Request(const QString &cid, const QString &psw, qint32 type, qint32 ordinal, qint32 dbgLevel) :
    m_state(NoError, ""),
    m_hdr(QJsonObject()),
    m_body(QJsonObject())
{
    INC_THIS(false);
    if(cid.isEmpty()) {
        m_state = {CIDError, QObject::tr("No valid CID", "SYS")};
        return;
    }

    if(psw.isEmpty()) {
        m_state = {PswError, QObject::tr("No valid password", "SYS")};
        return;
    }

    m_hdr[IDS_VER] = model::Constants::VERSION;
    m_hdr[IDS_CID] = cid;
    m_hdr[IDS_PSW] = psw;
    m_hdr[IDS_TYPE] = type;
    m_hdr[IDS_ORD] = ordinal;
    m_hdr[IDS_DBG_LVL] = dbgLevel;
}

Request::~Request()
{
    DEC_THIS(false);
}

Request::Result_t Request::state() const
{
    return m_state;
}

QString Request::toHttpJson() const
{
    m_hdr[IDS_CID] = QString(QUrl::toPercentEncoding(m_hdr[IDS_CID].toString()));
    m_hdr[IDS_PSW] = QString(QUrl::toPercentEncoding(m_hdr[IDS_PSW].toString()));
    QJsonObject root;
    root.insert(IDS_HEADER, m_hdr);
    root.insert(IDS_BODY, m_body);
    QJsonDocument jd;
    jd.setObject(root);
    return jd.toJson();
}

QString Request::toJson() const
{
    QJsonObject root;
    root.insert(IDS_HEADER, m_hdr);
    root.insert(IDS_BODY, m_body);
    QJsonDocument jd;
    jd.setObject(root);
    return jd.toJson();
}

Request &Request::insert(const QString &key, const QJsonValue &value)
{
    m_body.insert(key, value);
    return *this;
}



