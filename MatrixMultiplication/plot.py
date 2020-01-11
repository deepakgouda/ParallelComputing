import matplotlib.pyplot as plt
import numpy as np
with open('dat1.txt','r') as f:
	data1 = f.read()
data1 = data1.split('\n')
data1.remove('')
# X = 10.**np.arange(len(data1))

with open('dat2.txt','r') as f:
	data2 = f.read()
data2 = data2.split('\n')
data2.remove('')

with open('dat3.txt','r') as f:
	data3 = f.read()
data3 = data3.split('\n')
data3.remove('')

X = 2.**np.arange(1, len(data1)+1)
Y1 = [float(i) for i in data1]
Y2 = [float(i) for i in data2]
Y3 = [float(i) for i in data3]
plt.scatter(X, Y1)
plt.scatter(X, Y2)
plt.scatter(X, Y3)
plt.plot(X, Y1, label='original')
plt.plot(X, Y2, label='opt1')
plt.plot(X, Y3, label='opt2')
plt.xlabel('n')
plt.ylabel('time (in microseconds)')
plt.title('Time taken for matrix multiplication')
plt.legend()
# plt.save('plot1.png')
plt.show()