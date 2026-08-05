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

int main(void) {
  printf("hi");

  return 0;
}
