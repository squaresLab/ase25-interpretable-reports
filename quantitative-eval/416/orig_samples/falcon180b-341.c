//Falcon-180B DATASET v1.0 Category: Public-Key Algorithm Implementation ; Style: immersive
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//Defining the structure for the public and private keys
typedef struct{
    int *public_key;
    int *private_key;
}key_pair;

//Function to generate the prime numbers
int is_prime(int n){
    if(n <= 1){
        return 0;
    }
    for(int i = 2; i*i <= n; i++){
        if(n % i == 0){
            return 0;
        }
    }
    return 1;
}

//Function to generate the keys
void generate_keys(key_pair *key){
    int private_key = rand() % 10000;
    key->private_key = &private_key;

    while(1){
        int public_key = rand() % 10000;
        if(is_prime(public_key)){
            key->public_key = &public_key;
            break;
        }
    }
}

//Function to encrypt the message
void encrypt(int message, int public_key, int *encrypted_message){
    int temp = message;
    int result = 1;

    while(temp!= 0){
        if(temp % 2 == 1){
            result = (result * public_key) % 10000;
        }
        temp /= 2;
        public_key = (public_key * public_key) % 10000;
    }

    *encrypted_message = result;
}

//Function to decrypt the message
void decrypt(int encrypted_message, int private_key, int *decrypted_message){
    int temp = encrypted_message;
    int result = 1;

    while(temp!= 0){
        if(temp % 2 == 1){
            result = (result * private_key) % 10000;
        }
        temp /= 2;
    }

    *decrypted_message = result;
}

int main(){
    key_pair key;
    generate_keys(&key);

    int message = 12345;
    int encrypted_message;
    encrypt(message, *key.public_key, &encrypted_message);

    printf("Encrypted message: %d\n", encrypted_message);

    int decrypted_message;
    decrypt(encrypted_message, *key.private_key, &decrypted_message);

    printf("Decrypted message: %d\n", decrypted_message);

    return 0;
}