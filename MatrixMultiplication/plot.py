import matplotlib.pyplot as plt
import numpy as np

with open('dat0.txt', 'r') as f:
	data0 = f.read()
data0 = data0.split('\n')
data0.remove('')

with open('dat1.txt','r') as f:
	data1 = f.read()
data1 = data1.split('\n')
data1.remove('')

with open('dat2.txt','r') as f:
	data2 = f.read()
data2 = data2.split('\n')
data2.remove('')

with open('dat3.txt','r') as f:
	data3 = f.read()
data3 = data3.split('\n')
data3.remove('')

with open('dat4.txt','r') as f:
	data4 = f.read()
data4 = data4.split('\n')
data4.remove('')

X = 2.**np.arange(len(data1))
# X = np.arange(len(data1))
Y0 = [float(i) for i in data0]
Y1 = [float(i) for i in data1]
Y2 = [float(i) for i in data2]
Y3 = [float(i) for i in data3]
Y4 = [float(i) for i in data4]

plt.scatter(X, Y0)
plt.scatter(X, Y1)
plt.scatter(X, Y2)
plt.scatter(X, Y3)
plt.scatter(X, Y4)

plt.plot(X, Y0, label='theoretical')
plt.plot(X, Y1, label='original')
plt.plot(X, Y2, label='loop')
plt.plot(X, Y3, label='block')
plt.plot(X, Y4, label='recursion')

plt.xlabel('n')
# x_ticks = ['-o0', '-o1', '-o2', '-o3']
# plt.xticks(range(len(x_ticks)), x_ticks)
plt.xlabel('Compile options')
plt.ylabel('time (in microseconds)')
plt.title('Time taken for matrix multiplication')
plt.legend()
plt.savefig('plot.png')
# plt.show()
