#pragma once

#include <cstdint>
#include <vector>
#include <stdlib.h>

#define errquit(m) { perror(m); exit(-1); }
// error handling

#ifdef __DEBUG__
#define print printf
#else
#define print(...) 
#endif

namespace geometric {
    using id_type = int32_t;
    using metric_type = double;
    using coordinate_type = std::vector<metric_type>;
    struct node {
        id_type ID;
        coordinate_type point;
    };
}
