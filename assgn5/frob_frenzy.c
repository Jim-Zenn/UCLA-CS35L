/*
 * frob_frenzy.c
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#include <stdio.h>
#include <string.h>
#include <limits.h>

int main()
{
  char orig[255];
  char frob[255];
  for (char c = CHAR_MIN; c < CHAR_MAX; c ++) {
    orig[c + 128] = c;
    frob[c + 128] = c;
  }
  memfrob(frob, 255);
  for (char c = CHAR_MIN; c < CHAR_MAX; c ++) {
    printf("%d\t%d\n", orig[c + 128], frob[c + 128]);
  }
  putchar('\n');

  return 0;
}
