#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int find(char const arr[], char const ch)
// Return the index of the **only** occurrence given char in the array.
// if the given char is not found, return -1.
{
  int idx = -1;
  for (int i = 0; arr[i] != '\0'; i ++) {
    if (ch == arr[i])  // found the given char
      idx = i;  // save the index
  }
  return idx;
}

int has_duplicate(char const * str)
// Return 1 if duplicate characters are found in str; otherwise, return 0.
{
  char const * a, * b;
  for (a = str; *a != '\0'; a ++) {
    for (b = a + 1; *b != '\0'; b ++) {
      if (*a == *b) return 1;
    }
  }
  return 0;
}

int main(int argc, char const *argv[])
{
  char const * from = argv[1], * to = argv[2];
  char buf[1];
  int idx;
  if (argc != 3) {
    fprintf(stderr, "Must have exactly 2 operands. Given %d.\n", argc - 1);
    exit(1);
  }
  if (strlen(from) != strlen(to)) {
    fprintf(stderr, "The two operands must have the same length.\n");
    exit(1);
  }
  if (has_duplicate(from)) {
    fprintf(stderr, "The first operand string must not have duplicate characters.\n");
    exit(1);
  }

  // start transliterating
  while(read(0, buf, 1)) {
    idx = find(from, buf[0]);
    if (idx != -1)
      write(1, &to[idx], 1);
    else
      write(1, buf, 1);
  }
  return 0;
}
