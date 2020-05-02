echo '' > dat1.txt
echo '' > dat2.txt
echo '' > dat3.txt
echo '' > dat4.txt
echo '' > dat5.txt
echo '' > dat6.txt

g++ -fopenmp -o fsm fsm.cpp
g++ -fopenmp -pthread -o fsmParallel fsmParallel.cpp

# |w| vs speed
n=4000000;
for ((i=1000000; i <= $n; i+=100000));
do
	./fsm $((i)) 5 2 >> dat1.txt;
	./fsmParallel $((i)) 4 2 8 >> dat2.txt;
done

# k vs speed
n=16;
for ((i=1; i <= $n; i++));
do
	./fsm 4000000 $((i)) 2 >> dat3.txt;
	./fsmParallel 4000000 $((i)) 2 8 >> dat4.txt;
done

# p vs speed
n=16;
for ((i=1; i <= $n; i++));
do
	./fsm 4000000 4 2 >> dat5.txt;
	./fsmParallel 4000000 4 2 $((i)) >> dat6.txt;
done

