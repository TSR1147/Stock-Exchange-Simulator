#include <iostream>
#include <windows.h>
#include <bcrypt.h>
#include "password_hash.h"

using namespace std;

// Generate a random salt.
bool generateSalt(unsigned char salt[], int saltSize)
{
    if(salt == NULL || saltSize != SALT_SIZE)
        return false;

    NTSTATUS status = BCryptGenRandom(
        NULL,
        salt,
        saltSize,
        BCRYPT_USE_SYSTEM_PREFERRED_RNG
    );

    if(status == 0)
        return true;

    return false;
}

// Generate password hash using PBKDF2.
bool hashPassword(const string& password, unsigned char salt[], int saltSize, unsigned char hash[], int hashSize)
{
    if(salt == NULL || hash == NULL)
        return false;

    if(saltSize != SALT_SIZE || hashSize != HASH_SIZE)
        return false;

    BCRYPT_ALG_HANDLE hAlg = NULL;

    // Open SHA-256 algorithm provider for HMAC.
    NTSTATUS status = BCryptOpenAlgorithmProvider(
        &hAlg,
        BCRYPT_SHA256_ALGORITHM,
        NULL,
        BCRYPT_ALG_HANDLE_HMAC_FLAG
    );

    if(status != 0)
        return false;

    // Generate password hash using PBKDF2.
    status = BCryptDeriveKeyPBKDF2(
        hAlg,
        (PUCHAR)password.c_str(),
        (ULONG)password.length(),
        salt,
        saltSize,
        120000,
        hash,
        hashSize,
        0
    );

    // Close algorithm provider.
    BCryptCloseAlgorithmProvider(hAlg, 0);

    if(status == 0)
        return true;

    return false;
}

// Verify the entered password.
bool verifyPassword(const string& password, unsigned char salt[], int saltSize, unsigned char storedHash[], int hashSize)
{
    if(storedHash == NULL || hashSize != HASH_SIZE)
        return false;

    unsigned char newHash[HASH_SIZE];

    if(!hashPassword(password, salt, saltSize, newHash, HASH_SIZE))
        return false;

    // Compare the newly generated hash with the stored hash.
    unsigned char difference = 0;

    for(int i = 0; i < HASH_SIZE; i++)
    {
        difference |= newHash[i] ^ storedHash[i];
    }

    return difference == 0;
}
