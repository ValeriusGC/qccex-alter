/*
 *  (с) PPA, 2014/07
 */

#ifndef SHARED_RESULT_H
#define SHARED_RESULT_H

#include <QString>

#include "shared_def.h"

#ifdef QT_DEBUG
/**
 * @brief g_debugResults
 * 0 - constructors count
 * 1 - copy constructors count
 * 2 - destructors count
 * 3 - total +/- count
 */
extern qint32 debugCounters[4];
#endif


namespace vfx_shared {

/**
 *  Шаблон возвращаемого результата, расширенного строковым полем сообщения.
 *
 *  Возможны следующие простые инициализации:
 *  Result<Type> r = Type;
 *  Result<bool> r1 = true;
 *  Result<Type> r2 = r;
 *  r1 = {false, "message"};
 *
 */
template<typename R, typename D = QString>
class Result
{
public:
    Result(R result, const D& data) {
        r = result;
        d = data;
#ifdef QT_DEBUG
        ++debugCounters[0];
#endif
    }

    Result(const Result& result) {
        r = result.r;
        d = result.d;
#ifdef QT_DEBUG
        ++debugCounters[1];
#endif
    }

    ~Result(){
#ifdef QT_DEBUG
        ++debugCounters[2];
        debugCounters[3] = debugCounters[0] + debugCounters[1] - debugCounters[2];

        if(debugCounters[3] == 0) {
            LOG_TP(QString() << "all Result_t destroyed! [" << debugCounters[2] << "]");
        }
#endif
    }

    Result& operator=(const Result& result){
        this->r = result.r;
        this->d = result.d;
        return *this;
    }

//    Result& operator=(bool result){
//        this->r = result;
//        return *this;
//    }

    R result() const {
        return r;
    }

    D data() const {
        return d;
    }

private:
    R r;
    D d;
};

/**
 *  Сравнение двух объектов, типа так:
 *  Result<bool> r1;
 *  ...
 *  if(r1 == true) ...
 */
template<typename R, typename D>
bool operator==(const Result<R, D>& lhs, bool rhs){
    return lhs.result()==rhs;
}

/**
 *  Сравнение двух объектов, типа так:
 *  Result<bool> r1;
 *  ...
 *  if(r1 != true)...
 */
template<typename R, typename D>
bool operator!=(const Result<R, D>& lhs, bool rhs){
    return lhs.result()!=rhs;
}

/**
 * @brief BoolResult_t
 *  Предустановленный тип для возврата булевого результата
 */
typedef Result<bool> BoolResult_t;

/**
 * @brief BoolListResult_t
 *
 */
typedef Result<bool, QList<QString> > BoolListResult_t;



/**
 * @brief ProgressResult_t
 * Keeps result and message for some task progress result.
 * Values depend on context.
 */
//typedef Result<qint32, QString> ProgressResult_t;

} // namespace vxf_shared

/**
 * Для укороченного обращения к типу извне этого файла.
 */
using vfx_shared::Result;

/**
 * Для укороченного обращения к типу извне этого файла.
 */
using vfx_shared::BoolResult_t;

using vfx_shared::BoolListResult_t;

//using Proman::ProgressResult_t;


// Упрощенное назначение переменной значения {true, ""}
// Пример:
// BoolResult_t res = RES_TRUE
#define RES_TRUE    {true, ""}
// Упрощенное назначение переменной значения {false, message}
// Пример:
// BoolResult_t res = RES_FALSE("error!")
#define RES_FALSE(msg)    {false, msg}
// Упрощенное назначение переменной значения {false, message} с указанием названия класса
// Пример:
// BoolResult_t res = RES_FALSE("error!")
#define RES_FALSE_EXT(msg)    {false, QString("[%1]: %2").arg(this->metaObject()->className()).arg(msg)}


#endif // SHARED_RESULT_H
