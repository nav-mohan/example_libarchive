#!/bin/bash

# Simple for loop example
for i in {1..50000}
do
    echo "Iteration number: $i"
   nc localhost 10001 < WeirdAsset.xml -w 0
done
