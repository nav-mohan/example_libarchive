#!/bin/bash

# Simple for loop example
while true
do
	for i in {1..20000}
	do
	    echo "Iteration number: $i"
	    nc localhost 10001 < Prerec_started.xml -w 0
	done
	sleep 300
done

