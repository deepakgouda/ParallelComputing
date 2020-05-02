import matplotlib.pyplot as plt
import numpy as np

with open('dat1.txt','r') as f:
	data1 = f.read()
data1 = data1.split('\n')
data1 = [x for x in data1 if x]

with open('dat2.txt','r') as f:
	data2 = f.read()
data2 = data2.split('\n')
data2 = [x for x in data2 if x]

with open('dat3.txt','r') as f:
	data3 = f.read()
data3 = data3.split('\n')
data3 = [x for x in data3 if x]

with open('dat4.txt','r') as f:
	data4 = f.read()
data4 = data4.split('\n')
data4 = [x for x in data4 if x]

with open('dat5.txt','r') as f:
	data5 = f.read()
data5 = data5.split('\n')
data5 = [x for x in data5 if x]

with open('dat6.txt','r') as f:
	data6 = f.read()
data6 = data6.split('\n')
data6 = [x for x in data6 if x]

X1 = np.arange(1000000, 4000000+1, 100000)
Y1 = [float(i) for i in data1]
Y2 = [float(i) for i in data2]
plt.subplot(2, 1, 1)
plt.scatter(X1, Y1, marker='.')
plt.scatter(X1, Y2, marker='.')
plt.plot(X1, Y1, label='Sequential')
plt.plot(X1, Y2, label='Parallel')
plt.ylabel('Time(in seconds)')
plt.title('|w| vs Time(in seconds)')
plt.legend()

plt.subplot(2, 1, 2)
speedup = [y1/y2 for y1, y2 in zip(Y1, Y2)]
plt.scatter(X1, speedup, marker='.')
plt.xlabel('|w|')
plt.ylabel('Speedup')
plt.plot(X1, speedup)
plt.savefig('plot_1.png')
plt.show()

X2 = np.arange(1, 16+1)
Y3 = [float(i) for i in data3]
Y4 = [float(i) for i in data4]
plt.subplot(2, 1, 1)
plt.scatter(X2, Y3, marker='.')
plt.scatter(X2, Y4, marker='.')
plt.plot(X2, Y3, label='Sequential')
plt.plot(X2, Y4, label='Parallel')
plt.ylabel('Time(in seconds)')
plt.title('k vs Time(in seconds)')
plt.legend()

plt.subplot(2, 1, 2)
speedup = [y3/y4 for y3, y4 in zip(Y3, Y4)]
plt.scatter(X2, speedup, marker='.')
plt.xlabel('k')
plt.ylabel('Speedup')
plt.plot(X2, speedup)
plt.savefig('plot_2.png')
plt.show()

X3 = np.arange(1, 16+1)
Y5 = [float(i) for i in data5]
Y6 = [float(i) for i in data6]
plt.subplot(2, 1, 1)
plt.scatter(X3, Y5, marker='.')
plt.scatter(X3, Y6, marker='.')
plt.plot(X3, Y5, label='Sequential')
plt.plot(X3, Y6, label='Parallel')
plt.ylabel('Time(in seconds)')
plt.title('p vs Time(in seconds)')
plt.legend()

plt.subplot(2, 1, 2)
speedup = [y5/y6 for y5, y6 in zip(Y5, Y6)]
plt.scatter(X3, speedup, marker='.')
plt.xlabel('p')
plt.ylabel('Speedup')
plt.plot(X3, speedup)
plt.savefig('plot_3.png')
plt.show()
