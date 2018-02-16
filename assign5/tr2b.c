#include<stdio.h>
#include<stdlib.h>
#include <string.h>

int cmp(const void * a, const void * b) {
   return ( *(char*)a - *(char*)b );
}
 
void checkDuplicate(const char *from) {
  // Because we do not have a hashtable, we are going to use sorting to speed
  // up algorithm
  char * copy = malloc(strlen(from) + 1);
  if (!copy) {
    fprintf(stderr, "Memory allocation error.\n");
    exit(1);
  } 
  strcpy(copy, from);
  qsort(copy, strlen(copy), sizeof(char), cmp);
  int strlength = strlen(copy);
  // strlength cannot be 0, because if it is, then from operand does not exist
  for (int i = 0; i < strlength - 1; i++) {
    if (copy[i] == copy[i + 1]) {
      fprintf(stderr, "Duplicate character in from.\n");
      exit(1);
    }
  }
  free(copy);
}

int main(int argc, const char *argv[]) {
  // First parse the argument
  // Check if the number of arguments is correct
  if (argc != 3) {
    fprintf(stderr, "Wrong number of operands: %d.\n", argc);
    exit(1);
  }
  const char *from = argv[1], *to = argv[2];

  // Check if from and to are of the same length
  if (strlen(from) != strlen(to)) {
    fprintf(stderr, "from and to are of different lengths.");
    exit(1);
  }

  // Check if from has duplicate byte in from
  checkDuplicate(from);
  
  // We will first build a hashtable that maps from from[i] to to[i], for all
  // 0 <= i < len(from)

}