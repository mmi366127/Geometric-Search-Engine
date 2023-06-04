#include "distance.h"
#include "BruteForce.h"
#include "Kd_Tree.h"
#include "RTree.h"

#include <random>
#include <time.h>

using namespace geometric;
using namespace std;

#define print printf

std::random_device rd;
std::default_random_engine gen = std::default_random_engine(rd());
std::uniform_real_distribution<metric_type> dis(-1e9, 1e9);

coordinate_type random_point(size_t dim) {
    coordinate_type ret(dim);
    for(size_t i = 0; i < dim; ++i) {
        ret[i] = dis(gen);
    }
    return ret;
}

using dataset = pair<vector<coordinate_type>, vector<id_type>>;

dataset make_dataset(size_t n, size_t dim) {
    vector<id_type> id;
    vector<coordinate_type> points;
    for(size_t i = 0; i < n; ++i) {
        id.emplace_back(i);
        points.emplace_back(random_point(dim));
    }
    return make_pair(points, id);
}

void print_dataset(const dataset &x) {
    size_t n = x.first.size();
    size_t dim = x.first[0].size();
    for(size_t i = 0; i < n; ++i) {
        print("point %d: %f", x.second[i], x.first[i][0]);
        for(size_t j = 1; j < dim; ++j) {
            print(", %f", x.first[i][j]);
        }
        print("\n");
    }
}

void print_result(const vector<id_type> res) {
    print("%lu points found\n", res.size());
    if(res.size() > 0) {
        print("%d", res[0]);
        for(size_t i = 1; i < res.size(); ++i) {
            print(", %d", res[i]);
        }
        print("\n");
    }
}

void test_BruteForce(size_t n, size_t dim, size_t k, metric_type r) {
    dataset data = make_dataset(n, dim);
    BruteForce geo(data.first, data.second);
    coordinate_type pt = random_point(dim);

    // test K nearest 
    
    vector<id_type> res_K = geo.K_nearest(pt, k);
    
    // test range query
    vector<id_type> res = geo.range_query(pt, r);
}

void test_KDTree(size_t n, size_t dim, size_t k, metric_type r) {
    dataset data = make_dataset(n, dim);
    print_dataset(data);

    // Brete Force solution
    BruteForce bf(data.first, data.second);

    print("building the Kd-Tree...\n");
    Kd_Tree geo(data.first, data.second);
    coordinate_type pt = random_point(dim);

    // test K nearest 
    print("nearest K search...\n");
    vector<id_type> res_K = geo.K_nearest(pt, k);
    vector<id_type> res_K_ = bf.K_nearest(pt, k);
    print_result(res_K);
    print_result(res_K_);
    
    // test range query
    print("range query\n");
    vector<id_type> res = geo.range_query(pt, r);
    vector<id_type> res_ = bf.range_query(pt, r);
    print_result(res);
    print_result(res_);
}

void test_RTree(size_t n, size_t dim, size_t k, metric_type r) {
    dataset data = make_dataset(n, dim);
    print_dataset(data);

    // Brete Force solution
    BruteForce bf(data.first, data.second);

    print("building the R-Tree...\n");
    RTree geo(data.first, data.second);
    coordinate_type pt = random_point(dim);
    
    // test K nearest 
    print("nearest K search...\n");
    vector<id_type> res_K = geo.K_nearest(pt, k);
    vector<id_type> res_K_ = bf.K_nearest(pt, k);
    print_result(res_K);
    print_result(res_K_);
    
    // test range query
    print("range query\n");
    vector<id_type> res = geo.range_query(pt, r);
    vector<id_type> res_ = bf.range_query(pt, r);
    print_result(res);
    print_result(res_);
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);

    // test_BruteForce(100, 3, 10, 1e9);

    // test_KDTree(100, 3, 10, 1e9);

    test_RTree(100, 3, 10, 1e9);

    return 0;
}

