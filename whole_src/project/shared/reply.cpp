#include <QJsonParseError>

#include "reply.h"
#include "request.h"

using namespace storage;

const qint32 BAD_INT = -1;

const QString Reply::IDS_HEADER     = "hdr";
const QString Reply::IDS_BODY       = "body";
const QString Reply::IDS_VER        = "ver";
const QString Reply::IDS_CID        = "cid";
const QString Reply::IDS_TYPE       = "type";
const QString Reply::IDS_ORD        = "ord";
const QString Reply::IDS_RESULT     = "result";

Reply::Reply(const QString &candidate) :
    m_state(BadHeader, QObject::tr("Unknown error", "SYS")),
    m_candidate(candidate),
    m_hdr(QJsonObject()),
    m_body(QJsonObject()),
    m_ver(BAD_INT),
    m_cid(""),
    m_type(BAD_INT),
    m_ord(BAD_INT),
    m_result(BAD_INT)
{
    INC_THIS(false);
    parse();
}

Reply::~Reply()
{
    DEC_THIS(false);
}

QString Reply::candidate() const
{
    return m_candidate;
}

Reply::Result_t Reply::state() const
{
    return m_state;
}

QJsonObject Reply::hdr() const
{
    return m_hdr;
}

QJsonObject Reply::body() const
{
    return m_body;
}

BoolResult_t Reply::matches(const Request &request)
{
    Request::Result_t rr = request.state();
    if(rr.result() != Request::NoError) {
        return {false, rr.data()};
    }

    Reply candidate(request.toJson());
    if(m_ord != candidate.m_ord) {
        return {false, QObject::tr("Ordinal doesn't match", "SYS")};
    }
    if(m_ver != candidate.m_ver) {
        return {false, QObject::tr("Version doesn't match", "SYS")};
    }
    if(m_cid != candidate.m_cid) {
        return {false, QObject::tr("CID doesn't match", "SYS")};
    }
    if(m_type != candidate.m_type) {
        return {false, QObject::tr("Type doesn't match", "SYS")};
    }

    return {true, ""};
}

void Reply::parse()
{
    QJsonParseError jpe;
    QJsonDocument jd = QJsonDocument::fromJson(QByteArray(m_candidate.toLatin1()), &jpe);

    // 1. Just check correct string ( empty JSON, such as '{}' is correct too )
    if(jpe.error == QJsonParseError::NoError) {
        m_state = {NoError, QObject::tr("Parsing is ok", "SYS")};
    }else{
        m_state = {BadHeader, jpe.errorString()};
        return;
    }

    // Turn Document to QJsonObject
    QJsonObject root = jd.object();
    if(root.isEmpty()) {
        m_state = {BadHeader, QObject::tr("JSON is empty", "SYS")};
        return;
    }

    // Check for parts

    // HEADER
    if(!root[IDS_HEADER].isObject()) {
        m_state = {BadHeader, QObject::tr("JSON has no header", "SYS")};
        return;
    }
    m_hdr = root[IDS_HEADER].toObject();

    if(m_hdr[IDS_VER].isNull() || m_hdr[IDS_VER].toInt(BAD_INT)==BAD_INT) {
        m_state = {BadHeader, QObject::tr("JSON has no valid version", "SYS")};
        return;
    }else{
        m_ver = m_hdr[IDS_VER].toInt();
    }

    if(m_hdr[IDS_CID].isNull() || m_hdr[IDS_CID].toString() == "") {
        m_state = {BadHeader, QObject::tr("JSON has no valid CID", "SYS")};
        return;
    }else{
        m_cid = m_hdr[IDS_CID].toString();
    }

    if(m_hdr[IDS_TYPE].isNull() || m_hdr[IDS_TYPE].toInt(BAD_INT)==BAD_INT) {
        m_state = {BadHeader, QObject::tr("JSON has no valid query type", "SYS")};
        return;
    }else{
        m_type = m_hdr[IDS_TYPE].toInt();
    }

    if(m_hdr[IDS_ORD].isNull() || m_hdr[IDS_ORD].toInt(BAD_INT)==BAD_INT) {
        m_state = {BadHeader, QObject::tr("JSON has no valid query ordinal", "SYS")};
        return;
    }else{
        m_ord = m_hdr[IDS_ORD].toInt();
    }

    if(m_hdr[IDS_RESULT].isNull() || m_hdr[IDS_RESULT].toInt(BAD_INT)==BAD_INT) {
        m_state = {BadHeader, QObject::tr("JSON has no valid result", "SYS")};
        return;
    }else{
        m_result = m_hdr[IDS_RESULT].toInt();
    }

    // BODY
    if(!root[IDS_BODY].isObject()) {
        m_state = {BadHeader, QObject::tr("JSON has no body", "SYS")};
        return;
    }
    m_body = root[IDS_BODY].toObject();

    // ~Check for parts

    return;
}

