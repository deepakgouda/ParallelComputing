echo '' > dat1.txt
echo '' > dat2.txt
echo '' > dat3.txt
echo '' > dat4.txt
echo '' > dat5.txt
echo '' > dat6.txt

n=18;
g++ -fopenmp naive.cpp
for ((i=1; i <= $n; i++));
do
	./a.out $((2**$i)) >> dat1.txt;
done

g++ -pthread -fopenmp naiveParallel.cpp
for ((i=1; i <= $n; i++));
do
	./a.out $((2**$i)) 8 >> dat2.txt;
done

g++ -pthread -fopenmp karatsuba.cpp
for ((i=1; i <= $n; i++));
do
	./a.out $((2**$i)) >> dat3.txt;
done

g++ -pthread -fopenmp karatsubaParallel.cpp
for ((i=1; i <= $n; i++));
do
	./a.out $((2**$i)) >> dat4.txt;
done

g++ -std=c++17 -fopenmp fft.cpp
for ((i=1; i <= $n; i++));
do
	./a.out $((2**$i)) >> dat5.txt;
done

g++ -std=c++17 -pthread -fopenmp fftParallel.cpp
for ((i=1; i <= $n; i++));
do
	./a.out $((2**$i)) >> dat6.txt;
done

python plot.py