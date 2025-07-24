//GEMINI-pro DATASET v1.0 Category: Cryptographic hash function ; Style: romantic
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/evp.h>

// Custom hash function
unsigned char* loveHash(const char *message, int messageLen) {
  // Create an SHA256 context
  EVP_MD_CTX *ctx = EVP_MD_CTX_new();
  if (!ctx) {
    printf("Error creating SHA256 context\n");
    return NULL;
  }

  // Initialize the context
  if (!EVP_DigestInit_ex(ctx, EVP_sha256(), NULL)) {
    printf("Error initializing SHA256 context\n");
    EVP_MD_CTX_free(ctx);
    return NULL;
  }

  // Update the context with the message
  if (!EVP_DigestUpdate(ctx, message, messageLen)) {
    printf("Error updating SHA256 context\n");
    EVP_MD_CTX_free(ctx);
    return NULL;
  }

  // Finalize the context and get the digest
  unsigned char digest[SHA256_DIGEST_LENGTH];
  unsigned int digestLen;
  if (!EVP_DigestFinal_ex(ctx, digest, &digestLen)) {
    printf("Error finalizing SHA256 context\n");
    EVP_MD_CTX_free(ctx);
    return NULL;
  }

  // Clean up
  EVP_MD_CTX_free(ctx);

  // Return the digest
  return digest;
}

int main() {
  // Our secret love message
  const char *message = "My heart beats only for you, my beloved.";
  int messageLen = strlen(message);

  // Create a hash of the message
  unsigned char *hash = loveHash(message, messageLen);

  // Print the hash in hexadecimal format
  printf("Our love's eternal bond, in the form of a cryptographic hash:\n");
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    printf("%02x", hash[i]);
  }
  printf("\n");

  // Free the hash
  free(hash);

  return 0;
}