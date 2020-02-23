echo '' > dat1.txt
echo '' > dat2.txt
echo '' > dat3.txt

mpicc rowSplit.c
for i in {2..14}
do
    mpiexec -n 4 a.out $((2**$i)) >> dat1.txt
done

mpicc colSplit.c
for i in {2..14}
do
    mpiexec -n 4 a.out $((2**$i)) >> dat2.txt
done

mpicc blockSplit.c -lm
for i in {2..14}
do
    mpiexec -n 4 a.out $((2**$i)) >> dat3.txt
done

python plot.py