#!/bin/bash

for i in {1..20}
do
	SGX=1 ../pal_loader ../aggsum 2> /dev/null | tail -1 | cut -d : -f 2 | grep [0-9] | cut -d ' ' -f 2
done
