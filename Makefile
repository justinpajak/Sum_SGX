all: aes aggsum

aes: aes.cpp
	g++ aes.cpp -o aes -lcrypto

aggsum: aggsum.cpp
	g++ aggsum.cpp -o aggsum -lcrypto -lpthread

clean:
	rm aes aggsum
