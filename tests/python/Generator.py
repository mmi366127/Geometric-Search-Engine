
from collections import namedtuple
from abc import abstractmethod
import numpy as np

class Generator(object):
    def __init__(self, n, dim, K_query_count=100, R_query_count=100, range=(-1e9, 1e9)):
        self.n = n
        self.dim = dim
        self.range = range
        self.K_query_count = K_query_count
        self.R_query_count = R_query_count

    @abstractmethod
    def make_dataset(self):
        pass
        
    @abstractmethod
    def make_K_nearest_queries(self):
        pass
    
    @abstractmethod
    def make_range_queries(self):
        pass


RQuery = namedtuple("RQuery", "center, radius")
KQuery = namedtuple("KQuery", "center, K")

class UniformGenerator(Generator):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, *kwargs)

    def make_dataset(self):
        min_, max_ = self.range
        ID = np.arange(0, self.n, 1)
        point = np.random.uniform(min_, max_, size=(self.n, self.dim))
        return point, ID
        
    def make_K_nearest_queries(self):
        ret = []
        for i in range(self.K_query_count):
            min_, max_ = self.range
            point = np.random.uniform(min_, max_, size=(self.dim))
            ret.append(KQuery(center=point, K=min((i+1)*10, self.n)))
        return ret

    def make_range_queries(self):
        ret = []
        for i in range(self.R_query_count):
            min_, max_ = self.range
            point = np.random.uniform(min_, max_, size=(self.dim))
            radius = (max_ - min_) / 3.0
            ret.append(RQuery(center=point, radius=radius))
        return ret
    

# generators that generator dataset
generators = {
    "uniform": UniformGenerator,
    # "gaussian": make_dataset_gaussian,
    # "oneline": make_dataset_oneline,
    # "fourGroup": make_dataset_oneline,
}

