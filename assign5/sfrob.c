#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int frobcmp (char const* s1, char const* s2) {
  for ( ; *s1 == *s2; s1++, s2++) {
    if (*s1 == ' '){
      return 0;
    }
  }
  return ((*s1 ^ 42) < (*s2 ^ 42)) ? -1 : 1;
}

// This is a wrapper function to use frobcmp in qsort
int compar (const void *line1, const void *line2) {
  return frobcmp(*((const char **) line1), *((const char **) line2));
}
/*
This program reads frobnicated text lines from standard input, and writes a 
sorted version to standard output in frobnicated form.
*/
int main(void)
{ 
  // Variable declarations
  char currentChar;
  char *inputBuffer, **lineBuffer;
  int inputCount = 0, inputBufferSize = 20, lineNumber = 0;

  // If the stdin is a file, try to get its size
	struct stat fileStat;

	if(fstat(0, &fileStat) < 0)
	{
		fprintf( stderr, "Error: cannot read data from stdin\n");
		exit(1);
	}
  // If the file is a regular file, set the buffer size to the file size
  if (S_ISREG(fileStat.st_mode)) {
    inputBuffer = (char*) malloc(sizeof(char) * fileStat.st_size);
  }

  // The inputBuffer by default can store 20 characters
  inputBuffer = (char*) malloc(sizeof(char) * inputBufferSize);
  if (inputBuffer == NULL) {
    fprintf(stderr, "Memory Allocation Error");
    exit(1);
  }
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
          lineNumber++;
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
    if (currentChar == ' ') {
      lineNumber++;
    } 
    inputBuffer[inputCount++] = currentChar;
  }

  // If the input is empty, exit the program
  if (inputCount == 0) {
    exit(0);
  }

  // ENSURE: inputBuffer stores all the input and ends with a space 
  // Process the inputBuffer and store each frobnicated text as an element into
  // lineBuffer.
  lineBuffer = (char **) malloc(sizeof(char *) * lineNumber);
  if (inputBuffer == NULL) {
    fprintf(stderr, "Memory Allocation Error");
    exit(1);
  }
  char *line = inputBuffer;
  for (int i = 0, lineNumber = 0; i < inputCount; i++) {
    if (inputBuffer[i] == ' ') {
      lineBuffer[lineNumber++] = line;
      line = inputBuffer + i + 1;
    }
  }
  free(inputBuffer);
 // ENSURE: lineBuffer is an array of pointer to frobnicated texts
  // sort the lineBuffer
  qsort(lineBuffer, lineNumber, sizeof(char *), &compar);
  // Output the result of qsort into stdout
  for (int i = 0; i < lineNumber; i++) {
    for (int j = 0; lineBuffer[i][j] != ' '; j++) {
      printf("%c", lineBuffer[i][j]);
    }
    printf("%c", ' ');
  }

  free(lineBuffer);
  // Exit the program
  exit(0);
}