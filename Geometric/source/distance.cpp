#include "distance.h"
#include "types.h"

#include <cstdlib>
#include <cstdio>
#include <cmath>

#define errquit(m) { perror(m); exit(-1); }

namespace geometric {
    metric_type distance_l2(const coordinate_type &a, const coordinate_type &b) {
        if(a.size() != b.size()) errquit("distance metric failed, coordinates should have same dims.");
        metric_type ret = 0;
        for(size_t i = 0; i < a.size(); ++i) {
            ret += sqrt((a[i] - b[i]) * (a[i] - b[i]));
        }
        return ret;
    }
    metric_type distance_l1(const coordinate_type &a, const coordinate_type &b) {
        if(a.size() != b.size()) errquit("distance metric failed, coordinates should have same dims.");
        metric_type ret = 0;
        for(size_t i = 0; i < a.size(); ++i) {
            ret += fabs(a[i] - b[i]);
        }
        return ret;
    }
}