#ifndef HASH_TABLE
#define HASH_TABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define INIT_CAP 128

typedef struct
{
    char *key;
    char *value;
} KV;

typedef struct
{
    ssize_t cap;
    ssize_t size;
    KV *pairs[INIT_CAP];
} HT;

HT *create_ht(void);
void destroy_ht(HT *ht);
int get_hash(HT *ht, char *key);
void ht_put(HT *ht, char *key, char *val);
void resize(HT *ht);
void ht_print(HT *ht);
char *ht_get(HT *ht, char *key);

#endif