# Sum_SGX
Implemenation of an aggregated sum of n floats for n users inside SGX enclave using Graphene - Fully Multithreaded Version

Dependencies:
- Intel SGX: https://github.com/intel/linux-sgx
- Graphene: https://graphene.readthedocs.io/en/latest/quickstart.html#quick-start-with-sgx-support
- OpenSSL: https://www.openssl.org/

<hr/>
1. Compile program to encrypt the floats in data.txt using AES-CBC with NI and store encrypted data

	1. ~/Sum_SGX$ make aes

	- Can uncommment decryption code to test that it works
	- Decryption code will decrypt data in enc.txt and store it in dec.txt
	- Contents of dec.txt and data.txt should be the same

<hr/>
2. Generate a random float for n users, segment data, encrypt data in t different files, where t is the number of threads.

	1. ~/Sum_SGX$ ./scripts/gen.py -n 1000000 -t 2

	- Script also updates aggsum.manifest.template for t encrypted files

<hr/>
3. Run SGX Aggregated Sum Program - Multithreaded

	1. ~/Sum_SGX$ make aggsum

	2. ~/Sum_SGX$ make SGX=1 -f mk_graphene aggsum.manifest.sgx aggsum.token pal_loader

	3. ~/Sum_SGX$ SGX=1 ./pal_loader ./aggsum -n 1000000 -t 2
	
	- This program reads in encrypted data from enc0.txt, enc1.txt, ... (depends on # of threads specified).

	- Then performs the aggregated sum over all of the n floats from n users.

	- The total time taken to read in data, decrypt, and calculate sum is outputted to stdout.

<hr/>
4. Clean

	1. ~/Sum_SGX$ make clean
	2. ~/Sum_SGX$ make -f mk_graphene clean
	3. ~/Sum_SGX$ ./scripts/cleaner.sh
