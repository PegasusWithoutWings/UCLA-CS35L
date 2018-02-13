#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int frobcmp (const char *str1, const char *str2);

int frobcmp (const char *str1, const char *str2) {
  register const unsigned char *s1 = (const unsigned char*)str1;
  register const unsigned char *s2 = (const unsigned char*)str2;

  for (; *s1 == *s2; s1++, s2++) {
    if (*s1 == ' ') {
      return 0;
    }
    return ((*s1 ^ 42) < (*s2 ^ 42)) ? -1 : 1;
  }
}

/*
This program reads frobnicated text lines from standard input, and writes a 
sorted version to standard output in frobnicated form.

Format of frobnicated text in std input:
Frobnicated text lines consist of a series of non-space bytes followed by a 
single space; the spaces represent newlines in the original text.
If standard input ends in a partial record that does not have a trailing space,
your program should behave as if a space were appended to the input.

Memory Management:
Use malloc, realloc and free to allocate enough storage to hold all the input

Sorting:
Use qsort to sort the data.

Applicable input:
Works on empty files, as well as on files that are relatively large

Error Handling:
If the program encounters an error of any kind (including input, output or 
memory allocation failures, it will report the error to stderr and exit with 
status 1; otherwise, the program will succeed and exit with status 0. 
*/
int main(void)
{ 
  // Variable declarations
  char currentChar;
  char *inputBuffer, **lineBuffer;
  int inputCount, inputBufferSize;

  inputCount = 0;
  inputBufferSize = 20;
  // The inputBuffer by default can store 20 characters
  inputBuffer = (char*) malloc(sizeof(char) * inputBufferSize);

  // Reading from the standard input stream and store them into the 
  // inputBuffer, until we reach EOF or reads an error
  while (1) {
    currentChar = getchar();
    if (feof(stdin)) { // If reached the end of file
      // Make sure that if the file is nonempty, the output ends with a 
      // trailing space
      if (inputCount > 0) {
        // Check if the last input is a space
        if (inputBuffer[inputCount - 1] != ' ') {
          inputBuffer[inputCount++] = ' ';
        }
      }
      break;
    }
    if (ferror(stdin)) { // if input has an error
      fprintf(stderr, "IO Error");
      exit(1);
    }
    // ENSURE: currentChar is neither end of file nor error

    // Before we add the new character to the inputBuffer, we need to make sure
    // that there is still enough memory allocated
    if (inputCount == inputBufferSize) {
      // Double the memory allocated
      inputBufferSize *= 2;
      inputBuffer = (char*) realloc(inputBuffer, sizeof(char) *  
                                      inputBufferSize);
      if (inputBuffer == NULL) {
        fprintf(stderr, "Memory Allocation Error");
        exit(1);
      }
    }
    inputBuffer[inputCount++] = currentChar;
  }
  // ENSURE: inputBuffer stores all the input and ends with a space if input
  // is not empty

  // If the input is empty, exit the program
  if (inputCount == 0) {
    exit(0);
  }
  // Process the inputBuffer and store each frobnicated text as an element into
  // lineBuffer.
  char *line = inputBuffer;
  for (int i = 0; i < inputCount; i++) {
    
  }
  // ENSURE: lineBuffer is an array of pointer to frobnicated texts

  // sort the lineBuffer
  // qsort();

  // Output the result of qsort into stdout

  // Exit the program
  exit(0);
}