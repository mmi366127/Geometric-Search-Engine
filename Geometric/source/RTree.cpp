
#include "RTree.h"
#include "distance.h"
#include "types.h"

#include <algorithm>
#include <utility>
#include <vector>
#include <limits>

namespace geometric {

// item

item::item(const id_type &_, const coordinate_type &pt) : id(_), point(pt) {}

// RTNode

RNode::RNode(enum nodeType t, size_t dim) : type(t), box(dim) {}

size_t RNode::size() const {
    return ptr.size();
}

// RTree

void sort_nodes(RNode *x) {
    std::sort(x->ptr.begin(), x->ptr.end(), [&](void *a, void *b){
        if(x->type == LEAF) {   
            item *_a, *_b;
            _a = static_cast<item*>(a);
            _b = static_cast<item*>(b);
            return _a->point[0] < _b->point[0];
        }
        else {
            RNode *_a, *_b;
            _a = static_cast<RNode*>(a);
            _b = static_cast<RNode*>(b);
            return _a->box.low_bound[0] < _b->box.low_bound[0];
        }
    });
}

void update_box(RNode *x) {
    if(x->type == LEAF) {    
        item *it = static_cast<item*>(x->ptr[0]);
        x->box.low_bound = x->box.hi_bound = it->point;
        for(size_t i = 1; i < x->size(); ++i) {
            it = static_cast<item*>(x->ptr[i]);
            rect_expand(x->box, it->point);
        }
    }
    else {
        RNode *node = static_cast<RNode*>(x->ptr[0]);
        x->box = node->box;
        for(size_t i = 1; i < x->size(); ++i) {
            node = static_cast<RNode*>(x->ptr[i]);
            rect_expand(x->box, node->box);
        }
    }
}

// split the original node x into 2 nodes x, y
// the returned node is y, and the order is not guarantee.
RNode* RTree::split(RNode *x) {
    if(!x) return nullptr;
    int axis = rect_largest_axis(x->box);
    RNode *y = new RNode(x->type, x->box.dim());
    std::vector<void*> temp;
    temp.swap(x->ptr);
    for(size_t i = 0; i < temp.size(); ++i) {
        metric_type min_dist, max_dist;
        if(x->type == LEAF) {
            min_dist = static_cast<item*>(temp[i])->point[axis] - x->box.low_bound[axis];
            max_dist = x->box.hi_bound[axis] - static_cast<item*>(temp[i])->point[axis];
        }
        else {
            min_dist = static_cast<RNode*>(temp[i])->box.low_bound[axis] - x->box.low_bound[axis];
            max_dist = x->box.hi_bound[axis] - static_cast<RNode*>(temp[i])->box.hi_bound[axis];
        }
        if(min_dist < max_dist) {
            // put left
            x->ptr.emplace_back(temp[i]);
        }
        else {
            // put right
            y->ptr.emplace_back(temp[i]);
        }
    }
    // check balance
    if(x->size() < MIN_ENTRIES) {
        // sort low_bound by axis decreasingly
        std::sort(y->ptr.begin(), y->ptr.end(), [&](void *a, void *b){
            if(x->type == LEAF) {   
                item *_a, *_b;
                _a = static_cast<item*>(a);
                _b = static_cast<item*>(b);
                return _a->point[axis] > _b->point[axis];
            }
            else {
                RNode *_a, *_b;
                _a = static_cast<RNode*>(a);
                _b = static_cast<RNode*>(b);
                return _a->box.low_bound[axis] > _b->box.low_bound[axis];
            }
        });
        do {
            x->ptr.emplace_back(y->ptr.back());
            y->ptr.pop_back();
        } while(x->size() < MIN_ENTRIES);
    }
    else if(y->size() < MIN_ENTRIES){   
        // sort hi_bound by axis decreasingly
        std::sort(x->ptr.begin(), x->ptr.end(), [&](void *a, void *b){
            if(x->type == LEAF) {   
                item *_a, *_b;
                _a = static_cast<item*>(a);
                _b = static_cast<item*>(b);
                return _a->point[axis] > _b->point[axis];
            }
            else {
                RNode *_a, *_b;
                _a = static_cast<RNode*>(a);
                _b = static_cast<RNode*>(b);
                return _a->box.hi_bound[axis] > _b->box.hi_bound[axis];
            }
        });
        do {
            y->ptr.emplace_back(x->ptr.back());
            x->ptr.pop_back();
        } while(y->size() < MIN_ENTRIES);
    }
    // sort low_bound by axis 0 increasingly
    sort_nodes(x); 
    sort_nodes(y);
    // update bounding box
    update_box(x);
    update_box(y);
    return y;
}

RTree::RTree(std::vector<coordinate_type> points, std::vector<id_type> IDs) : dim(points[0].size()), root(nullptr) {
    distance = new L2Distance;
    if(points.size() != IDs.size()) {
        errquit("Inconsistent number of points and their IDs.");
    }
    items.reserve(points.size());
    for(size_t i = 0; i < IDs.size(); ++i) {
        if(id_set.find(IDs[i]) != id_set.end()) {
            errquit("Duplicate id!!");
        }
        id_set.insert(IDs[i]);
        if(points[i].size() != dim) {
            errquit("Point with different dim found!!!");
        }
        insert(points[i], IDs[i]);
    }
}

RTree::~RTree() {
    print("free memory");
    free_tree(root);
}

int RTree::choose_subtree(const RNode *x, const rect &r) {
    for(size_t i = 0; i < x->size(); ++i) {
        RNode *node = static_cast<RNode*>(x->ptr[i]);
        if(rect_contains(node->box, r)) return i;
    }
    // choose subtree with least enlargement
    size_t ret = 0;
    metric_type delta = std::numeric_limits<metric_type>::max();
    for(size_t i = 0; i < x->size(); ++i) {
        RNode *node = static_cast<RNode*>(x->ptr[i]);
        rect temp = node->box;
        rect_expand(temp, r);
        metric_type area_bf = area(node->box);
        metric_type area_af = area(temp);
        if(area_af - area_bf < delta) {
            delta = area_af - area_bf;
            ret = i;
        }
    }
    return ret;
}

void RTree::free_tree(RNode *x) {
    if(x->type == LEAF) {
        delete x;
        return;
    }
    for(size_t i = 0; i < x->size(); ++i) {
        RNode *node = static_cast<RNode*>(x->ptr[i]);
        free_tree(node);
    }
    delete x;
}

RNode* RTree::node_insert(RNode *x, item *pt) {
    if(x->type == LEAF) {
        // insert
        size_t i;
        for(i = 0; i < x->size(); ++i) {
            item *it = (item*)x->ptr[i];
            if(it->point[0] >= pt->point[0]) {
                break;
            }
        }
        // update bounding box
        rect_expand(x->box, pt->point);
        // insert item
        x->ptr.insert(x->ptr.begin() + i, pt);
        // need split
        if(x->size() > MAX_ENTRIES) { 
            return split(x);
        }    
        return nullptr;
    }   
    // choose sub-tree to insert
    rect box(pt->point.size());
    box.low_bound = box.hi_bound = pt->point;
    int subtree = choose_subtree(x, box);
    // update bounding box
    rect_expand(x->box, pt->point);

    // insert to sub-tree
    RNode *y = node_insert(static_cast<RNode*>(x->ptr[subtree]), pt);
    if(y == nullptr) {
        // no extra node to insert
        sort_nodes(x);
        return nullptr;
    }
    else {
        // insert new node
        x->ptr.emplace_back(y);
        
        // need split 
        if(x->size() > MAX_ENTRIES) {
            return split(x);
        }
        else {
            // update bounding box
            sort_nodes(x);
        }
    }
    return nullptr;
}

int RTree::insert(const coordinate_type &pt, const id_type &id) {
    print("insert %d\n", id);
    if(!root) {
        root = new RNode(LEAF, pt.size());
        items.emplace_back(id, pt);
        node_insert(root, &items.back());
    }
    else {
        items.emplace_back(id, pt);
        RNode *y = node_insert(root, &items.back());
        if(y) {
            RNode *new_root = new RNode(BRANCH, pt.size());
            new_root->ptr.emplace_back(root);
            new_root->ptr.emplace_back(y);
            sort_nodes(new_root);
            update_box(new_root);
            root = new_root;
        }
    }
}

void RTree::range_search(RNode *x, const coordinate_type &c, const metric_type &r, std::vector<id_type> *res) {
    metric_type curdis = distance->minDistance(c, x->box);
    if(curdis > r) return;
    if(x->type == LEAF) {
        for(size_t i = 0; i < x->size(); ++i) {
            item *it = static_cast<item*>(x->ptr[i]);
            if(distance->distance(it->point, c) <= r)
                res->emplace_back(it->id);
        }
    }
    else {    
        for(size_t i = 0; i < x->size(); ++i) {
            range_search(static_cast<RNode*>(x->ptr[i]), c, r, res);    
        }
    }
}

void RTree::neighbor_search(const coordinate_type &pt, RNode* x, size_t k, RitemHeap* pq) {
    print("neighbor search, queue size %lu\n", pq->size());
    if(x->type == LEAF) {
        
    }
    else {    
        for(size_t i = 0; i < x->size(); ++i) {
            RNode *node = static_cast<RNode*>(x->ptr[i]);
            metric_type curdis = distance->minDistance(pt, node->box);
            if(pq->size() < k) {
                neighbor_search(pt, node, k, pq);
            }
            else if(curdis < pq->top().dis) {
                neighbor_search(pt, node, k, pq);
            }
        }
    }
}

// public functions

std::vector<id_type> RTree::range_query(coordinate_type center, metric_type r) {
    if(center.size() != dim) errquit("RTree range query: dim not match");
    std::vector<id_type> ret;
    range_search(root, center, r, &ret);
    return ret;
}

std::vector<id_type> RTree::K_nearest(const coordinate_type &pt, size_t k) {

}

int RTree::ndim() {
    return dim;
}

};
