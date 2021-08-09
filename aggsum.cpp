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
#include <pthread.h>

using std::vector;

double agg_sum();

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext);

void readAndDecrypt();

size_t NTHREADS = 1;
size_t n = 1000000;
vector<float> nums;
int enc_data;

// Lock
pthread_mutex_t lock;

int main(int argc, char *argv[]) {
	
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch(argv[i][1]) {
				case 'n':
					n = atoi(argv[++i]);
					break;
				case 't':
					NTHREADS = atoi(argv[++i]);
					break;
				default:
					return EXIT_FAILURE;
			}
		} else {
			return EXIT_FAILURE;
		}
	}
	fprintf(stdout, "n: %lu\n", n);
	fprintf(stdout, "t: %lu\n", NTHREADS);
	pthread_mutex_init(&lock, NULL);

	auto start_tot = std::chrono::high_resolution_clock::now();
	
	/* Read in data from d_enc.txt, decrypt and put in nums vector */
	enc_data = open("enc.txt", O_RDONLY);
	readAndDecrypt();

	/* Compute sum */
	double sum = agg_sum();
	std::cout << std::fixed << "Sum: " << sum << std::endl;

	/*for (size_t i = 0; i < nums.size(); i++) {
		std::cout << nums[i] << std::endl;
	}*/

	/* Compute time */
	auto stop_tot = std::chrono::high_resolution_clock::now();
	auto duration_tot = std::chrono::duration_cast<std::chrono::microseconds>(stop_tot - start_tot);
	std::cout << "time: " << duration_tot.count() << std::endl;
	return 0;
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

typedef struct {
	size_t start;
	size_t stop;
} SumArgs;

void *thread_read(void * args) {
	SumArgs *a = (SumArgs*)args;

	unsigned char ciphertext[128];
	unsigned char *key = (unsigned char *)"01234567890123456789012345678901";
    unsigned char *iv = (unsigned char *)"0123456789012345";
	pthread_mutex_lock(&lock);
	lseek(enc_data, a->start * 128, SEEK_SET);
	while(read(enc_data, (char*)ciphertext, 128) && a->start <= a->stop) {
		unsigned char plaintext[128];
		int plaintext_len = decrypt(ciphertext, 32, key, iv, plaintext);
		plaintext[plaintext_len] = '\0';
		nums.push_back(atof((const char*)plaintext));
		a->start++;
	}
	pthread_mutex_unlock(&lock);
	return NULL;
}

void readAndDecrypt() {
	pthread_t threads[NTHREADS];
	SumArgs args[NTHREADS];
	
	int e = n / NTHREADS;
	int last = e + n % NTHREADS;

	for (size_t i = 0; i < NTHREADS; i++) {
		args[i].start = i * e;
		args[i].stop = (i * e) + last - 1;
		pthread_create(&threads[i], NULL, thread_read, (void*)&args[i]);
	}

	int *result;
	for (size_t i = 0; i < NTHREADS; i++) {
		pthread_join(threads[i], (void**)&result);
	}
}

void *thread_sum(void *arg) {
	SumArgs *a = (SumArgs*)arg;
	double local_sum = 0;

	for (size_t i = a->start; i <= a->stop; i++) {
		local_sum += nums[i];
	}
	double *result = (double*)malloc(sizeof(double));
	*result = local_sum;
	return (void*)result;
}

double agg_sum() {
	pthread_t threads[NTHREADS];
	SumArgs args[NTHREADS];

	int e = n / NTHREADS;
	int last = e + n % NTHREADS;

	for (size_t i = 0; i < NTHREADS; i++) {
		args[i].start = i * e;
		args[i].stop = (i * e) + last - 1;
		pthread_create(&threads[i], NULL, thread_sum, (void*)&args[i]);
	}

	double sum_total = 0;
	double *t_sum;
	for (size_t i = 0; i < NTHREADS; i++) {
		pthread_join(threads[i], (void**)&t_sum);
		sum_total += *t_sum;
		free(t_sum);
	}

	return sum_total;
}
