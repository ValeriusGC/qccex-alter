#ifndef BASETABLE_H
#define BASETABLE_H

#include <QObject>
#include <QVector>
#include <QSqlDatabase>

#include "sb_def.h"

namespace storage {

class BaseTable : public QObject
{
    Q_OBJECT
public:
    explicit BaseTable(QObject *parent = 0);
    ~BaseTable();

    virtual QString name() const = 0;

    SqlStringResult_t create(const QSqlDatabase &db);

    QVector<QString> &fieldNames();

    /**
     *  Returns flag if this value contains in this column.
     * @brief contains
     * @param column
     * @param value
     * @return contains or not this value?
     */
    bool contains(const QString &column, const QVariant &value);

signals:

public slots:

protected:

    //--------------------------------------------------------------------------------
    // Controlling group for all descendants.
    // Change according current DB-version

    /**
     * @brief createV1
     * @param db
     * @return Result of creation.
     */
    virtual SqlStringResult_t createV1(const QSqlDatabase &db) = 0;

    // ~Controlling group for all descendants.
    //--------------------------------------------------------------------------------

private:
    QVector<QString> m_fieldNames;

};

} //

#endif // BASETABLE_H
