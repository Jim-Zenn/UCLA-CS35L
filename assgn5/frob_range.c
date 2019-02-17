/*
 * frob_range.c
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#include <limits.h>
#include <stdio.h>

int main()
{
  for (char c = CHAR_MIN; c < CHAR_MAX; c ++)
    printf("orig_c: %d\tfrob_c: %d\n", c, c ^ 42);
  return 0;
}

