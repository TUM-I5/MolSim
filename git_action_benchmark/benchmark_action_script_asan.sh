#!/bin/bash

for i in {1..16} do
	time (make asan-MolSim -j $i > /dev/null 2>&1)
	make clean
done
time (make asan-MolSim -j > /dev/null 2>&1)
