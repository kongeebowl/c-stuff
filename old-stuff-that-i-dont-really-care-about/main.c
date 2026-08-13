#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// byte strings: length:contents ex: 4:spam = spam
// integers: i<integer>e ex: i42e = 42
// lists: l<contents>e ex: l4spam3:egg4:porke ['spam', 'eggs', 'pork']
// dictionaries: d<key1><value1>...e ex: d3:cow3:moo4:spam4:eggse
/* {
  "cow": "moo",
  "spam": "eggs"
}
*/

typedef enum { BENCODE_INT, BENCODE_STR } BencodeType;

typedef struct {
  BencodeType type;
  union {
    long long integer;
    struct {
      char *data;
      size_t len;
    } string;
  };
} BencodeValue;

// Double pointers: they are used to

BencodeValue *parse_int(const char **buf) {
  (*buf)++; // skips i

  // converts the digits into long long
  long long num = atoll(*buf);
  while (**buf != 'e' && **buf != '\0') {
    (*buf)++;
  }
  if (**buf == 'e') {
    (*buf)++;
  }

  BencodeValue *val = malloc(sizeof(BencodeValue));
  val->type = BENCODE_INT;
  val->integer = num;
  return val;
}

BencodeValue *parse_string(const char **buf) {
  int length = atoi(*buf);

  while (**buf != ':' && **buf != '\0') {
    (*buf)++;
  }
  if (**buf == ':') {
    (*buf)++;
  }

  // Allocate the memory to add the null terminator to the string
  char *str_data = malloc(length + 1);
  memcpy(str_data, *buf, length);
  str_data[length] = '\0';

  *buf += length;

  BencodeValue *val = malloc(sizeof(BencodeValue));
  val->type = BENCODE_STR;
  val->string.data = str_data;
  val->string.len = length;
  return val;
}

// parse_value returns the desired function based on which type the encoded
// value is
BencodeValue *parse_value(const char **buf) {
  if (**buf == 'i') {
    return parse_int(buf);
  } else if (**buf >= '0' && **buf <= '9') {
    return parse_string(buf);
  }
  return NULL;
}

BencodeValue *parse_list(const)

    void free_value(BencodeValue *val) {
  if (!val)
    return;
  if (val->type == BENCODE_STR) {
    free(val->string.data);
  }
  free(val);
}

int main() {
  // --- Example 1: Parsing "i12345e" ---
  const char *test_int = "i12345e";
  const char *ptr1 = test_int; // ptr1 points to 'i'

  // Address-of (&ptr1) passes the double-pointer so parse_value can update ptr1
  BencodeValue *res1 = parse_value(&ptr1);

  if (res1 && res1->type == BENCODE_INT) {
    printf("Parsed Integer: %lld\n", res1->integer);
  }
  free_value(res1); // Clean up allocated memory

  // --- Example 2: Parsing "5:hello" ---
  const char *test_str = "5:hello";
  const char *ptr2 = test_str; // ptr2 points to '5'

  BencodeValue *res2 = parse_value(&ptr2);

  if (res2 && res2->type == BENCODE_STR) {
    printf("Parsed String: %s (Length: %zu)\n", res2->string.data,
           res2->string.len);
  }
  free_value(res2); // Clean up allocated memory

  return 0;
}
