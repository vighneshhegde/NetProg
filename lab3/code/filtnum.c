#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
void
err_sys (char *str)
{
  perror (str);
  exit (-1);
}

int
main (void)
{
  int c;

  while ((c = getchar ()) != EOF)
    {
      if (('0'<=c && c<='9')|| c=='\n'){
      	if (putchar (c) == EOF)
		err_sys ("output error");
	  }
      if (c == '\n')
	fflush (stdout);
    }

  exit (0);
}
