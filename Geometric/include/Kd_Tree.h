#pragma once

#include "distance.h"
#include "types.h"

#include <unordered_set>
#include <iostream>
#include <cstdint>
#include <vector>
#include <queue>


namespace geometric {    
    struct KdNode {
        id_type id;
        size_t depth;
        coordinate_type point;  
        coordinate_type low_bound, hi_bound;
        KdNode *lo_node, *hi_node;
        KdNode(coordinate_type, id_type);
    };
    struct heapItem {
        double dis;  // distance between the query point
        KdNode *item; // pointer of the node 
        heapItem(metric_type, KdNode*);
    };
    struct compare {
        ssize_t dim;
        compare(); // distance comparator
        compare(ssize_t dim); // dim specific comparator
        bool operator()(const KdNode &a, const KdNode &b);
        bool operator()(const heapItem &a, const heapItem &b);
    };
    typedef std::priority_queue<heapItem, std::vector<heapItem>, compare> itemHeap;
    class Kd_Tree {
        public:
            Kd_Tree(std::vector<coordinate_type> points, std::vector<id_type> IDs);
            std::vector<id_type> K_nearest(const coordinate_type &pt, size_t k);
            std::vector<id_type> range_query(coordinate_type center, metric_type r);
            size_t ndim();
        private:
            size_t dim;
            KdNode *root;
            Metric *distance;
            std::vector<KdNode> tree;
            std::unordered_set<id_type> id_set;
            int insert(coordinate_type pt, id_type id);
            KdNode *build_tree(size_t depth, size_t l, size_t r);
            bool neighbot_search(const coordinate_type&, KdNode*, size_t, itemHeap*);
            void range_search(const coordinate_type&, KdNode*, metric_type, std::vector<id_type>*);
            bool ball_within_bounds(const coordinate_type&, metric_type, KdNode*);
            bool bounds_overlap_ball(const coordinate_type&, metric_type, KdNode*);
    };
}

