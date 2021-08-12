#!/bin/bash

for i in {1..10}
do
	SGX=1 ./pal_loader ./aggsum -n 9000000 -t 8
done
