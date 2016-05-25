#ifndef ITABLE
#define ITABLE

#include "sb_def.h"

class ITable {

public:
    virtual SqlStringResult_t create(V1_t v1) = 0;
    virtual SqlStringResult_t create(V2_t v2) = 0;
    virtual SqlStringResult_t create(V3_t v2) = 0;

    // V2
    virtual SqlStringResult_t upgrade(V1_t old, V2_t fresh) = 0;

    // V3
    virtual SqlStringResult_t upgrade(V1_t old, V3_t fresh) = 0;
    virtual SqlStringResult_t upgrade(V2_t old, V3_t fresh) = 0;

};

#endif // ITABLE

