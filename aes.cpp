#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <string.h>
#include <iostream>

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char * key,
            unsigned char *iv, unsigned char *ciphertext);

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext);

int main(int argc, char *argv[]) {

	/* 256 bit key */
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

    /* 128 bit IV */
    unsigned char *iv = (unsigned char *)"0123456789012345";

    /* Plaintext and Ciphertext declaration */
    unsigned char *plaintext;
    unsigned char ciphertext[128];
    int ciphertext_len, decryptedtext_len;

	char *filename = NULL;
	int num = -1;

	/* Parse Command Line Arguments */
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
				case 'f':
					filename = argv[++i];
					break;
				case 'n':
					num = atoi(argv[++i]);
					break;
				default:
					return EXIT_FAILURE;
			}
		} else {
			return EXIT_FAILURE;
		}
	}
	
	if (num == -1 || !filename) {
		return EXIT_FAILURE;
	}

    /* Loop through data.txt file and encrypt each float */
    FILE* data = fopen((const char*)filename, "r+");
    if (!data) {
        fprintf(stderr, "Unable to open file data: %s\n", strerror(errno));
        return 1;
    }

	std::string e_string = "enc";
	e_string += std::to_string(num);
	e_string += ".txt";
	const char *e_cstring = e_string.c_str();
	std::cout << e_cstring << std::endl;

    FILE* enc_data = fopen(e_cstring, "w+");
    if (!enc_data) {
        fprintf(stderr, "Unable to open file enc: %s\n", strerror(errno));
        return 1;
    }
    char buffer[BUFSIZ];
    while (fgets(buffer, BUFSIZ, data)) {
        buffer[16] = '\0';
        plaintext = (unsigned char*)buffer;
        ciphertext_len = encrypt(plaintext, strlen((char*)plaintext), key, iv, ciphertext);
        fwrite((const char*)ciphertext, 128, 1, enc_data);
    };


    // test protocol: Decrypt all of enc.txt to dec.txt 
    /*FILE* dec_data = fopen("dec.txt", "w");
    if (!dec_data) {
        fprintf(stderr, "Unable to open file: %s\n", strerror(errno));
        return 1;
    }
    fseek(enc_data, 0L, SEEK_SET);
    while(fread((char*)ciphertext, 128, 1, enc_data)) {
        unsigned char decryptedtext[128];
        decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv, decryptedtext);
        decryptedtext[decryptedtext_len] = '\0';
        fputs((const char*)decryptedtext, dec_data);
        fputs("\n", dec_data);
    }

    fclose(dec_data);*/

    fclose(data);
    fclose(enc_data);

	return 0;
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len);
    ciphertext_len = len;

	EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
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

