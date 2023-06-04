

# append import path
import sys, os
sys.path.append('/home/lolicon/workspace/NSD/Geometric-Search-Engine/python')

# import Geometric module
import Geometric


import unittest
import random
import timeit
import os

import numpy as np


def make_dataset(n, dim):
    IDs = np.arange(0, n, 1)
    pts = np.random.uniform(-1e9, 1e9, size=(n, dim))
    return pts, IDs


class TestBruteForce(unittest.TestCase):

    def test_init(self):
        pts, IDs = make_dataset(100, 3)
        geo = Geometric.BruteForce(pts, IDs)
    
    def test_range_query(self):
        pts, IDs = make_dataset(100, 3)
        geo = Geometric.BruteForce(pts, IDs)
        center = np.random.uniform(-1e9, 1e9, size=(geo.ndim))
        res = geo.range_query(center, 1e9)
        print(res)

    def test_K_nearest(self):
        pts, IDs = make_dataset(100, 3)
        geo = Geometric.BruteForce(pts, IDs)
        center = np.random.uniform(-1e9, 1e9, size=(geo.ndim))
        res = geo.K_nearest(center, 50)
        print(res)


class TestKdTree(unittest.TestCase):

    def test_init(self):
        pts, IDs = make_dataset(1000000, 3)
        geo = Geometric.KDTree(pts, IDs)

    def test_range_qurey(self):
        pts, IDs = make_dataset(1000000, 3)
        geo = Geometric.KDTree(pts, IDs)

        center = np.random.uniform(-1e9, 1e9, size=(geo.ndim))

        res = geo.range_query(center, 1e9)

        print(res)

    def test_K_nearest(self):
        pts, IDs = make_dataset(1000000, 3)
        geo = Geometric.KDTree(pts, IDs)

        center = np.random.uniform(-1e9, 1e9, size=(geo.ndim))

        res = geo.K_nearest(center, 500)

        print(res)


class TestRtree(unittest.TestCase):

    def test_init(self):
        pts, IDs = make_dataset(1000, 3)
        geo = Geometric.RTree(pts, IDs)

    def test_range_query(self):
        pass

    def test_K_nearest(self):
        pass


if __name__ == '__main__':


    x = TestRtree()

    x.test_init()
    x.test_range_qurey()
    x.test_K_nearest()