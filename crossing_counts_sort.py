import numpy as np

data = np.zeros((60, 3))
for i in range(60):
    data[i, 0] = i + 1
    data[i, 1] = input()
    data[i, 2] = input()

rank = np.argsort(data[:, 1])

data = data[rank]

print("graph \t ratio \t crossings")
for i in range(60):
    print("{}.gr \t {:0.4f} \t {:0.0f}".format(
        int(data[i, 0]),
        data[i, 1],
        data[i, 2]
    ))
print("sum \t {:0.3f}".format(
    sum(data[:, 1]),
))