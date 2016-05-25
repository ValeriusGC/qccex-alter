#ifndef BASEMODELITEMLIST_H
#define BASEMODELITEMLIST_H

#include <QObject>

namespace model {


class BaseModelItemList : public QObject
{
    Q_OBJECT
    typedef QObject Inherited_t;
public:
    explicit BaseModelItemList(QObject *parent = 0);
    ~BaseModelItemList();

signals:

public slots:
};

} // namespace model

Q_DECLARE_METATYPE(model::BaseModelItemList*)


#endif // BASEMODELITEMLIST_H
