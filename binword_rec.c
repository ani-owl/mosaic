// @brief Generate binary words.
// @author <aurelien.esnard@u-bordeaux.fr>

#define N 6

#include <stdio.h>
#include <stdlib.h>

void printWord(int *word, int len) {
  for (int i = 0; i < len; i++) printf("%d", word[i]);
  printf("\n");
}

void genWords(int pos, int len, int *word, unsigned long *count) {
  // stop recursive calls
  if (pos == len) {
    (*count)++;
    printWord(word, len);
    return;
  }

  // extend current word recursively with value 0 at position pos
  word[pos] = 0;
  genWords(pos + 1, len, word, count);

  // extend current word recursively with value 1 at position pos
  word[pos] = 1;
  genWords(pos + 1, len, word, count);
}

int main(int argc, char *argv[]) {
  int word[N];
  unsigned long nbwords = 0;

  // reset word
  for (int k = 0; k < N; k++) word[k] = 0;

  // generate all possible words
  genWords(0, N, word, &nbwords);

  printf("=> %lu words generated (of length %d)\n", nbwords, N);

  return EXIT_SUCCESS;
}