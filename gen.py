#!/usr/bin/env python3

import sys, random

def main():
	
	n = 1000000 # number of users

	# Parse command line arguments */
	arguments = sys.argv[1:]
	while arguments and arguments[0].startswith("-n"):
		arg = arguments.pop(0)
		if arg == '-n':
			n = int(arguments.pop(0))
		else:
			sys.exit(1)

	# Write data to file 
	f = open("data.txt", "w")
	for _ in range(n):
		f.write("{0:.11f}".format(random.uniform(1000, 9999)))
		f.write("\n")
	f.close()

if __name__ == '__main__':
	main()
