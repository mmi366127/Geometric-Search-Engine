#ifndef GEOMETRIC_H_
#define GEOMETRIC_H_

#include "distance.h"
#include "types.h"

#include <unordered_set>
#include <iostream>
#include <cstdint>
#include <vector>



namespace geometric {
    class BruteForce {
        public:
            BruteForce() {}
            BruteForce(std::vector<coordinate_type> points, std::vector<id_type> IDs);
            std::vector<id_type> K_nearest(const coordinate_type &coordinate, int k);
            std::vector<id_type> range_query(coordinate_type center, metric_type distance);
            int insert(coordinate_type pt, id_type id); 
            int remove(id_type id);
        private:
            int dim;
            std::vector<id_type> point_ids;
            std::unordered_set<id_type> id_set;
            std::vector<coordinate_type> points;
    };
}

#endif
