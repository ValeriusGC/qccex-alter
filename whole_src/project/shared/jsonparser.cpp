#include <QJsonParseError>
#include <QJsonDocument>

#include "jsonparser.h"
#include "shared_def.h"

const QString JsonParser::IDS_HEADER    = "hdr";
const QString JsonParser::IDS_BODY      = "body";
const QString JsonParser::IDS_VER       = "ver";
const QString JsonParser::IDS_CID       = "cid";
const QString JsonParser::IDS_PSW       = "psw";
const QString JsonParser::IDS_QRY_TYPE  = "type";
const QString JsonParser::IDS_QRY_ORD   = "ord";
const QString JsonParser::IDS_DBG_LVL   = "dbg";
const QString JsonParser::IDS_RESULT    = "result";

const qint32 BAD_INT = -1;

//JsonParser::JsonParser() :
//    QObject(nullptr),
//    m_candidate(""),
//    m_result(JPE_Empty, "")
//{

//}

JsonParser::JsonParser(const QString &candidate, QObject *parent) :
    QObject(parent),
    m_candidate(candidate),
    m_ver(BAD_INT),
    m_cid(""),
    m_psw(""),
    m_type(BAD_INT),
    m_ord(BAD_INT),
    m_dbg(BAD_INT),
    m_state(JPE_Empty, QObject::tr("Empty JSON", "SYS")),
    m_root(QJsonObject()),
    m_hdr(QJsonObject()),
    m_body(QJsonObject())
{
    INC_THIS(false);
    m_root[IDS_HEADER] = m_hdr;
    m_root[IDS_BODY] = m_body;
    qint32 cnt1 = m_root.count();
    if(!m_candidate.isEmpty()) {
        parse();
    }
}

//JsonParser::JsonParser(JsonParser &rhs) :
//    QObject(rhs.parent()),
//    m_result(rhs.m_result)
//{
//    m_candidate = rhs.m_candidate;
//    m_ver = rhs.m_ver;
//    m_object = rhs.m_object;
//    m_header = rhs.m_header;
//    m_body = rhs.m_body;
//}

JsonParser::~JsonParser()
{
    DEC_THIS(false);
}

QString JsonParser::toJson() const
{
//    m_hdr[IDS_VER] = m_ver;
//    m_hdr[IDS_CID] = m_cid;
//    m_hdr[IDS_PSW] = m_psw;
//    m_hdr[IDS_QRY_TYPE] = m_type;
//    m_hdr[IDS_QRY_ORD] = m_ord;
//    m_hdr[IDS_DBG_LVL] = m_dbg;
//    m_root[IDS_HEADER] = m_hdr;
//    m_root[IDS_BODY] = m_body;

    QJsonObject root;
    const qint32 cnt = m_body.count();
    root.insert(IDS_HEADER, m_hdr);
    root.insert(IDS_BODY, m_body);
    QJsonDocument jd;
    jd.setObject(root);
    return jd.toJson();
//    QJsonObject hdr;
//    QJsonObject body;
//    QJsonObject root;
//    hdr[IDS_VER] = m_ver;
//    hdr[IDS_CID] = m_cid;
//    hdr[IDS_PSW] = m_psw;
//    hdr[IDS_QRY_TYPE] = m_type;
//    hdr[IDS_QRY_ORD] = m_ord;
//    hdr[IDS_DBG_LVL] = m_dbg;
//    root[IDS_HEADER] = hdr;
//    root[IDS_BODY] = body;
//    QJsonDocument jd;
//    jd.setObject(root);
//    return jd.toJson();
}

JsonParser::Result_t JsonParser::state() const
{
    return m_state;
}

qint32 JsonParser::ver() const
{
    return m_ver;
}

JsonParser &JsonParser::setVer(qint32 ver)
{
    if(ver != m_ver){
        m_ver = ver;
        m_hdr[IDS_VER] = m_ver;
    }
    qint32 cnt1 = m_root.count();
    qint32 cnt2 = m_hdr.count();
    qint32 v1 = m_hdr[IDS_VER].toInt();

    m_root[IDS_HEADER].toObject()[IDS_VER] = v1+2;
    qint32 v2 = m_hdr[IDS_VER].toInt();


    m_root[IDS_HEADER].toObject().count();
    return *this;
}

QString JsonParser::cid() const
{
    return m_cid;
}

JsonParser &JsonParser::setCid(const QString &cid)
{
    if(m_cid != cid){
        m_cid = cid;
        m_hdr[IDS_CID] = m_cid;
    }
    qint32 cnt1 = m_root.count();
    qint32 cnt2 = m_hdr.count();
    return *this;
}

QString JsonParser::psw() const
{
    return m_psw;
}

JsonParser &JsonParser::setPsw(const QString &psw)
{
    if(psw != m_psw){
        m_psw = psw;
        m_hdr[IDS_PSW] = m_psw;
    }
    return *this;
}

qint32 JsonParser::type() const
{
    return m_type;
}

JsonParser &JsonParser::setType(const qint32 &type)
{
    if(type != m_type){
        m_type = type;
        m_hdr[IDS_QRY_TYPE] = m_type;
    }
    return *this;
}

qint32 JsonParser::ord() const
{
    return m_ord;
}

JsonParser &JsonParser::setOrd(const qint32 &ord)
{
    if(ord != m_ord){
        m_ord = ord;
        m_hdr[IDS_QRY_ORD] = m_ord;
    }
    return *this;
}

qint32 JsonParser::dbg() const
{
    return m_dbg;
}

JsonParser &JsonParser::setDbg(const qint32 &dbg)
{
    if(dbg != m_dbg){
        m_dbg = dbg;
        m_hdr[IDS_DBG_LVL] = m_dbg;
    }
    return *this;
}

QString JsonParser::rawCandidate() const
{
    return m_candidate;
}

QString JsonParser::jsonCandidate() const
{
    return m_jsonCandidate;
}

QJsonObject JsonParser::root() const
{
    return m_root;
}

QJsonObject JsonParser::hdr() const
{
    return m_hdr;
}

QJsonObject JsonParser::body()
{
    m_body["test"] = 1;
    return m_body;
}

void JsonParser::parse()
{
    QJsonParseError jpe;
    QJsonDocument jd = QJsonDocument::fromJson(QByteArray(m_candidate.toLatin1()), &jpe);
    m_jsonCandidate = jd.toJson();

    // 1. Just check correct string ( empty JSON, such as '{}' is correct too )
    if(jpe.error == QJsonParseError::NoError) {
        m_state = {JPE_NoError, QObject::tr("Parsing is ok", "SYS")};
    }else{
        m_state = {JPE_SmthWrong, jpe.errorString()};
        return;
    }

    // Turn Document to QJsonObject
    qint32 cnt1 = m_root.count();
    m_root = jd.object();
    cnt1 = m_root.count();
    if(m_root.isEmpty()) {
        m_state = {JPE_Empty, QObject::tr("JSON is empty", "SYS")};
        return;
    }

    // Check for parts

    // HEADER
    if(!m_root[IDS_HEADER].isObject()) {
        m_state = {JPE_HeaderError, QObject::tr("JSON has no header", "SYS")};
        return;
    }
    m_hdr = m_root[IDS_HEADER].toObject();
    if(m_hdr[IDS_VER].isNull() || m_hdr[IDS_VER].toInt(BAD_INT)==BAD_INT) {
        m_state = {JPE_VersionError, QObject::tr("JSON has no valid version", "SYS")};
        return;
    }else{
        m_ver = m_hdr[IDS_VER].toInt();
    }

    if(m_hdr[IDS_CID].isNull() || m_hdr[IDS_CID].toString() == "") {
        m_state = {JPE_CIDError, QObject::tr("JSON has no valid CID", "SYS")};
        return;
    }else{
        m_cid = m_hdr[IDS_CID].toString();
    }

    if(!m_hdr[IDS_PSW].isNull()) {
        m_psw = m_hdr[IDS_PSW].toString();
    }

    if(m_hdr[IDS_QRY_TYPE].isNull() || m_hdr[IDS_QRY_TYPE].toInt(BAD_INT)==BAD_INT) {
        m_state = {JPE_QryTypeError, QObject::tr("JSON has no valid query type", "SYS")};
        return;
    }else{
        m_type = m_hdr[IDS_QRY_TYPE].toInt();
    }

    if(m_hdr[IDS_QRY_ORD].isNull() || m_hdr[IDS_QRY_ORD].toInt(BAD_INT)==BAD_INT) {
        m_state = {JPE_QryOrdError, QObject::tr("JSON has no valid query ordinal", "SYS")};
        return;
    }else{
        m_ord = m_hdr[IDS_QRY_ORD].toInt();
    }

    if(!m_hdr[IDS_DBG_LVL].isNull() && m_hdr[IDS_DBG_LVL].toInt(BAD_INT)!=BAD_INT) {
        m_dbg = m_hdr[IDS_DBG_LVL].toInt();
    }

    // BODY
    if(!m_root[IDS_BODY].isObject()) {
        m_state = {JPE_BodyError, QObject::tr("JSON has no body", "SYS")};
        return;
    }
    m_body = m_root[IDS_BODY].toObject();

    // ~Check for parts

    return;
}

