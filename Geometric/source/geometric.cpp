
#include "geometric.h"
#include "distance.h"
#include "types.h"

#include <algorithm>
#include <utility>
#include <vector>

#define errquit(m) { perror(m); exit(-1); }

namespace geometric {

Bruteforce::Bruteforce(std::vector<coordinate_type> points, std::vector<id_type> IDs) : BasicGeoTree(points, IDs), points(points) {
    if(points.size() != IDs.size()) {
        errquit("Inconsistent number of points and their IDs.");
    }
} 

std::vector<id_type> Bruteforce::K_nearest(const coordinate_type &coordinate, int k) {
    std::vector<std::pair<metric_type, id_type>> pts;
    for(size_t i = 0; i < points.size(); ++i) {
        pts.emplace_back(distance_l2(coordinate, points[i]), point_ids[i]);
    }
    std::sort(pts.begin(), pts.end());
    multi_id_type ret;
    for(int i = 0; i < std::min(k, (int)pts.size()); ++i) {
        ret.emplace_back(pts[i].second);
    }
    return ret;
}   

std::vector<id_type> Bruteforce::range_query(coordinate_type center, metric_type distance) {
    multi_id_type ret;
    for(size_t i = 0; i < points.size(); ++i) {
        if(distance_l1(center, points[i]) < distance) {
            ret.emplace_back(point_ids[i]);
        }
    }
    return ret;
}



};
