#!/bin/bash

rm -rf lab3	#usuwanie -r rekurencyjne -f bez pytania

mkdir lab3
mkdir lab3/K1
mkdir lab3/K1/K2
mkdir lab3/K1/K2/K3
mkdir lab3/K4
mkdir lab3/K4/K5
mkdir lab3/K4/K6

for i in `seq 1 9`
do
	echo "test$i"  > lab3/K1/K2/K3/P$i
done

echo "test10" > lab3/K1/K2/P10
echo "test20" > lab3/K4/K5/P20

ln -s ../../K1/K2/P10 lab3/K4/K6/link_P10
ln -s ../K5/P20	lab3/K4/K6/link_P20


ln -s `readlink -e lab3/K1/K2/P10` lab3/K4/K6/link_P10_bez

for i in `seq 1 9`
do
	ln -s ../../K1/K2/K3/P$i lab3/K4/K5/link_$i
done

exit 1
