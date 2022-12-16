#!/bin/bash

make clean
for i in {1..16}
do
	echo "Time for -j=$i:"
	time (make asan-MolSim -j $i > /dev/null 2>&1)
	make clean
done

echo "Time for -j inf:"
time (make asan-MolSim -j > /dev/null 2>&1)
make clean
