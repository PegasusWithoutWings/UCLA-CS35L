#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <dlfcn.h>
#include <stdlib.h>
#include "randcpuid.h"

static bool
writebytes (unsigned long long x, int nbytes)
{
  int ndigits = nbytes * 2;
  do
    {
      if (putchar ("0123456789abcdef"[x & 0xf]) < 0)
	return false;
      x >>= 4;
      ndigits--;
    }
  while (0 < ndigits);

  return 0 <= putchar ('\n');
}

static void *
get_func(const char *func_name, void *dl_handle) {
  void (*myfunc)(int *);
  char *error;
  myfunc = dlsym(dl_handle, func_name);
  if ((error = dlerror()) != NULL) {
    printf ("dlsym %s error - %s\n", func_name, error);
    exit(1);
  }
  return myfunc;
}

static void *
Dl_open(const char *lib_name, int __mode) {
  void *result = dlopen(lib_name, __mode);
  if (!result) {
    printf("dlopen() error - %s\n", dlerror());
    exit(1);
  }
  return result;
}

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{

  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  void (*initialize) (void);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);
  void *dl_handle;
  if (rdrand_supported ())
    {
      dl_handle = Dl_open("randlibhw.so", RTLD_LAZY);
      initialize = get_func("hardware_rand64_init", dl_handle);
      rand64 = get_func("hardware_rand64", dl_handle);
      finalize = get_func("hardware_rand64_fini", dl_handle);
    }
  else
    {
      dl_handle = Dl_open("randlibsw.so", RTLD_LAZY);
      initialize = get_func("software_rand64_init", dl_handle);
      rand64 = get_func("software_rand64", dl_handle);
      finalize = get_func("software_rand64_fini", dl_handle);
    }

  initialize ();
  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  do
    {
      unsigned long long x = rand64 ();
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (!writebytes (x, outbytes))
	{
	  output_errno = errno;
	  break;
	}
      nbytes -= outbytes;
    }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
      finalize ();
      return 1;
    }

  finalize ();
  return 0;
}