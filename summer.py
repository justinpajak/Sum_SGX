#!/usr/bin/env python3

f = open("data.txt", "r")

total = 0
for line in f:
	total += float(line)

print(total)
