#ifndef REPLY_H
#define REPLY_H

#include <QJsonObject>
#include <QSharedPointer>

#include "shared_result.h"

namespace storage {

QT_FORWARD_DECLARE_CLASS(Request)

class Reply
{
public:

    enum Error {
        NoError = 0x00,         ///< No error found
        BadHeader = 0x01,       ///< Some error
        __TOPIDX = 0x10         ///< Top index of this ENUM (May used in descendants)
    };

    static const QString IDS_HEADER;
    static const QString IDS_BODY;
    static const QString IDS_VER;
    static const QString IDS_CID;
    static const QString IDS_TYPE;
    static const QString IDS_ORD;
    static const QString IDS_RESULT;


    typedef Result<Error, QString> Result_t;

    Reply(const QString &candidate);
    ~Reply();

    QString candidate() const;

    /**
     * @brief state
     * Keeps state of Reply depending on its building process.
     * @return Current state.
     */
    Result_t state() const;

    QJsonObject hdr() const;
    QJsonObject body() const;

    /**
     * @brief matches
     * Check if it is reply for particular request.
     *
     * @param request
     * @return true if this is reply to that request.
     */
    BoolResult_t matches(const Request& request);

private:
    Result_t m_state;

    /**
     * @brief m_candidate
     * Input string as candidate to JSON-object
     */
    QString m_candidate;

    QJsonObject m_hdr;
    QJsonObject m_body;

    qint32 m_ver;
    QString m_cid;
    qint32 m_type;
    qint32 m_ord;
    qint32 m_result;

    void parse();

};

} // namespace storage

Q_DECLARE_METATYPE(QSharedPointer<storage::Reply>)

#endif // REPLY_H
