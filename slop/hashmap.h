#ifndef HASHMAP_H
#define HASHMAP_H

// define hashtable struct
typedef struct ht ht;
// create function
ht* ht_create(void);
// destroy function
void ht_destroy(ht* table);
// get function
void* ht_get(ht* table, const char* key);
// set function, sets the value of a key
const char* ht_set(ht* table, const char* key, void* value);
// length function

// struct for the hashtable iterator

// return fuction with the hashtable iterator

// move iterator to the next item in the hashtable, update the iterator's key
// and value to current item and return true, no items = false

#endif
