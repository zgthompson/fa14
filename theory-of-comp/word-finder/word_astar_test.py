from word_astar import WordAStar
import sys
import random
import itertools
from matplotlib import pyplot

def createPlot(results):
    pyplot.title("Average transformability after removal of unconnected subgraphs")
    pyplot.xlabel("Word length")
    pyplot.ylabel("Average transformability coefficient")

    for num, result in enumerate(results):
        xs = [word_len for word_len, transform in result]
        ys = [transform for word_len, transform in result]
        pyplot.plot(xs, ys, label="Wordlist w/o %dRN words" % num, marker='o', linewidth=2)
    pyplot.legend()

def main():

    WORD_LEN_MIN = 2
    WORD_LEN_MAX = 9
    SAMPLE_TOTAL = 20

    lens_with_transform = []
    for word_len in range(WORD_LEN_MIN, WORD_LEN_MAX):
        pool = [word for word in WordAStar.finder.dictionary if len(word) == word_len]
        n = len(pool)
        transform_average = 0.0
        for sample in range(SAMPLE_TOTAL):
            indices = sorted(random.sample(xrange(n), 2))
            start, end = tuple(pool[i] for i in indices)
            path = WordAStar(start, end).search()

            steps_min = reduce(lambda x, y: x + (y[0] != y[1]), zip(start, end), 0)

            if path:
                transform = steps_min / (len(path) - 1)
            else:
                transform = 0

            transform_average += transform

        lens_with_transform.append((word_len, transform_average / SAMPLE_TOTAL))
    createPlot([lens_with_transform])


#main()
