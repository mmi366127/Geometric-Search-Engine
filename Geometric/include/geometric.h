#ifndef GEOMETRIC_H_
#define GEOMETRIC_H_

#include "distance.h"
#include "types.h"

#include <iostream>
#include <vector>
#include <cstdint>


namespace geometric {

class BasicGeoTree {
    public:
        BasicGeoTree(std::vector<coordinate_type> points, std::vector<id_type> IDs);
        virtual std::vector<id_type> K_nearest(const coordinate_type &coordinate, int k);
        virtual std::vector<id_type> range_query(coordinate_type center, metric_type distance);
        virtual void insert(coordinate_type pt, id_type id); 
        virtual void remove(id_type id);
    protected:
        std::vector<id_type> point_ids;
};

class Bruteforce : public BasicGeoTree {
    public:
        Bruteforce(std::vector<coordinate_type> points, std::vector<id_type> IDs);
        std::vector<id_type> K_nearest(const coordinate_type &coordinate, int k);
        std::vector<id_type> range_query(coordinate_type center, metric_type distance);
    private:
        std::vector<coordinate_type> points;
};

}

#endif
