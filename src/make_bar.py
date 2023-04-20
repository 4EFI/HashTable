import matplotlib.pyplot as plt
import numpy as np

fig = plt.figure(figsize = (6, 4))
ax = fig.add_subplot()

y = []
data = []

with open( "res/output_const.csv", "r" ) as f:
    data = list(map(int, f.readline().split()))

for i in range(len(data)):
    for j in range(data[i]):
        y.append(i)
ax.hist( y, 100, range = (0,1000) )

fig.savefig('res/const_graph.png')
plt.show()