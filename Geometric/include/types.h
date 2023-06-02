#ifndef TYPES_H_
#define TYPES_H_

#include <cstdint>
#include <vector>


#define errquit(m) { perror(m); exit(-1); }
// error handling


namespace geometric {
    using id_type = int32_t;
    using metric_type = double;
    using coordinate_type = std::vector<metric_type>;
    struct node {
        id_type ID;
        coordinate_type point;
    };
}

#endif