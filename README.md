# Sum_SGX
This program performs an aggregated sum of n floats for n users inside SGX enclave using Graphene

1. Generate a random float for n users.
   
   	1. ./gen.py -n 10000000
	
	- This stores the floats in data.txt for 10000000 users

2. Encrypt the floats in data.txt using AES-CBC with NI and store encrypted data in enc.txt
	
	1. g++ aes.cpp -o aes -lcrypto
	2. ./aes

	- Can uncomment code to test that it works
	- Decrypts data in enc.txt and stores it in dec.txt
	- Contents of dec.txt and data.txt should be the same

3. Run SGX Aggregated Sum Program 

	1. g++ aggsum.cpp -o aggsum -lcrypto

	2. make SGX=1 -f mk_graphene aggsum.manifest.sgx aggsum.token pal_loader

	3. SGX=1 ./pal_loader ./aggsum
	
	- This program reads in encrypted data from enc.txt, decrypts each ciphertext
	  and then perform the aggregated sum over all of the n floats from n users.

	- The time taken to read in data, decrypt, and calculate sum is outputted to stdout.
