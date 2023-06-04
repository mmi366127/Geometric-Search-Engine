#pragma once

#include "distance.h"
#include "types.h"

#include <unordered_set>
#include <iostream>
#include <cstdint>
#include <vector>
#include <queue>


namespace geometric {    
    const int MAX_ENTRIES = 5;
    const int MIN_ENTRIES_PERCENTAGE = 40 ;
    const int MIN_ENTRIES = ((MAX_ENTRIES) * (MIN_ENTRIES_PERCENTAGE) / 100 + 1);
    enum nodeType {
        LEAF = 0,
        BRANCH = 1
    };
    struct item {
        id_type id;
        coordinate_type point;
        item(const id_type&, const coordinate_type&);
    };
    struct RNode {
        rect box;
        enum nodeType type;
        std::vector<void*> ptr;
        RNode(enum nodeType, size_t);
        size_t size() const;
        // if LEAF, store item, else store RNode
    };
    struct RheapItem {
        double dis;  // distance between the query point
        item *_item; // pointer of the node 
        RheapItem(metric_type, item*);
    };
    struct Rcompare {
        ssize_t dim;
        bool operator()(const RheapItem &a, const RheapItem &b);
    };
    typedef std::priority_queue<RheapItem, std::vector<RheapItem>, Rcompare> RitemHeap;
    class RTree {
        public:
            ~RTree();
            RTree(std::vector<coordinate_type> points, std::vector<id_type> IDs);
            std::vector<id_type> K_nearest(const coordinate_type &pt, size_t k);
            std::vector<id_type> range_query(coordinate_type center, metric_type r);
            size_t ndim();
        
        private:
            size_t dim;
            RNode *root;
            Metric *distance;
            std::vector<item> items;
            std::unordered_set<id_type> id_set;
            
            RNode *split(RNode *x);
            void free_tree(RNode *x);
            RNode *node_insert(RNode*, item*);
            int choose_subtree(const RNode *x, const rect &r);
            int insert(const coordinate_type &pt, const id_type &id);
            metric_type minDistance(const coordinate_type &c, const rect &r);
            void neighbor_search(const coordinate_type&, RNode*, size_t, RitemHeap*);
            bool ball_within_bounds(const coordinate_type&, metric_type, RNode*);
            bool bounds_overlap_ball(const coordinate_type&, metric_type, RNode*);
            void range_search(RNode *x, const coordinate_type &c, const metric_type &r, std::vector<id_type>*);
    };
}

