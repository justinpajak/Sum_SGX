#!/usr/bin/env python3

from Crypto.Cipher import AES
import random
import string

def main():
		
	# Encrypt data from data.txt using AES-GCM and put in d_enc.txt

	f = open("data.txt", "r")
	e = open("d_enc.txt", "w")

	key = "this is your key" # 16 bytes 
	iv = "".join(random.choice(string.ascii_letters + string.digits) for _ in range(16))
	cipher = AES.new(key, AES.MODE_CBC, iv)
	
	for line in f:
		line = line.rstrip()
		line = str(cipher.encrypt(line)).replace("'", "")[1:]
		e.write(line)
		e.write("\n")

	f.close()
	e.close()

if __name__ == '__main__':
	main()
