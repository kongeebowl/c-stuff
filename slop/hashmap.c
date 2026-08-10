#include "hashmap.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  const char* key;
  void* value;
} ht_entry;

struct ht {
  ht_entry* entries;
  size_t capacity;
  size_t length;
};

#define INITIAL_CAPACITY 16

ht* ht_create(void) {
  ht* table = malloc(sizeof(ht));
  if (table == NULL) {
    return NULL;
  };
  table->capacity = INITIAL_CAPACITY;
  table->length = 0;

  table->entries = calloc(table->capacity, sizeof(ht_entry));
  if (table->entries == NULL) {
    free(table);
    return (NULL);
  }
  return table;
}

void ht_destroy(ht* table) {
  for (size_t i = 0; i < table->capacity; i++) {
    free((void*)table->entries[i].key);
  };

  free(table->entries);
  free(table);
}

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

static uint64_t hash_key(const char* key) {
  uint64_t hash = FNV_OFFSET;
  for (const char* p = key; *p; p++) {
    hash ^= (uint64_t)(unsigned char)(*p);
    hash *= FNV_PRIME;
  }
  return hash;
}

void* ht_get(ht* table, const char* key) {
  uint64_t hash = hash_key(key);
  size_t index = (size_t)(hash & (uint64_t)(table->capacity - 1));

  while (table->entries[index].key != NULL) {
    if (strcmp(key, table->entries[index].key) == 0) {
      return table->entries[index].value;
    };
    index++;
    if (index >= table->length) {
      index = 0;
    }
  }
  return NULL;
}
