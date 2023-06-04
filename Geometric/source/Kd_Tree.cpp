
#include "Kd_Tree.h"
#include "distance.h"
#include "types.h"

#include <algorithm>
#include <utility>
#include <vector>
#include <limits>

namespace geometric {

heapItem::heapItem(metric_type _dis, KdNode *nd) : dis(_dis), item(nd) {}

compare::compare() : dim(-1) {}

compare::compare(ssize_t d) : dim(d) {}

bool compare::operator()(const heapItem &a, const heapItem &b) {
    return a.dis == b.dis ? a.item->id < b.item->id : a.dis < b.dis;
}

bool compare::operator()(const KdNode &a, const KdNode &b) {
    return a.point[dim] < b.point[dim];
}

KdNode::KdNode(coordinate_type _pt, id_type _id) : id(_id), depth(0), point(_pt), 
    low_bound(_pt), hi_bound(_pt), lo_node(nullptr), hi_node(nullptr) {
} 

int Kd_Tree::insert(coordinate_type pt, id_type id) {
    if(pt.size() != dim) {
        return -1; // dim not match
    }
    if(id_set.find(id) != id_set.end()) {
        return -1; // duplicate id
    }
    int i;
    tree.emplace_back(pt, id);
    KdNode *&cur = root, *node = &tree.back();
    for(i = 0; cur != nullptr; ++i) {            
        int curdim = i % dim;
        for(size_t j = 0; j < dim; ++j) {
            if(pt[j] < cur->low_bound[j])
                cur->low_bound[j] = pt[j];
            if(pt[j] > cur->hi_bound[j])
                cur->hi_bound[j] = pt[j];
        }
        if(pt[curdim] <= cur->point[curdim]) {
            cur = cur->lo_node; 
        }
        else {
            cur = cur->hi_node;
        }
    }
    cur = node;
    cur->depth = i;
    id_set.insert(id);
    return 0;
}

KdNode* Kd_Tree::build_tree(size_t depth, size_t l, size_t r) {
    if(r - l <= 1) {
        if(l != r) {
            tree[l].depth = depth;
            return &tree[l];
        }
        return nullptr;
    }
    print("depth: %lu, range: [%lu, %lu)\n", depth, l, r);
    size_t m = (l + r) >> 1;
    std::nth_element(tree.begin() + l, tree.begin() + m, 
                     tree.begin() + m, compare(depth % dim));
    tree[m].depth = depth;
    tree[m].lo_node = build_tree(depth + 1, l, m);
    tree[m].hi_node = build_tree(depth + 1, m + 1, r);
    for(size_t i = 0; i < dim; ++i) {
        if(tree[m].lo_node) {
            tree[m].low_bound[i] =  std::min(tree[m].low_bound[i], tree[m].lo_node->low_bound[i]);
            tree[m].hi_bound[i] =  std::max(tree[m].hi_bound[i], tree[m].lo_node->hi_bound[i]);
        }
        if(tree[m].hi_node) {
            tree[m].low_bound[i] =  std::min(tree[m].low_bound[i], tree[m].hi_node->low_bound[i]);
            tree[m].hi_bound[i] =  std::max(tree[m].hi_bound[i], tree[m].hi_node->hi_bound[i]);
        }
    }
    return &tree[m];
}

Kd_Tree::Kd_Tree(std::vector<coordinate_type> points, std::vector<id_type> IDs) : dim(points[0].size()) {
    distance = new L2Distance;
    if(points.size() != IDs.size()) {
        errquit("Inconsistent number of points and their IDs.");
    }
    for(size_t i = 0; i < IDs.size(); ++i) {
        if(id_set.find(IDs[i]) != id_set.end()) {
            errquit("Duplicate id!!");
        }
        id_set.insert(IDs[i]);
        if(points[i].size() != dim) {
            errquit("Point with different dim found!!!");
        }
        tree.emplace_back(points[i], IDs[i]);   
    }
    root = build_tree(0, 0, tree.size());
}

// return true when the bounds of the node completely contain the ball with radius d around pt
bool Kd_Tree::ball_within_bounds(const coordinate_type &pt, metric_type d, KdNode *x) {
    size_t i;
    for(i = 0; i < dim; ++i) {
        if(distance->coordinate_distance(pt[i], x->low_bound[i], i) <= d || 
           distance->coordinate_distance(pt[i], x->hi_bound[i], i)  <= d) return false;
    }
    return true;
}

// return true when the bounds of the node overlap with the ball with radius d around pt
bool Kd_Tree::bounds_overlap_ball(const coordinate_type &pt, metric_type d, KdNode *x) {
    metric_type distsum = 0.0;
    size_t i;
    for(i = 0; i < dim; ++i) {
        if(pt[i] < x->low_bound[i]) {
            distsum += distance->coordinate_distance(pt[i], x->low_bound[i], i);
            if(distsum > d) return false;
        }
        else if(pt[i] > x->hi_bound[i]) {
            distsum += distance->coordinate_distance(pt[i], x->hi_bound[i], i);
            if(distsum > d) return false;
        }
    }
    return true;
}

bool Kd_Tree::neighbot_search(const coordinate_type &pt, KdNode* x, size_t k, itemHeap* pq) {
    metric_type dist, curdis = distance->distance(pt, x->point);
    print("neighbor search, depth: %lu, queue size: %lu\n", x->depth, pq->size());
    if(pq->size() < k) {
        pq->emplace(curdis, x);
    }
    else if(curdis < pq->top().dis) {
        pq->pop();
        pq->emplace(curdis, x);
    }
    size_t curdim = x->depth % dim;
    // search the near side
    if(pt[curdim] < x->point[curdim]) {
        if(x->lo_node)
            if(neighbot_search(pt, x->lo_node, k, pq)) return true;
    }
    else {
        if(x->hi_node)
            if(neighbot_search(pt, x->hi_node, k, pq)) return true;
    }
    // search far side, if necessary
    if(pq->size() < k) {
        dist = std::numeric_limits<metric_type>::max();
    }
    else {
        dist = pq->top().dis;
    }
    if(pt[curdim] < x->point[curdim]) {
        if(x->hi_node && bounds_overlap_ball(pt, dist, x->hi_node))
            if(neighbot_search(pt, x->hi_node, k, pq)) return true;
    }
    else {
        if(x->lo_node && bounds_overlap_ball(pt, dist, x->lo_node))
            if(neighbot_search(pt, x->lo_node, k, pq)) return true;
    }
    if(pq->size() == k) dist = pq->top().dis;
    return ball_within_bounds(pt, dist, x);
}

void Kd_Tree::range_search(const coordinate_type &pt, KdNode *x, metric_type r, std::vector<id_type> *res) {
    metric_type curdis = distance->distance(pt, x->point);
    if(curdis <= r) {
        res->emplace_back(x->id);
    }
    print("range search, depth: %lu\n", x->depth);
    if(x->lo_node != nullptr && bounds_overlap_ball(pt, r, x->lo_node)) {
        range_search(pt, x->lo_node, r, res);
    }
    if(x->hi_node != nullptr && bounds_overlap_ball(pt, r, x->hi_node)) {
        range_search(pt, x->hi_node, r, res);
    }
}

std::vector<id_type> Kd_Tree::K_nearest(const coordinate_type &pt, size_t k) {
    if(pt.size() != dim) {
        errquit("Query dim did not match.");
    }
    print("K nearest querying...\n");
    itemHeap *heap = new itemHeap;
    if(k >= id_set.size()) {
        k = id_set.size();
        for(size_t i = 0; i < tree.size(); ++i) {
            metric_type d = distance->distance(pt, tree[i].point);
            heap->emplace(d, &tree[i]);
        }
    }
    else {
        neighbot_search(pt, root, k, heap);
    }
    std::vector<id_type> ret;
    while(!heap->empty()) {
        KdNode *node = heap->top().item;
        heap->pop();
        ret.emplace_back(node->id);
    }
    std::reverse(begin(ret), end(ret));
    return ret;
}   

std::vector<id_type> Kd_Tree::range_query(coordinate_type center, metric_type r) {
    if(center.size() != dim) {
        errquit("Query dim did not match.");
    }
    if(distance->distance() == 2) {
        r = r * r;
    }   
    print("Range Search...\n");
    std::vector<id_type> ret;
    range_search(center, root, r, &ret);
    print("finish.\n");
    sort(begin(ret), end(ret)); // sort by id
    return ret;
}

size_t Kd_Tree::ndim() {
    return dim;
}

};
