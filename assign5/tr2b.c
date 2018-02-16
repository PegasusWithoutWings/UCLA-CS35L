#include<stdio.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>

void checkDuplicate(const char *from) {
  // We are going to use hashtable to check duplication
  ENTRY e, *ep;
  for (int i = 0; i < strlen(from); i++) {
    e.key = from[i];
    e.data = (void *) i;
    ep = hsearch(e, FIND);
    if (ep) {
      fprintf(stderr, "Duplicate byte in from: %c\n", from[i]);
      exit(1);
    } 
    else {
      ep = hsearch(e, ENTER);
    }
  }
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