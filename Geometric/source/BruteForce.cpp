
#include "BruteForce.h"
#include "distance.h"
#include "types.h"

#include <algorithm>
#include <utility>
#include <vector>

namespace geometric {

BruteForce::BruteForce(std::vector<coordinate_type> points, std::vector<id_type> IDs) : dim(points[0].size()), point_ids(IDs), points(points) {
    if(points.size() != IDs.size()) {
        errquit("Inconsistent number of points and their IDs.");
    }
    for(size_t i = 0; i < IDs.size(); ++i) {
        if(id_set.find(IDs[i]) != id_set.end()) {
            errquit("Duplicate ID found!!!");
        }
        if(points[i].size() != dim) {
            errquit("Point with different dim found!!!");
        }
        id_set.insert(IDs[i]);
    }
} 

std::vector<id_type> BruteForce::K_nearest(const coordinate_type &coordinate, int k) {
    std::vector<std::pair<metric_type, id_type>> pts;
    for(size_t i = 0; i < points.size(); ++i) {
        pts.emplace_back(distance_l2(coordinate, points[i]), point_ids[i]);
    }
    std::sort(pts.begin(), pts.end());
    std::vector<id_type> ret;
    for(int i = 0; i < std::min(k, (int)pts.size()); ++i) {
        ret.emplace_back(pts[i].second);
    }
    return ret;
}   

std::vector<id_type> BruteForce::range_query(coordinate_type center, metric_type distance) {
    std::vector<id_type> ret;
    for(size_t i = 0; i < points.size(); ++i) {
        if(distance_l1(center, points[i]) < distance) {
            ret.emplace_back(point_ids[i]);
        }
    }
    return ret;
}

int BruteForce::insert(coordinate_type pt, id_type id) {
    if(pt.size() != dim) return -1; // insert fail
    if(id_set.find(id) != id_set.end()) {
        return -1; // duplicate id
    }
    id_set.insert(id);
    point_ids.emplace_back(id);
    points.emplace_back(pt);
    return 0;
}

int BruteForce::remove(id_type id) {
    if(id_set.find(id) == id_set.end()) {
        return -1; // id not exist
    }
    for(size_t i = 0; i < points.size(); ++i) {
        if(point_ids[i] == id) {
            point_ids.erase(point_ids.begin() + i);
            points.erase(points.begin() + i);
            return 0;
        } 
    }
    return -1; // should not reach here
}

};
