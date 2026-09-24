#include "hash_table.h"

// int main(void)
// {
//     HT *ht = create_ht();
//     ht_put(ht, "Nikita", "Semendjajevs");
//     ht_put(ht, "Anastasija", "Aleksandrova");
//     ht_put(ht, "Arvids", "Senkans");
//     ht_put(ht, "Aleksandrs", "Zalmans");
//     ht_put(ht, "Egors", "Malishevs");
//     ht_put(ht, "Lauren", "Tang");
//     ht_put(ht, "Helena", "Ilisevic");
//     ht_print(ht);
//     get(ht, "Nikita");
//     destroy_ht(ht);
//     return 0;
// }

void ht_put(HT *ht, char *key, char *val)
{
    int hash = get_hash(ht, key);
    KV *kv = malloc(sizeof(KV));
    kv->key = strdup(key);
    kv->value = strdup(val);

    KV *temp = ht->pairs[hash];
    if (temp == NULL)
    {
        ht->pairs[hash] = kv;
    }
    else
    {
        while (temp != NULL)
        {
            temp = ht->pairs[++hash];
        }
        ht->pairs[hash] = kv;
    }

    if (ht->size >= ht->cap * (3.0 / 4.0))
    {
        // resize(ht);
    }

    ht->size++;
}

int get_hash(HT *ht, char *key)
{
    uint64_t hash = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        hash += ((uint64_t)key[i] * 7) + i;
    }
    return ((hash * 31)) % ht->cap;
}

void ht_print(HT *ht)
{
    KV *pair = NULL;
    for (ssize_t i = 0, j = 0; i < ht->cap || j != ht->size; i++)
    {
        pair = ht->pairs[i];
        if (pair == NULL)
        {
            continue;
        }
        else
        {
            printf("{%s : %s}\n", pair->key, pair->value);
            j++;
        }
        pair++;
    }
}

// void resize(HT *ht)
// {
// }

HT *create_ht(void)
{
    HT *ht = (HT *)calloc(1, sizeof(HT));
    ht->cap = INIT_CAP;
    ht->size = 0;
    return ht;
}

void destroy_ht(HT *ht)
{
    KV *pair = NULL;
    for (ssize_t i = 0, j = 0; i < ht->cap || j != ht->size; i++)
    {
        pair = ht->pairs[i];
        if (pair == NULL)
        {
            continue;
        }
        else
        {
            free(ht->pairs[i]->key);
            free(ht->pairs[i]->value);
            j++;
        }
        pair++;
    }
    free(ht);
}

char *ht_get(HT *ht, char *key)
{
    int hash = get_hash(ht, key);
    if (ht->pairs[hash] == NULL)
        return NULL;
    char *k = NULL;
    if (strcmp(key, ht->pairs[hash]->key) == 0)
    {
        k = ht->pairs[hash]->value;
        assert(k != NULL);
    }
    else
    {
        while (strcmp(key, ht->pairs[++hash]->key) != 0)
        {
            ;
        }
        k = ht->pairs[hash]->key;
    }
    return k;
}