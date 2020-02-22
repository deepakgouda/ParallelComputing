echo '' > dat1.txt

mpicc oddEvenSorting.c
for i in {1..6}
do
    mpiexec -n $((i*10)) a.out >> dat1.txt
done

python plot.py