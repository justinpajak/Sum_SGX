# Sum_SGX
This program performs an aggregated sum of n floats for n users inside SGX enclave using Graphene

Dependencies:
- Intel SGX: https://github.com/intel/linux-sgx
- Graphene: https://graphene.readthedocs.io/en/latest/quickstart.html#quick-start-with-sgx-support
- OpenSSL: https://www.openssl.org/

<hr/>
1. Compile program to encrypt the floats in data.txt using AES-CBC with NI and store encrypted data in enc.txt

	1. ~/Sum_SGX$ make aes

	- Can uncommment decryption code to test that it works
	- Decryption code will decrypt data in enc.txt and store it in dec.txt
	- Contents of dec.txt and data.txt should be the same

<hr/>
2. Generate a random float for n users and encrypt data to enc.txt file

	1. ~/Sum_SGX$ ./scripts/gen.py -n 200000

<hr/>
3. Run SGX Aggregated Sum Program 

	1. make aggsum

	2. make SGX=1 -f mk_graphene aggsum.manifest.sgx aggsum.token pal_loader

	3. SGX=1 ./pal_loader ./aggsum
	
	- This program reads in encrypted data from enc.txt, decrypts each ciphertext
	  and then perform the aggregated sum over all of the n floats from n users.

	- The total time taken to read in data, decrypt, and calculate sum is outputted to stdout.

<hr/>
4. Clean

	1. make clean
	2. make -f mk_graphene clean
