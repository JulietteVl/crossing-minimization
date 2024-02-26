import sys
import argparse
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

def read_graph(filename: str):
    graph = None
    with open(filename, "r") as f:
        edges = []
        for line in f:
            if line[0] == "p":
                n0, n1 = map(int, line.split(" ")[2:4])
            elif line[0] == "c":
                pass
            else:
                edges.append(list(map(int, line.split(" "))))
    return n0, n1, np.array(edges)

def count_neighbours(edges, i, n0=0):
    if i == 0:
        return np.bincount(edges[:, i])[1:]
    else:
        return np.bincount(edges[:, i])[n0 + 1:]



parser = argparse.ArgumentParser(description='Print information on the degree of the vertices of a graph.')
parser.add_argument("-p", "--path", help="use with range for filenames of name 'path/i.gr' with i in range")
parser.add_argument(
    "-r",
    "--range",
    help="use with path for filenames of name 'path/i.gr' with i in range",
    metavar=("start", "end"),
    type=int,
    nargs=2,
)
parser.add_argument("-f", "--filename")

layer = ["fixed", "free"]

if __name__ == '__main__':
    argument = parser.parse_args()
    if argument.range:
        start, end = argument.range
        print(start, end)
    if argument.path:
        path = argument.path
    if argument.filename:
        filename = argument.filename

    if argument.path and argument.range:
        print("Minimum and maximum degrees:")
        for j in range (start, end):
            n0, n1, edges = read_graph(path + "/{}.gr".format(j))
            for i in range(2):
                c = count_neighbours(edges, i, n0=n0)
                print("{} {}:\t {} {}".format(j, layer[i], min(c), max(c)))
            print()

    if argument.filename:
        n0, n1, edges = read_graph(argument.filename)

        fig, axs = plt.subplots(1, 2, figsize=(10, 5))
        axs[0].set_title("Fixed layer")
        axs[1].set_title("Free layer")
        plt.suptitle("Degree distribution")
        for i in range(2):
            c = count_neighbours(edges, i, n0=n0)
            print(c)
            sns.histplot(
                data=c,
                ax = axs[i],
            )
        plt.show()