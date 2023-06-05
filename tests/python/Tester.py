
# append import path
import sys, os
sys.path.append('/home/lolicon/workspace/NSD/Geometric-Search-Engine/python')

# import Geometric module
import Geometric


class TestCorrect(object):

    def __init__(self, generator, testant="KdTree"):

        self.generator = generator
        point, ID = self.generator.make_dataset()
        
        self.ID = ID
        self.point = point

        # testant
        constructor = getattr(Geometric, testant)
        self.target = constructor(point, ID)

        # true answer for checking
        self.gold = Geometric.BruteForce(point, ID)
    
    def K_nearest(self):
        
        tests = self.generator.make_K_nearest_queries()

        for testcase in tests:
            K = testcase.K
            center = testcase.center
            res_gold = self.gold.K_nearest(center, K)
            res_test = self.target.K_nearest(center, K)

            if res_gold != res_test:
                print(f"Error when querying {K} nearest at {center}")
                return False

        return True

    def range_query(self):
        
        tests = self.generator.make_range_queries()

        for testcase in tests:
            center = testcase.center
            radius = testcase.radius
            res_gold = self.gold.range_query(center, radius)
            res_test = self.target.range_query(center, radius)

            if res_gold != res_test:
                print(f"Error when range querying at {center} with raduis {radius}")
                return False

        return True


class TestPerformance(object):

    def __init__(self, generator, testant="KdTree"):

        self.generator = generator

        point, ID = self.generator.make_dataset()

        self.ID = ID
        self.point = point

        # testant
        constructor = getattr(Geometric, testant)
        self.target = constructor(point, ID)

    def K_nearest(self):
        tests = self.generator.make_K_nearest_queries()

        for testcase in tests:
            K = testcase.K
            center = testcase.center
            res_test = self.target.K_nearest(center, K)

        return True

    def range_query(self):
        
        tests = self.generator.make_range_queries()

        for testcase in tests:
            center = testcase.center
            radius = testcase.radius
            res_test = self.target.range_query(center, radius)

        return True

