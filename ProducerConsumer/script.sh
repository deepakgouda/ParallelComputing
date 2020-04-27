echo '' > dat1.txt
echo '' > dat2.txt
echo '' > dat3.txt
echo '' > dat4.txt

g++ -pthread -fopenmp producerConsumer.cpp
n=2800;
for ((i=100; i <= $n; i+=300));
do
	./a.out $((i)) 10 5 50 >> dat1.txt;
done

for ((i=10; i <= $n; i+=10));
do
	./a.out 10 $((i)) 5 50 >> dat2.txt;
done

n=2000;
for ((i=100; i <= $n; i+=100));
do
	./a.out 10 10 $((i)) 5 >> dat3.txt;
done

n=100;
for ((i=10; i <= $n; i+=10));
do
	./a.out 100 100 5 $((i)) >> dat4.txt;
done
