#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef _A3_H_
#define _A3_H_
#define MSIZE 10001 // Maximum number of lines in the program(empty or non empty)
#define MNAME 101   // Maximum number of names.
#define MLINE 31    // Maximum number of characters in a line.

typedef struct {
    char name[31];
    int count;
} NameCountData;

typedef enum {
    TYPE_NAMECOUNT,
    TYPE_B // there is a possibility to extend with more types in the future
} MessageType;

typedef struct {
    MessageType type;
    size_t size; // Size of the following payload
} MessageHeader;

int check_in(char *a, char *b[]);
void nprinter(char *nused[], int count[]);
void clnup(char *a1[], char *a2[]);
void ncount(char *arr[], char *nused[], int count[]);
#endif