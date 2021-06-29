#!/usr/bin/env python3

f = open("data.txt", "r")

sum = 0
for line in f:
	sum += float(line)

f.close()
print(sum)
