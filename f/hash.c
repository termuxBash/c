#include <stdio.h>
#include <stdlib.h>

#define EMPTY 0
#define OCCUPIED 1
#define DELETED 2

typedef struct
{
    int key;
    int value;
    int state;
} HashEntry;

typedef struct
{
    HashEntry *table;
    int size;
    int count;
    int collisions;
} HashTable;

int hashFunction(int key, int size)
{
    int index = key % size;
    if (index < 0)
    {
        index += size;
    }
    return index;
}

void initHashTable(HashTable *ht, int size)
{
    int i;

    if (size <= 0)
    {
        printf("[Error] Table size must be positive.\n");
        exit(1);
    }

    ht->table = (HashEntry *)malloc(sizeof(HashEntry) * size);
    if (ht->table == NULL)
    {
        printf("[Error] Memory allocation failed!\n");
        exit(1);
    }

    ht->size = size;
    ht->count = 0;
    ht->collisions = 0;

    for (i = 0; i < size; i++)
    {
        ht->table[i].state = EMPTY;
    }
}

void freeHashTable(HashTable *ht)
{
    free(ht->table);
    ht->table = NULL;
    ht->size = 0;
    ht->count = 0;
    ht->collisions = 0;
}

int insert(HashTable *ht, int key, int value)
{
    int index, start, firstDeleted;

    if (ht->count >= ht->size)
    {
        return -1;
    }

    index = hashFunction(key, ht->size);
    start = index;
    firstDeleted = -1;

    do
    {
        if (ht->table[index].state == OCCUPIED)
        {
            if (ht->table[index].key == key)
            {
                return 0;
            }
            ht->collisions++;
        }
        else if (ht->table[index].state == DELETED)
        {
            if (firstDeleted == -1)
            {
                firstDeleted = index;
            }
        }
        else
        {
            int slot = (firstDeleted != -1) ? firstDeleted : index;
            ht->table[slot].key = key;
            ht->table[slot].value = value;
            ht->table[slot].state = OCCUPIED;
            ht->count++;
            return 1;
        }

        index = (index + 1) % ht->size;
    } while (index != start);

    if (firstDeleted != -1)
    {
        ht->table[firstDeleted].key = key;
        ht->table[firstDeleted].value = value;
        ht->table[firstDeleted].state = OCCUPIED;
        ht->count++;
        return 1;
    }

    return -1;
}

int searchHash(HashTable *ht, int key)
{
    int index = hashFunction(key, ht->size);
    int start = index;

    do
    {
        if (ht->table[index].state == EMPTY)
        {
            return -1;
        }

        if (ht->table[index].state == OCCUPIED && ht->table[index].key == key)
        {
            return index;
        }

        index = (index + 1) % ht->size;
    } while (index != start);

    return -1;
}

int updateHash(HashTable *ht, int key, int newValue)
{
    int index = searchHash(ht, key);

    if (index == -1)
    {
        return 0;
    }

    ht->table[index].value = newValue;
    return 1;
}

int deleteHash(HashTable *ht, int key)
{
    int index = searchHash(ht, key);

    if (index == -1)
    {
        return 0;
    }

    ht->table[index].state = DELETED;
    ht->count--;
    return 1;
}

void displayHashTable(HashTable *ht)
{
    int i;

    printf("\nHash Table (index -> key : value)\n");
    for (i = 0; i < ht->size; i++)
    {
        if (ht->table[i].state == OCCUPIED)
        {
            printf("[%d] -> %d : %d\n", i, ht->table[i].key, ht->table[i].value);
        }
    }
    printf("Total active entries: %d\n", ht->count);
    printf("Total collisions: %d\n", ht->collisions);
}

float loadFactor(HashTable *ht)
{
    if (ht->size == 0)
    {
        return 0.0f;
    }
    return (float)ht->count / (float)ht->size;
}

int main()
{
    HashTable ht;
    int keys[] = {12, 22, 32, 42, 52, 62, 7, 17, 27, 37};
    int values[] = {120, 220, 320, 420, 520, 620, 70, 170, 270, 370};
    int n = sizeof(keys) / sizeof(keys[0]);
    int i;

    printf("=== HASH TABLE DEMO (OPEN ADDRESSING + LINEAR PROBING) ===\n\n");

    initHashTable(&ht, 10);

    for (i = 0; i < n; i++)
    {
        printf("Inserting (%d, %d)\n", keys[i], values[i]);
        insert(&ht, keys[i], values[i]);
    }

    displayHashTable(&ht);
    printf("Load factor: %.2f\n", loadFactor(&ht));

    printf("\nSearching for key 42 -> %s\n", searchHash(&ht, 42) != -1 ? "Found" : "Not Found");
    printf("Searching for key 99 -> %s\n", searchHash(&ht, 99) != -1 ? "Found" : "Not Found");

    printf("\nUpdating key 17 to value 171...\n");
    updateHash(&ht, 17, 171);

    printf("Deleting key 32...\n");
    deleteHash(&ht, 32);

    displayHashTable(&ht);

    freeHashTable(&ht);
    return 0;
}
