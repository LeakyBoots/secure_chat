/* examples of using various crypto utilities from openssl. */
#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

/* demonstrates AES in counter mode */
void ctr_example()
{
	unsigned char key[32];
	size_t i;
	/* setup dummy (non-random) key and IV */
	for (i = 0; i < 32; i++) key[i] = i;
	unsigned char iv[16];
	for (i = 0; i < 16; i++) iv[i] = i;
	/* NOTE: in general you need t compute the sizes of these
	 * buffers.  512 is an arbitrary value larger than what we
	 * will need for our short message. */
	unsigned char ct[512];
	unsigned char pt[512];
	/* so you can see which bytes were written: */
	memset(ct,0,512);
	memset(pt,0,512);
	char* message = "this is a test message :D";
	size_t len = strlen(message);
	/* encrypt: */
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	if (1!=EVP_EncryptInit_ex(ctx,EVP_aes_256_ctr(),0,key,iv))
		ERR_print_errors_fp(stderr);
	int nWritten; /* stores number of written bytes (size of ciphertext) */
	if (1!=EVP_EncryptUpdate(ctx,ct,&nWritten,(unsigned char*)message,len))
		ERR_print_errors_fp(stderr);
	EVP_CIPHER_CTX_free(ctx);
	size_t ctlen = nWritten;
	printf("ciphertext of length %i:\n",nWritten);
	for (i = 0; i < ctlen; i++) {
		printf("%02x",ct[i]);
	}
	printf("\n");
	/* now decrypt.  NOTE: in counter mode, encryption and decryption are
	 * actually identical, so doing the above again would work.  Also
	 * note that it is crucial to make sure IVs are not reused.  */
	/* wipe out plaintext to be sure it worked: */
	memset(pt,0,512);
	ctx = EVP_CIPHER_CTX_new();
	if (1!=EVP_DecryptInit_ex(ctx,EVP_aes_256_ctr(),0,key,iv))
		ERR_print_errors_fp(stderr);
	#if 0
	if (1!=EVP_DecryptUpdate(ctx,pt,&nWritten,ct,ctlen))
		ERR_print_errors_fp(stderr);
	printf("decrypted %i bytes:\n%s\n",nWritten,pt);
	#endif
	for (size_t i = 0; i < len; i++) {
		if (1!=EVP_DecryptUpdate(ctx,pt+i,&nWritten,ct+i,1))
			ERR_print_errors_fp(stderr);
	}
	printf("decrypted %lu bytes:\n%s\n",len,pt);
	EVP_CIPHER_CTX_free(ctx);
	/* NOTE: counter mode will preserve the length (although the person
	 * decrypting needs to know the IV) */
}

static void use_encrypt(char* msg, unsigned char* ciph, unsigned char* k, unsigned char* iv)
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	if (1!=EVP_EncryptInit_ex(ctx,EVP_aes_256_ctr(),0,k,iv))
		ERR_print_errors_fp(stderr);
	int nWritten;
    size_t len = strlen(msg);
	if (1!=EVP_EncryptUpdate(ctx,ciph,&nWritten,(unsigned char*)msg,len))
		ERR_print_errors_fp(stderr);
	EVP_CIPHER_CTX_free(ctx);

	// size_t ctlen = nWritten;
	// printf("ciphertext of length %i:\n",nWritten);
	// for (int i = 0; i < ctlen; i++) {
	// 	printf("%02x",ciph[i]);
	// }
	// printf("\n");
}

static void use_decrypt(unsigned char* ciph, char* plain, unsigned char* k, unsigned char* iv)
{
	EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
	if (1!=EVP_DecryptInit_ex(ctx,EVP_aes_256_ctr(),0,k,iv))
		ERR_print_errors_fp(stderr);
	int nWritten; 
	size_t ctlen = strlen(ciph);
	if (1!=EVP_DecryptUpdate(ctx,plain,&nWritten,ciph,ctlen))
		ERR_print_errors_fp(stderr);
	EVP_CIPHER_CTX_free(ctx);
}