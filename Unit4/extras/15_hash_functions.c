/*==========================================================================
 * FILE  : 15_hash_functions.c
 * TOPIC : HASH FUNCTIONS - DIVISION MULTIPLICATION MID-SQUARE FOLDING AND STRINGS
 *==========================================================================
 *
 * THEORY
 * ------
 * A hash function maps a key into the table range 0..18. This program
 * demonstrates every method listed in the hashing lecture. Division
 * normalizes negative remainders; the other numeric demonstrations use the
 * key magnitude.
 *
 * ALGORITHM AND OPERATIONS
 * ------------------------
 * Division: normalized key mod 19. Multiplication: floor(19*frac(|key|*A)),
 * A about 0.61803398875. Mid-square: square the magnitude and extract two
 * middle decimal digits. Folding: add two-digit decimal groups. String
 * hashing: accumulate a polynomial modulo 19.
 *
 * COMPLEXITY
 * ----------
 * Integer methods use a bounded machine-word key; string hashing is O(L).
 * Additional storage is constant for bounded integer keys.
 *
 * COMPILE AND RUN
 * ---------------
 * gcc -std=c99 15_hash_functions.c -o study.exe
 * On Windows: .\study.exe
 * Compile this file by itself. Each numbered file has its own main
 * function.
 *
 * SAMPLE RUN
 * ----------
 * Integer key: 44
 * Table size = 19
 * Division: 6
 * Mid-square (middle two decimal digits): 17
 * Folding (two-digit groups): 6
 * Multiplication: 3
 * String key: abc
 * Polynomial string hash: 5
 *
 * SOURCE
 * ------
 * Hash lecture methods: division, multiplication, mid-square, folding,
 * string hashing
 *
 *==========================================================================*/

#include <stdio.h>
#include <string.h>
#define TABLE_SIZE 19
int divisionHash(int key)
{
    int r = key % TABLE_SIZE;
    return r < 0 ? r + TABLE_SIZE : r;
}
unsigned long long magnitude(int key)
{
    return key < 0 ? (unsigned long long)(-(long long)key) : (unsigned long long)key;
}
int midSquareHash(int key)
{
    unsigned long long n = magnitude(key), square;
    char digits[32];
    int len, start, i, middle = 0;
    square = n * n;
    sprintf(digits, "%llu", square);
    len = (int)strlen(digits);
    start = len > 2 ? (len - 2) / 2 : 0;
    for (i = start; i < len && i < start + 2; i++) middle = middle * 10 + digits[i] - '0';
    return middle % TABLE_SIZE;
}
int foldingHash(int key)
{
    unsigned long long n = magnitude(key), sum = 0;
    do { sum += n % 100; n /= 100; } while (n);
    return (int)(sum % TABLE_SIZE);
}
int multiplicationHash(int key)
{
    double product = (double)magnitude(key) * 0.6180339887498949;
    unsigned long long whole = (unsigned long long)product;
    return (int)(TABLE_SIZE * (product - (double)whole));
}
int stringHash(const char *s)
{
    unsigned int h = 0;
    while (*s) h = (h * 31U + (unsigned char)*s++) % TABLE_SIZE;
    return (int)h;
}
int main(void)
{
    int key;
    char word[100];
    printf("Enter an integer key: ");
    if (scanf("%d", &key) != 1) return 1;
    printf("Table size = %d\n", TABLE_SIZE);
    printf("Division: %d\n", divisionHash(key));
    printf("Mid-square (middle two decimal digits): %d\n", midSquareHash(key));
    printf("Folding (two-digit groups): %d\n", foldingHash(key));
    printf("Multiplication: %d\n", multiplicationHash(key));
    printf("Enter a string key: ");
    if (scanf("%99s", word) == 1) printf("Polynomial string hash: %d\n", stringHash(word));
    return 0;
}
