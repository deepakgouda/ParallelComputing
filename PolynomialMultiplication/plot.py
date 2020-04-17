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

X = (2.**np.arange(1, len(data1)+1))
Y1 = [float(i) for i in data1]
Y2 = [float(i) for i in data2]
Y3 = [float(i) for i in data3]
Y4 = [float(i) for i in data4]
Y5 = [float(i) for i in data5]
Y6 = [float(i) for i in data6]

plt.scatter(X, Y1, marker='.')
plt.scatter(X, Y2, marker='.')
plt.scatter(X, Y3, marker='.')
plt.scatter(X, Y4, marker='.')
plt.scatter(X, Y5, marker='.')
plt.scatter(X, Y6, marker='.')

# plt.plot(X, Y1, label='Naive Sequential')
plt.plot(X, Y2, label='Naive PThread')
plt.plot(X, Y3, label='Karatsuba Sequential')
plt.plot(X, Y4, label='Karatsuba PThread')
plt.plot(X, Y5, label='FFT Sequential')
plt.plot(X, Y6, label='FFT PThread')

plt.xlabel('dimension(n)')
plt.ylabel('time (in seconds)')
plt.title('Time taken for multiplication')
plt.legend()
plt.savefig('plot.png')
plt.show()
