#ifndef STORAGEFACTORY_H
#define STORAGEFACTORY_H

#include <QObject>
#include <QMap>

namespace storage {

QT_FORWARD_DECLARE_CLASS(BaseStorage)

typedef QMap<QString, BaseStorage *> StorageMap_t;

class StorageFactory : public QObject
{
    Q_OBJECT
    typedef QObject Inherited_t;
public:
    /**
     * @brief instance (As Meyers singleton)
     * @return singleton.
     *
     */
    static StorageFactory &instance();

    BaseStorage *make(const QString &type);

signals:

public slots:


private:

    StorageMap_t m_map;

    // Methods
    explicit StorageFactory();
    ~StorageFactory();
    StorageFactory(StorageFactory &);
    StorageFactory &operator =(StorageFactory &);
    // ~Methods

};

} // namespace storage

#endif // STORAGEFACTORY_H
