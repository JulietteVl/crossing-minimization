import sys
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

def read_graph(filename: str):
    graph = None
    with open(filename, "r") as f:
        a = 0
        b = 0
        edges = []
        for line in f:
            if line[0] == "p":
                a, b = map(int, line.split(" ")[2:4])
            elif line[0] == "c":
                pass
            else:
                edges.append(list(map(int, line.split(" "))))
                offset = np.bincount(edges[:, 0])
    return np.array(edges), offset


edges, _ = read_graph(sys.argv[1])

# Plot
layer = ["fixed", "free"]

fig, axs = plt.subplots(1, 2, figsize=(10, 5))
axs[0].set_title("Fixed layer")
axs[1].set_title("Free layer")
plt.suptitle("Degree repartition")

for i in range(2):
    count_neighbours = np.bincount(edges[:, i])
    count_neighbours = count_neighbours[count_neighbours > 0]
    sns.histplot(
        data=count_neighbours,
        ax = axs[i],
    )
    print("Minimum and maximum degrees in the {} layer:{}, {}".format(layer[i], min(count_neighbours), max(count_neighbours)))

plt.show()