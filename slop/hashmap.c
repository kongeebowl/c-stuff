#include "hashmap.h"
#include <stddef.h>
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
