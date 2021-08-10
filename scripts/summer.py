#!/usr/bin/env python3
import os

directory = os.listdir(".")
sum_tot = 0
for d in directory:
	if "data" in d:
		sum_d = 0
		f = open(d, "r");
		for line in f:
			sum_d += float(line)
		sum_tot += sum_d
		f.close()

print(sum_tot)
