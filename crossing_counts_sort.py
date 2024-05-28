import numpy as np

name1 = input()
name2 = input()

ratios = np.zeros((60, 3))
for i in range(60):
    ratios[i, 0] = i + 1
    ratios[i, 1] = input()
    ratios[i, 2] = input()

best_ratios = np.min(ratios[:, 1:], axis=1)
rank = np.argsort(best_ratios)

ratios = ratios[rank]

print("graph \t {} \t {}".format(name1, name2))
for i in range(60):
    print("{}.gr \t {:0.4f} \t {:0.4f}".format(
        int(ratios[i, 0]),
        ratios[i, 1],
        ratios[i, 2]
    ))
print("sum \t {:0.3f} \t {:0.3f}".format(
    sum(ratios[:, 1]),
    sum(ratios[:, 2])
))