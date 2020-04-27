import matplotlib.pyplot as plt
import numpy as np

def graphPlot(X, Y, xlabel, title, yticks=None):
	plt.figure()
	plt.scatter(X, Y, marker='.')
	plt.plot(X, Y)
	plt.xlabel(xlabel)
	plt.ylabel('Throughput')
	if yticks is not None:
		plt.yticks(yticks)
	plt.title(title)
	plt.savefig('plot_'+xlabel+'.png')

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

X1 = np.arange(100, 2800+1, 300)
Y1 = [float(i) for i in data1]
xlabel = 'numProducers'
title = 'Producers vs Throughput'
graphPlot(X1, Y1, xlabel, title)

X2 = np.arange(10, 10*len(data1)+1, 10)
Y2 = [float(i) for i in data2]
xlabel = 'numConsumers'
title = 'Consumers vs Throughput'
graphPlot(X2, Y2, xlabel, title)

X3 = np.arange(100, 1900+1, 100)
Y3 = [float(i) for i in data3]
xlabel = 'numQueues'
title = 'Queues vs Throughput'
yticks = np.arange(0, 2001, 200)
graphPlot(X3, Y3, xlabel, title, yticks=yticks)

X4 = np.arange(10, 10*len(data4)+1, 10)
Y4 = [float(i) for i in data4]
xlabel = 'queueSize'
title = 'Queue Size vs Throughput'
yticks = np.arange(0, 20001, 2000)
graphPlot(X4, Y4, xlabel, title, yticks=yticks)
