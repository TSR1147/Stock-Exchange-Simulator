#ifndef PASSWORD_HASH_H
#define PASSWORD_HASH_H

#include <string>
using namespace std;

// Fixed sizes for salt and password hash.
const int SALT_SIZE = 16;
const int HASH_SIZE = 32;

// To Generate A Random Salt For Password Hashing.
bool generateSalt(unsigned char salt[], int saltSize);

// To Generate Password Hash Using PBKDF2 Algorithm.
bool hashPassword(const string& password, unsigned char salt[], int saltSize, unsigned char hash[], int hashSize);

// Verify the entered password.
bool verifyPassword(const string& password, unsigned char salt[], int saltSize, unsigned char storedHash[], int hashSize);

#endif
