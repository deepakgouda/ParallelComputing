import matplotlib.pyplot as plt
import numpy as np

with open('dat1.txt','r') as f:
	data1 = f.read()
data1 = data1.split('\n')
data1 = [x for x in data1 if x]

X = (2.**np.arange(len(data1)))*8
Y1 = [float(i) for i in data1]

plt.scatter(X, Y1)

plt.plot(X, Y1, label='Method 1')

plt.xlabel('n')
plt.xlabel('dimension(n)')
plt.ylabel('time (in seconds)')
plt.title('Time taken for Sorting')
plt.legend()
plt.savefig('final_plot.png')
