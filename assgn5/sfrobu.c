#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#define DEFAULT_WORD_CAPACITY 40
#define DEFAULT_WORD_ARRAY_CAPACITY 128
#define SCALING_FACTOR 2

int frobcmp(const void * s1, const void * s2);
// Compare the original string of the given two frobnicated string. Return 1 if
// the original string of s1 is 'greater than' that of s2; return 0 if equal;
// -1 otherwise.

int frobptrcmp(const void * s1, const void * s2);
// Compare the original string of the two frobnicated string.

int frobcmp_f(const void * s1, const void * s2);
// Case insensitively compare the original string of the given two frobnicated
// string. Return 1 if the original string of s1 is 'greater than' that of s2;
// return 0 if equal; -1 otherwise.

int frobptrcmp_f(const void * s1, const void * s2);
// Case insensitively compare the original string of the two frobnicated string.


size_t scan_buf_words(char * buf, size_t buf_size, char *** word_arr);
// scan the buffer, find start position of every words, and store them
// in `word_arr`. Returns the number of word positions stored in the
// `word_arr`.
// The `word_arr` need not to be initialized.
// NOTICE that this function does not create any copy of the content in the
// given buffer. It only stores words' starting characters' address.

size_t scan_stream_words(int fd, char *** word_arr);
// scan the stream, malloc and store every words, and store there pointers
// in `word_arr`. Returns the number of word positions stored in the
// `word_arr`.
// The `word_arr` need not to be initialized.

int main(int argc, char const *argv[]) {
  // basic checks
  if (argc > 2) {
    fprintf(stderr, "Too many operands provided.\n");
    exit(1);
  }
  if (argc == 2 && strcmp(argv[1], "-f") != 0) {
    fprintf(stderr, "%s is not a valid operand.\n", argv[1]);
    exit(1);
  }
  struct stat file_stat;
  if (fstat(0, &file_stat) == -1) {
    fprintf(stderr, "Error checking file status.\n");
    exit(1);
  }
  int is_case_insensitive;
  if (argc == 2 && strcmp(argv[1], "-f") == 0)
    is_case_insensitive = 1;
  char ** word_arr;
  char * file_buf;
  int num_words;
  int is_file;
  if (S_ISREG(file_stat.st_mode)) {
    // stdin holds a regular file
    is_file = 1;
    // malloc memory for the file buffer
    file_buf = (char *) malloc((file_stat.st_size) * sizeof(char));
    if (file_buf == NULL) {
      fprintf(stderr, "Failed to allocate memory for file buffer.\n");
      exit(1);
    }
    if(read(0, file_buf, file_stat.st_size) == -1) {
      fprintf(stderr, "Failed when reading file.\n");
      exit(1);
    }
    // manually replace the trailing newline with a ' ' to make things easier
    file_buf[file_stat.st_size - 1] = ' ';
    num_words = scan_buf_words(file_buf, file_stat.st_size, &word_arr);
  } else {
    // stdin is an input stream
    is_file = 0;
    num_words = scan_stream_words(0, &word_arr);
  }
  char * chptr;

  // sort the frob array
  if (is_case_insensitive)
    qsort(word_arr, num_words, sizeof(char *), frobptrcmp_f);
  else
    qsort(word_arr, num_words, sizeof(char *), frobptrcmp);

  for (int i = 0; i < num_words; i ++) {
    chptr = word_arr[i];
    // the end of the word is marked by ' '
    while (*chptr != ' ') {
      putchar(*chptr);
      chptr ++;
    }
    putchar(' ');
  }

  // free memory
  if (is_file == 1) {
    free(file_buf);
    free(word_arr);
  } else {
    for (int k = 0; k < num_words; k ++)
      free(word_arr[k]);
    free(word_arr);
  }
  return 0;
}

size_t scan_buf_words(char * buf, size_t buf_size, char *** word_arr) {
  size_t offset = 0, num_words = 0;
  // locate the first word first. Ignore all the leading spaces.
  for (; offset < buf_size && buf[offset] == ' '; offset ++);
  if (offset == buf_size) {
    // curr is at the end of the buffer, the buffer doesn't have any words
    *word_arr = NULL;
    return 0;
  }
  int word_pos_arr_capacity = DEFAULT_WORD_ARRAY_CAPACITY;
  *word_arr = malloc(word_pos_arr_capacity * sizeof(void *));
  // curr should now be pointing at the start of the first word.
  (*word_arr)[0] = buf + offset;
  num_words ++;
  // To look for words, we look for patterns where a space character ' ' is
  // followed by a non-space character.
  for (; offset < buf_size - 1; offset ++) {
    if (buf[offset] == ' ' && buf[offset + 1] != ' ') {
      (*word_arr)[num_words] = &(buf[offset + 1]);
      num_words ++;
    }
    if (num_words >= word_pos_arr_capacity) {
      *word_arr = realloc(*word_arr,
        word_pos_arr_capacity * SCALING_FACTOR * sizeof(void *));
    }
  }
  return num_words;
}

size_t scan_stream_words(int fd, char *** word_arr) {
  size_t word_buf_capacity = DEFAULT_WORD_CAPACITY;
  size_t array_buf_capacity = DEFAULT_WORD_ARRAY_CAPACITY;
  char ch;
  char * word_buf = (char *) malloc(word_buf_capacity * sizeof(char));
  *word_arr = (char **) malloc(array_buf_capacity * sizeof(void *));
  int num_char = 0, num_words = 0;
  while (read(fd, &ch, 1)) {
    if (num_char == 0 && ch == ' ')
      continue;  // the space precedes a new word; just ignore it
    else if (ch == ' ') {
      word_buf[num_char] = ' ';
      (*word_arr)[num_words] = word_buf;  // save the word to the word array
      word_buf_capacity = DEFAULT_WORD_CAPACITY;  // reset the buffer's size
      word_buf = (char *) malloc(word_buf_capacity * sizeof(char));
      num_char = 0;  // reset char count
      num_words ++;  // increment word count
      // if the word array buffer is full, resize the word array buffer
      if (num_words >= array_buf_capacity) {
        array_buf_capacity *= SCALING_FACTOR;
        *word_arr =
          (char **) realloc(*word_arr, array_buf_capacity * sizeof (void *));
      }
    } else {
      word_buf[num_char] = ch;
      num_char ++;
      // if the word buffer is full, resize the word buffer
      if (num_char >= word_buf_capacity) {
        word_buf_capacity *= SCALING_FACTOR;
        word_buf = (char *) realloc(word_buf, word_buf_capacity * sizeof(char));
      }
    }
  };
  // save the last word if it was not saved
  if (num_char != 0) {
    word_buf[num_char] = ' ';
    (*word_arr)[num_words] = word_buf;  // save the word to the word array
    num_words ++;
  }
  return num_words;
  return 0;
}

int frobcmp(const void * s1, const void * s2) {
  char *a = (char *) s1, *b = (char *) s2;
  while (*a != ' ' || *b != ' ') {
    if ((*a ^ 42) < (*b ^ 42))  return -1;
    if ((*a ^ 42) > (*b ^ 42))  return 1;
    a++; b++;
  }
  if (*a == ' ')  return -1;
  if (*b == ' ')  return 1;
  // the two frobnicated memory are exactly the same
  return 0;
}

int frobptrcmp(const void * s1, const void * s2) {
  return frobcmp(*(char **)s1, *(char **)s2);
}

int frobcmp_f(const void * s1, const void * s2) {
  char *a = (char *) s1, *b = (char *) s2;
  char frob_a, frob_b;
  while (*a != ' ' || *b != ' ') {
    frob_a = *a ^ 42;
    frob_b = *b ^ 42;
    if (isalpha(frob_a)) frob_a = toupper(frob_a);
    if (isalpha(frob_b)) frob_b = toupper(frob_b);
    if (frob_a < frob_b)  return -1;
    if (frob_a > frob_b)  return 1;
    a++; b++;
  }
  if (*a == ' ')  return -1;
  if (*b == ' ')  return 1;
  // the two frobnicated memory are exactly the same
  return 0;
}

int frobptrcmp_f(const void * s1, const void * s2) {
  return frobcmp_f(*(char **)s1, *(char **)s2);
}
