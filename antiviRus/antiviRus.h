#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <windows.h>

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;

#define Blocksize 100 * 1000
#define Hashsize 100 * 1000 + 3

#define $1 (int8 *)
#define $2 (int16)
#define $4 (int32 *)
#define $8 (int64)
#define $c (char *)
#define $i (int)

int main(int, char**);