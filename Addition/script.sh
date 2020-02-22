echo '' > dat1.txt
echo '' > dat2.txt
echo '' > dat3.txt

mpicc additionMethod1.c
for i in {3..22}
do
    mpiexec -n 4 a.out $((2**$i)) >> dat1.txt
done

mpicc additionMethod2.c
for i in {3..22}
do
    mpiexec -n 4 a.out $((2**$i)) >> dat2.txt
done

mpicc additionMethod3.c
for i in {3..22}
do
    mpiexec -n 4 a.out $((2**$i)) >> dat3.txt
done

python plot.py