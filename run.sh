#!/bin/bash

for i in {1..20}
do
	SGX=1 ./pal_loader ./aggsum
done
