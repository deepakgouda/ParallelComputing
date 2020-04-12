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

# with open('dat3.txt','r') as f:
# 	data3 = f.read()
# data3 = data3.split('\n')
# data3 = [x for x in data3 if x]

X = (np.arange(1, len(data1)+1))*100
Y1 = [float(i) for i in data1]
Y2 = [float(i) for i in data2]
# Y3 = [float(i) for i in data3]

plt.scatter(X, Y1, marker='.')
plt.scatter(X, Y2, marker='.')
# plt.scatter(X, Y3)

plt.plot(X, Y1, label='Naive')
plt.plot(X, Y2, label='Karatsuba')
# plt.plot(X, Y3, label='Block-wise')

plt.xlabel('dimension(n)')
plt.ylabel('time (in seconds)')
plt.title('Time taken for multiplication')
plt.legend()
plt.savefig('plot.png')
plt.show()
