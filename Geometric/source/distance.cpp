#include "distance.h"
#include "types.h"

#include <cstdlib>
#include <cstdio>
#include <cmath>

#define errquit(m) { perror(m); exit(-1); }

namespace geometric {

// rect

rect::rect(size_t dim) : low_bound(dim), hi_bound(dim) {}

size_t rect::dim() const {
    return low_bound.size();
}

void rect_expand(rect &r, const rect &other) {
    if(r.dim() != other.dim()) errquit("rect/expand: dim not match.");
    for (size_t i = 0; i < r.dim(); ++i) {
        if (other.low_bound[i] < r.low_bound[i]) r.low_bound[i] = other.low_bound[i];
        if (other.hi_bound[i] > r.hi_bound[i]) r.hi_bound[i] = other.hi_bound[i];
    }
}

void rect_expand(rect &r, const coordinate_type &other) {
    if(r.dim() != other.size()) errquit("rect/expand: dim not match.");
    for(size_t i = 0; i < r.dim(); ++i) {
        if (other[i] < r.low_bound[i]) r.low_bound[i] = other[i];
        if (other[i] > r.hi_bound[i]) r.hi_bound[i] = other[i];
    }
}

rect rect_union(rect a, const rect &b) {
    if(a.dim() != b.dim()) errquit("rect/union: dim not match.");
    rect_expand(a, b);
    return a;
}

metric_type area(const rect &r) {
    metric_type area = r.hi_bound[0] - r.low_bound[0];
    for (size_t i = 1; i < r.dim(); ++i) {
        area *= r.hi_bound[i] - r.low_bound[i];
    }
    return area;
}

bool rect_contains(const rect &r, const rect &other) {
    if(r.dim() != other.dim()) errquit("rect/contain: dim not match.");
    for (size_t i = 0; i < r.dim(); i++) {
        if (other.low_bound[i] < r.low_bound[i] || other.hi_bound[i] > r.hi_bound[i]) {
            return false;
        }
    }
    return true;
}

bool rect_intersects(const rect &r, const rect &other) {
    if(r.dim() != other.dim()) errquit("rect/intersect: dim not match.");
    for (size_t i = 0; i < r.dim(); i++) {
        if (other.low_bound[i] > r.hi_bound[i] || other.hi_bound[i] < r.low_bound[i]) {
            return false;
        }
    }
    return true;
}

int rect_largest_axis(const rect &r) {
    int axis = 0;
    metric_type nlength = r.hi_bound[0] - r.low_bound[0];
    for (size_t i = 1; i < r.dim(); i++) {
        metric_type length = r.hi_bound[i] - r.low_bound[i];
        if (length > nlength) {
            nlength = length;
            axis = i;
        }
    }
    return axis;
}


// distance metric
int L1Distance::distance() { return 1; }

int L2Distance::distance() { return 2; }

int LInfDistance::distance() { return 0; }

metric_type L1Distance::minDistance(const coordinate_type &p, const rect &r) {
    if(p.size() != r.dim()) errquit("minDistance: dim not match!!!");
    metric_type ret = 0;
    for(size_t i = 0; i < p.size(); ++i) {
        if(r.low_bound[i] > p[i]) {
            ret += r.low_bound[i] - p[i];
        }
        else if(p[i] < r.hi_bound[i]) {
            ret += r.hi_bound[i] - p[i];
        }
    }
    return ret;
}

metric_type L2Distance::minDistance(const coordinate_type &p, const rect &r) {
    if(p.size() != r.dim()) errquit("minDistance: dim not match!!!");
    metric_type ret = 0;
    for(size_t i = 0; i < p.size(); ++i) {
        if(r.low_bound[i] > p[i]) {
            ret += (r.low_bound[i] - p[i]) * (r.low_bound[i] - p[i]);
        }
        else if(p[i] < r.hi_bound[i]) {
            ret += (r.hi_bound[i] - p[i]) * (r.hi_bound[i] - p[i]);
        }
    }
    return ret;
}

metric_type LInfDistance::minDistance(const coordinate_type &p, const rect &r) {
    if(p.size() != r.dim()) errquit("minDistance: dim not match!!!");
    metric_type ret = 0;
    for(size_t i = 0; i < p.size(); ++i) {
        if(r.low_bound[i] > p[i]) {
            ret = std::max(ret, r.low_bound[i] - p[i]);
        }
        else if(p[i] < r.hi_bound[i]) {
            ret = std::max(r.hi_bound[i] - p[i], ret);
        }
    }
    return ret;
}

metric_type L1Distance::distance(const coordinate_type& p, const coordinate_type& q) {
    if(p.size() != q.size()) errquit("distance metric failed, coordinates should have same dims.");
    metric_type ret = 0;
    for(size_t i = 0; i < p.size(); ++i) {
        ret += fabs(p[i] - q[i]);
    }
    return ret;
}

metric_type L2Distance::distance(const coordinate_type& p, const coordinate_type& q) {
    if(p.size() != q.size()) errquit("distance metric failed, coordinates should have same dims.");
    metric_type ret = 0;
    for(size_t i = 0; i < p.size(); ++i) {
        ret += (p[i] - q[i]) * (p[i] - q[i]); 
    }
    return ret;
}

metric_type LInfDistance::distance(const coordinate_type& p, const coordinate_type& q) {
    if(p.size() != q.size()) errquit("distance metric failed, coordinates should have same dims.");
    metric_type ret = 0;
    for(size_t i = 0; i < p.size(); ++i) {
        ret = std::max(ret, fabs(p[i] - q[i]));
    }
    return ret;
}

metric_type L1Distance::coordinate_distance(metric_type x, metric_type y, size_t dim) {
    return fabs(x - y);
}

metric_type L2Distance::coordinate_distance(metric_type x, metric_type y, size_t dim) {
    return (x - y) * (x - y);
}

metric_type LInfDistance::coordinate_distance(metric_type x, metric_type y, size_t dim) {
    return fabs(x - y);
}

}