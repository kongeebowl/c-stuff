#include "hashmap.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to test basic insertion, lookup, and length update
void test_create_and_basic_ops(void) {
  printf("Testing ht_create, ht_set, ht_get, ht_length...\n");

  ht* table = ht_create();
  assert(table != NULL);
  assert(ht_length(table) == 0);

  int val1 = 42;
  int val2 = 100;

  // Insert first key
  const char* key1 = ht_set(table, "apple", &val1);
  assert(key1 != NULL);
  assert(strcmp(key1, "apple") == 0);
  assert(ht_length(table) == 1);
  assert(ht_get(table, "apple") == &val1);

  // Insert second key
  const char* key2 = ht_set(table, "banana", &val2);
  assert(key2 != NULL);
  assert(strcmp(key2, "banana") == 0);
  assert(ht_length(table) == 2);
  assert(ht_get(table, "banana") == &val2);

  ht_destroy(table);
  printf("  [PASSED] Basic operations\n");
}

// Test retrieving non-existent keys
void test_get_non_existent(void) {
  printf("Testing ht_get with non-existent keys...\n");

  ht* table = ht_create();
  int val = 10;
  ht_set(table, "foo", &val);

  assert(ht_get(table, "bar") == NULL);
  assert(ht_get(table, "FOO") == NULL); // Case sensitivity
  assert(ht_get(table, "") == NULL);

  ht_destroy(table);
  printf("  [PASSED] Non-existent keys\n");
}

// Test updating value for an existing key
void test_overwrite_key(void) {
  printf("Testing updating existing key value...\n");

  ht* table = ht_create();
  int val1 = 10;
  int val2 = 20;

  const char* key1 = ht_set(table, "key", &val1);
  assert(ht_length(table) == 1);
  assert(*(int*)ht_get(table, "key") == 10);

  // Overwrite existing key
  const char* key2 = ht_set(table, "key", &val2);
  assert(ht_length(table) == 1); // Length should NOT increase
  assert(*(int*)ht_get(table, "key") == 20);

  ht_destroy(table);
  printf("  [PASSED] Overwrite key\n");
}

// Test edge cases: empty strings, long strings
void test_edge_cases(void) {
  printf(
      "Testing edge cases (empty keys, special characters, long strings)...\n");

  ht* table = ht_create();
  int val = 999;

  // Empty string key
  assert(ht_set(table, "", &val) != NULL);
  assert(ht_length(table) == 1);
  assert(ht_get(table, "") == &val);

  // Key with spaces and symbols
  assert(ht_set(table, "hello world @ 123!\n", &val) != NULL);
  assert(ht_get(table, "hello world @ 123!\n") == &val);

  // Very long key
  char long_key[1024];
  memset(long_key, 'a', 1023);
  long_key[1023] = '\0';

  assert(ht_set(table, long_key, &val) != NULL);
  assert(ht_get(table, long_key) == &val);

  ht_destroy(table);
  printf("  [PASSED] Edge cases\n");
}

// Test hash map iterator functionality
void test_iterator(void) {
  printf("Testing iterator (hti, ht_iterator, ht_next)...\n");

  ht* table = ht_create();

  // Iterate empty table
  hti it = ht_iterator(table);
  assert(ht_next(&it) == false);

  // Populate table
  int v1 = 1, v2 = 2, v3 = 3;
  ht_set(table, "k1", &v1);
  ht_set(table, "k2", &v2);
  ht_set(table, "k3", &v3);

  size_t count = 0;
  bool found_k1 = false, found_k2 = false, found_k3 = false;

  it = ht_iterator(table);
  while (ht_next(&it)) {
    count++;
    if (strcmp(it.key, "k1") == 0) {
      assert(*(int*)it.value == 1);
      found_k1 = true;
    } else if (strcmp(it.key, "k2") == 0) {
      assert(*(int*)it.value == 2);
      found_k2 = true;
    } else if (strcmp(it.key, "k3") == 0) {
      assert(*(int*)it.value == 3);
      found_k3 = true;
    }
  }

  assert(count == 3);
  assert(found_k1 && found_k2 && found_k3);

  ht_destroy(table);
  printf("  [PASSED] Iterator\n");
}

// Stress test: insert many items to force rehashing / collisions
void test_stress_and_collisions(void) {
  printf("Testing large volume insertion (10,000 items)...\n");

  ht* table = ht_create();
  const int NUM_ITEMS = 10000;
  int* values = malloc(NUM_ITEMS * sizeof(int));

  char key_buf[32];

  // Insert 10,000 items
  for (int i = 0; i < NUM_ITEMS; i++) {
    values[i] = i;
    snprintf(key_buf, sizeof(key_buf), "key_%d", i);
    assert(ht_set(table, key_buf, &values[i]) != NULL);
  }

  assert(ht_length(table) == (size_t)NUM_ITEMS);

  // Verify all 10,000 items can be retrieved correctly
  for (int i = 0; i < NUM_ITEMS; i++) {
    snprintf(key_buf, sizeof(key_buf), "key_%d", i);
    int* retrieved = (int*)ht_get(table, key_buf);
    if (retrieved == NULL) {
      printf("\nFailed to find key: '%s' at index %d (Table length: %zu)\n",
             key_buf, i, ht_length(table));
      assert(retrieved != NULL);
    }
  }

  ht_destroy(table);
  free(values);
  printf("  [PASSED] Stress test\n");
}

int main(void) {
  printf("--- Running Hashmap Unit Tests ---\n\n");

  test_create_and_basic_ops();
  test_get_non_existent();
  test_overwrite_key();
  test_edge_cases();
  test_iterator();
  test_stress_and_collisions();

  printf("\nAll tests passed successfully!\n");
  return 0;
}
