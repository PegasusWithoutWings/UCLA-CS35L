#include<stdio.h>
#include <stdlib.h>
#include <search.h>

char *data[] = { "alpha", "bravo", "charlie", "delta",
     "echo", "foxtrot", "golf", "hotel", "india", "juliet",
     "kilo", "lima", "mike", "november", "oscar", "papa",
     "quebec", "romeo", "sierra", "tango", "uniform",
     "victor", "whisky", "x-ray", "yankee", "zulu"
};

// As a first step, we will get comfortable with getchar and stdin
int main() {
  while (!feof(stdin)) {
    fputc(fgetc(stdin), stdout);
  }
  return 0;
}