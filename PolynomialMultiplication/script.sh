echo '' > dat1.txt
echo '' > dat2.txt
# echo '' > dat3.txt

g++ -pthread -fopenmp naive.cpp
for i in {1..100};
do
    ./a.out $((100*i)) 5 >> dat1.txt;
done

g++ -pthread -fopenmp karatsuba.cpp
for i in {1..100};
do
    ./a.out $((100*i)) >> dat2.txt;
done

python plot.py