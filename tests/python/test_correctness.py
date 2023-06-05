import unittest
import timeit

from Generator import *
from Tester import *

# test for higher dimensions
dims = [
    2, 3, 5, 16, 32
]

# test size for correctness test
size_correct = [
    100, 500, 1000, 5000, 10000
]

Kd_Tree_Tests = []
for size in size_correct:
    for dim in dims:
        gen = UniformGenerator(size, dim)
        tester = TestCorrect(gen, "KDTree")
        Kd_Tree_Tests.append(tester)


R_Tree_Tests = []
for size in size_correct:
    for dim in dims:
        gen = UniformGenerator(size, dim)
        tester = TestCorrect(gen, "RTree")
        R_Tree_Tests.append(tester)


class TestKdTreeCorrectness(unittest.TestCase):

    def test_range_qurey(self):
        
        for test in Kd_Tree_Tests:
            print(f"Test the correctness of range query of Kd-Tree with {test.generator.n} points in {test.generator.dim} dimension.")
            ns = dict(tester=test)
            t_query = timeit.Timer("tester.range_query()", globals=ns)
            time_query = sum(t_query.repeat(1, 10)) / 10.0

            print(f"Test finish in {time_query} seconds")

    def test_K_nearest(self):

        for test in Kd_Tree_Tests:
            print(f"Test the correctness of K nearest neighbor of Kd-Tree with {test.generator.n} points in {test.generator.dim} dimensions.")
            ns = dict(tester=test)
            t_query = timeit.Timer("tester.K_nearest()", globals=ns)
            time_query = sum(t_query.repeat(1, 10)) / 10.0

            print(f"Test finish in {time_query} seconds")


class TestRTreeCorrectness(unittest.TestCase):

    def test_range_qurey(self):
        
        for test in R_Tree_Tests:
            print(f"Test the correctness of range query of R-Tree with {test.generator.n} points in {test.generator.dim} dimension.")
            ns = dict(tester=test)
            t_query = timeit.Timer("tester.range_query()", globals=ns)
            time_query = sum(t_query.repeat(1, 10)) / 10.0

            print(f"Test finish in {time_query} seconds")

    def test_K_nearest(self):
        for test in R_Tree_Tests:
            print(f"Test the correctness of K nearest neighbor of R-Tree with {test.generator.n} points in {test.generator.dim} dimensions.")
            ns = dict(tester=test)
            t_query = timeit.Timer("tester.K_nearest()", globals=ns)
            time_query = sum(t_query.repeat(1, 10)) / 10.0

            print(f"Test finish in {time_query} seconds")


if __name__ == '__main__':
    unittest.main()