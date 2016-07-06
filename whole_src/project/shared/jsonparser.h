#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>
#include <QJsonObject>
#include <QSharedPointer>

#include "shared_result.h"


/**
 * @brief The JsonParser class
 *  Responsible:
 *  Prepare string to Json, parse to model objects, checks basic structure, such as HEADER, BODY, VERSION, DEBUG, etc.
 */
class JsonParser : public QObject
{
    Q_OBJECT
public:

    enum Error_e {
        JPE_NoError,        ///< No error found
        JPE_SmthWrong,      ///< Some unqualified error
        JPE_Empty,          ///< Empty Json
        JPE_HeaderError,    ///< Header not exists
        JPE_BodyError,      ///< Body not exists
        JPE_VersionError,   ///< Version not exists
        JPE_CIDError,       ///< Client UUID not exists
        JPE_PswError,       ///< Client PSW not exists
        JPE_QryTypeError,   ///< Query type not exists
        JPE_QryOrdError,    ///< Query ordinal number not exists
        JPE__TOPIDX         ///< Top index of this ENUM (May used in descendants)
    };

    static const QString IDS_HEADER;
    static const QString IDS_BODY;
    static const QString IDS_VER;
    static const QString IDS_CID;
    static const QString IDS_PSW;
    static const QString IDS_QRY_TYPE;
    static const QString IDS_QRY_ORD;
    static const QString IDS_DBG_LVL;
    static const QString IDS_RESULT;

    typedef Result<Error_e, QString> Result_t;

//    explicit JsonParser();
    /**
     * @brief JsonParser
     * @param candidate String to be parsed
     * @param parent
     */
    JsonParser(const QString &candidate = QString(), QObject *parent = 0);
//    JsonParser(JsonParser &rhs);
    ~JsonParser();

    QString toJson() const;

    Result_t state() const;

    qint32 ver() const;
    JsonParser &setVer(qint32 ver);

    QString cid() const;
    JsonParser &setCid(const QString &cid);

    QString psw() const;
    JsonParser &setPsw(const QString &psw);

    qint32 type() const;
    JsonParser &setType(const qint32 &type);

    qint32 ord() const;
    JsonParser &setOrd(const qint32 &ord);

    qint32 dbg() const;
    JsonParser &setDbg(const qint32 &dbg);

    QString rawCandidate() const;
    QString jsonCandidate() const;

    QJsonObject root() const;
    QJsonObject hdr() const;
    QJsonObject body();

    // To find all usages ))))
    virtual void doAbstract() = 0;

signals:

public slots:

private:
    QString m_candidate;
    QString m_jsonCandidate;
    qint32 m_ver;
    QString m_cid;
    QString m_psw;
    qint32 m_type;
    qint32 m_ord;
    qint32 m_dbg;
    Result_t m_state;
    QJsonObject m_root;
    QJsonObject m_hdr;
    QJsonObject m_body;

    void parse();
};

//Q_DECLARE_METATYPE(JsonParser)
Q_DECLARE_METATYPE(QSharedPointer<JsonParser>)

#endif // JSONPARSER_H
