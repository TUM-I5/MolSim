#!/bin/bash

for i in {1..16} do
	time (make runtest -j $i CXX_FLAGS +="-std=c++20" > /dev/null 2>&1)
	make clean
done
time (make runtest -j CXX_FLAGS +="-std=c++20"> /dev/null 2>&1)
