#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "distance.h"
#include "BruteForce.h"
#include "Kd_Tree.h"
#include "RTree.h"

namespace py = pybind11;

using namespace geometric;

PYBIND11_MODULE(Geometric, m) {
    m.doc() = "Library for geometric search."; 
    
    // BruteForce method for testing
    py::class_<BruteForce>(m, "BruteForce")
        .def(py::init<std::vector<coordinate_type>, std::vector<id_type>>()) 
        .def("K_nearest", &BruteForce::K_nearest)
        .def("range_query", &BruteForce::range_query)
        .def_property_readonly("ndim", &BruteForce::ndim);

    // K-d tree
    py::class_<Kd_Tree>(m, "KDTree")
        .def(py::init<std::vector<coordinate_type>, std::vector<id_type>>())
        .def("K_nearest", &Kd_Tree::K_nearest)
        .def("range_query", &Kd_Tree::range_query)
        .def_property_readonly("ndim", &Kd_Tree::ndim);

    // R-Tree
    py::class_<RTree>(m, "RTree")
        .def(py::init<std::vector<coordinate_type>, std::vector<id_type>>())
        .def("K_nearest", &RTree::K_nearest)
        .def("range_query", &RTree::range_query)
        .def_property_readonly("ndim", &RTree::ndim);
    
}



