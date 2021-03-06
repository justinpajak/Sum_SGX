#include <iostream>
#include <cstdlib>
#include <string.h>
#include <vector>
#include <chrono>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <unistd.h>
#include <fcntl.h>

using std::vector;

double agg_sum(vector<float> nums);

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext);

void readAndDecrypt(vector<float>& nums);

int main(int argc, char *argv[]) {
	
	int n = 1000000;
	auto start = std::chrono::high_resolution_clock::now();
	vector<float> nums(n);
	
	/* Read in data from d_enc.txt, decrypt and put in nums vector */
	readAndDecrypt(nums);

	/* Compute sum */
	std::cout << std::fixed << "Sum: " << agg_sum(nums) << std::endl;

	/* Compute time */
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "n = " << n << ". Time: " << duration.count() / float(1000000) << " secs." << std::endl;
	return 0;
}

void readAndDecrypt(vector<float>& nums) {
	int enc_data = open("enc.txt", O_RDONLY);
	if (!enc_data) {
		fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
		return;
	}
	unsigned char ciphertext[128];
	unsigned char *key = (unsigned char *)"01234567890123456789012345678901";
    unsigned char *iv = (unsigned char *)"0123456789012345";
	while(read(enc_data, (char*)ciphertext, 128)) {
		unsigned char plaintext[128];
		int plaintext_len = decrypt(ciphertext, 32, key, iv, plaintext);
		plaintext[plaintext_len] = '\0';
		nums.push_back(atof((const char*)plaintext));
	}

}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
    plaintext_len = len;

    EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

double agg_sum(vector<float> nums) {
	double sum;
	for (int i = 0; i < nums.size(); i++) {
		sum += nums[i];
	}
	return sum;
}
