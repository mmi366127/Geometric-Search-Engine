#ifndef DISTANCE_H_
#define DISTANCE_H_

#include "types.h"

namespace geometric {
    metric_type distance_l2(const coordinate_type &a, const coordinate_type &b);
    metric_type distance_l1(const coordinate_type &a, const coordinate_type &b);
}


#endif