#pragma once

#include "types.h"

namespace geometric {
    //--------------------------------------------------------------
    // different distance metrics
    //--------------------------------------------------------------
    struct rect {
        coordinate_type low_bound, hi_bound;
        rect(size_t dim);
        size_t dim() const;
    };
    void rect_expand(rect &r, const rect &other);
    void rect_expand(rect &r, const coordinate_type &other);
    rect rect_union(rect a, const rect &b);
    metric_type area(const rect &r);
    bool rect_contains(const rect &r, const rect &other);
    bool rect_intersects(const rect &r, const rect &other);
    int rect_largest_axis(const rect &r);
    class Metric {
        public:
            Metric() {}
            ~Metric() {}
            virtual int distance() = 0;
            virtual metric_type minDistance(const coordinate_type &p, const rect &r) = 0;
            virtual metric_type distance(const coordinate_type &p, const coordinate_type &q) = 0;
            virtual metric_type coordinate_distance(metric_type x, metric_type y, size_t dim) = 0;
    };
    class L1Distance : public virtual Metric {
        public:
            int distance();
            metric_type minDistance(const coordinate_type &p, const rect &r);
            metric_type distance(const coordinate_type& p, const coordinate_type& q);
            metric_type coordinate_distance(metric_type x, metric_type y, size_t dim);
    };
    class L2Distance : public virtual Metric {
        public:
            int distance();
            metric_type minDistance(const coordinate_type &p, const rect &r);
            metric_type distance(const coordinate_type& p, const coordinate_type& q);
            metric_type coordinate_distance(metric_type x, metric_type y, size_t dim);
    };
    class LInfDistance : public virtual Metric {
        public:
            int distance();
            metric_type minDistance(const coordinate_type &p, const rect &r);
            metric_type distance(const coordinate_type& p, const coordinate_type& q);
            metric_type coordinate_distance(metric_type x, metric_type y, size_t dim);
    };
    metric_type distance_l2(const coordinate_type &a, const coordinate_type &b);
    metric_type distance_l1(const coordinate_type &a, const coordinate_type &b);
}
