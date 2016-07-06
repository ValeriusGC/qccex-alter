#ifndef REQUEST_H
#define REQUEST_H

#include <QJsonObject>
#include "shared_result.h"

namespace storage {

class Request
{
public:

    enum Error {
        NoError = 0x00,        ///< No error found
        CIDError = 0x01,       ///< Client UUID not exists
        PswError = 0x02,       ///< Client PSW not exists
        __TOPIDX = 0x10        ///< Top index of this ENUM (May used in descendants)
    };

    static const QString IDS_HEADER;
    static const QString IDS_BODY;
    static const QString IDS_VER;
    static const QString IDS_CID;
    static const QString IDS_PSW;
    static const QString IDS_TYPE;
    static const QString IDS_ORD;
    static const QString IDS_DBG_LVL;

    typedef Result<Error, QString> Result_t;

    Request(const QString &cid, const QString &psw, qint32 type, qint32 ordinal, qint32 dbgLevel = 0);
    ~Request();

    /**
     * @brief state
     * Keeps state of request depending on its building process.
     * @return Current state.
     */
    Result_t state() const;

    QString toHttpJson() const;


    /**
     * @brief toJson
     * @return JSON-string
     */
    QString toJson() const;

    Request &insert(const QString &key, const QJsonValue &value);

private:
    Result_t m_state;
    QJsonObject m_hdr;
    QJsonObject m_body;
};


} // namespace storage

#endif // REQUEST_H
