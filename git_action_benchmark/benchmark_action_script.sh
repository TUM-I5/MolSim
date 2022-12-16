#!/bin/bash

for i in {1..16} do
	time (make -j $i > /dev/null 2>&1)
	make clean
done
time (make -j > /dev/null 2>&1)
