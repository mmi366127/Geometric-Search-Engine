#pragma once

#include "distance.h"
#include "types.h"

#include <unordered_set>
#include <iostream>
#include <cstdint>
#include <vector>



namespace geometric {
    class BruteForce {
        public:
            BruteForce(std::vector<coordinate_type> points, std::vector<id_type> IDs);
            std::vector<id_type> K_nearest(const coordinate_type &coordinate, int k);
            std::vector<id_type> range_query(coordinate_type center, metric_type range);
            int insert(coordinate_type pt, id_type id); 
            int remove(id_type id);
            size_t ndim();
        private:
            size_t dim;
            Metric *distance;
            std::vector<id_type> point_ids;
            std::unordered_set<id_type> id_set;
            std::vector<coordinate_type> points;
    };
}
