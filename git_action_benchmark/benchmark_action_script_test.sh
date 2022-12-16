#!/bin/bash

make clean
for i in {1..16}
do
	echo "Time for -j=$i:"
	time (make runtest -j $i CXX_FLAGS+="-std=c++20" > /dev/null 2>&1)
	make clean
done

echo "Time for -j inf:"
time (make runtest -j CXX_FLAGS+="-std=c++20"> /dev/null 2>&1)
make clean
