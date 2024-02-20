import statistics
import math

def two_populations_variance(pop1, pop2):
    return math.sqrt((statistics.stdev(pop1)/len(pop1)) + (statistics.stdev(pop2)/len(pop2)))

def mean(pop):
    return sum(pop)/len(pop)

def t_score(pop1, pop2):
    pop_variance = two_populations_variance(pop1, pop2)
    avg_pop1 = mean(pop1)
    avg_pop2 = mean(pop2)
    return abs((avg_pop1 - avg_pop2)/pop_variance)

def degrees_of_freedom(pop1, pop2):
    return len(pop1) + len(pop2) - 2

def mean_dif_significant(pop1, pop2):
    mean_1 = mean(pop1)
    mean_2 = mean(pop2)
    mean_diff = abs(mean_1 - mean_2)
