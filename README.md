# Geometric Search Engine

A project that implement R-tree, kd-tree, M-tree for geometric searching.

## Basic Information
GitHub repository: [Geometric search engine](https://github.com/mmi366127/Geometric-Search-Engine)

Geometric search is important for implmenting map and geometry system. For more efficient query, there are many data structures for different circumstances, including R-tree, kd-tree and M-tree. Moreover, these data structures can also be used to do machine learning tasks in KNN(K-nearest-neighbors) algorithm. Therefore, in this project, I am going to implement an API for geometric search for arbitrary dimension data. 

## Problem to Solve

Create a library that provide R-tree, kd-tree, and M-tree to do query on nearest-n-points, and range-query.(Provide choices for data structure, not combining them together.) This project assumes using Euclidean distance for distance metric(could add more distance metric if possible.)

## Prospective Users

For users that aims to build their geometric information system, the API can provide useful tools to do the searching. And for machine learning users, the API can provide high dimension support for their usage.

## System Architeure

- Using ``C++`` to implement the algorithm.
- Using ``pybind11`` to make API in python.


## API Description

- Using ``vector<double>`` for coordinate and ``int32_t`` for ID to represent the data points in C++.

````c++
// build data structure with coordinates of the data points and their IDs.
Tree build(vector<vector<double>> points, vector<int32_t> IDs);


// return the ID of the data points
vector<int> K_nearest(vector<double> coordinate);

vector<int> range_query(vector<double> center, double distance);

````
- Use ``numpy`` for data points in python and use pybind to pass it to C++.

````python
# build data structure with coordinates of the data points and their IDs.
def build(points: numpy.ndarray, IDs: numpy.ndarray) -> Tree:
    pass


# return the ID of the data points
def K_nearest(coordinate: numpy.ndarray) -> [int]:
    pass

def range_query(center: numpy.ndarray, distance: float) -> [int]:
    pass

````

## Engineering Infrastructure

- Use ``CMake`` to build the binaries.
- Use ``git`` for version control.
- Use ``pytest`` for testing the performance and correctness.

## Future Work
- Add ``insert`` function.
- Refactor the code architecture.
- Optimize performance and add more options.


## References

- https://en.wikipedia.org/wiki/Category:Geometric_data_structures
- https://en.wikipedia.org/wiki/R-tree
- https://en.wikipedia.org/wiki/K-d_tree
- https://en.wikipedia.org/wiki/M-tree


