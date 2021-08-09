#!/usr/bin/env python3

import sys, random, os

def main():
	
	n = 1000000 # number of users
	t = 1       # of threads

	# Parse command line arguments */
	arguments = sys.argv[1:]
	while arguments and arguments[0].startswith("-"):
		arg = arguments.pop(0)
		if arg == '-n':
			n = int(arguments.pop(0))
		elif arg == '-t':
			t = int(arguments.pop(0))
		else:
			sys.exit(1)

	print(n)
	print(t)

	# Segment data into different files for multithreading
	e = n // t
	last = e + n % t
	for i in range(t):
		f = open(f"data{i}.txt", "w")
		for j in range(i * e, (i * e) + last):
			f.write("{0:.13f}".format(random.uniform(10, 99)))
			f.write("\n")
		f.close()

		# Run encryption program
		command = f"./aes -f data{i}.txt -n {i}"
		os.system(command)
	
if __name__ == '__main__':
	main()
