#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int frobcmp (const char *str1, const char *str2);

int frobcmp (const char *str1, const char *str2)
{
  register const unsigned char *s1 = (const unsigned char*)str1;
  register const unsigned char *s2 = (const unsigned char*)str2;

  for (; *s1 == *s2; s1++, s2++)
  {
    if (*s1 == ' ') {
      return 0;
    }
    return ((*s1 ^ 42) < (*s2 ^ 42)) ? -1 : 1;
  }
}
