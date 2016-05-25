#ifndef ISTORAGEVERSIONER
#define ISTORAGEVERSIONER

#include "sb_def.h"

class IStorageVersioner {
public:
    virtual SqlStringResult_t create(V1_t v1) = 0;
    virtual SqlStringResult_t create(V2_t v2) = 0;
    virtual SqlStringResult_t create(V3_t v2) = 0;

    virtual SqlStringResult_t upgrade(V1_t old, V2_t fresh) = 0;
    virtual SqlStringResult_t upgrade(V2_t old, V3_t fresh) = 0;
    virtual SqlStringResult_t upgrade(V1_t old, V3_t fresh) = 0;
};

#endif // ISTORAGEVERSIONER

