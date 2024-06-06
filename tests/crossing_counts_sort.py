import numpy as np

time = np.zeros(60)
for i in range(60):
    time[i] = input()

data = np.zeros((60, 3))
for i in range(60):
    data[i, 0] = i + 1
    data[i, 1] = input()
    data[i, 2] = input()

rank = np.argsort(data[:, 1])

data = data[rank]
print(time[0])
print("graph \t ratio \t time")
for i in range(60):
    print("{}.gr \t {:0.4f} \t {:0.2f}s".format(
        int(data[i, 0]),
        data[i, 1],
        time[i],
    ))
print("sum \t {:0.3f}\t {:0.2f}".format(
    sum(data[:, 1]), sum(time)
))