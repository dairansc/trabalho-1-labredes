#ifndef _Socket_
#define _Socket_

#include <stdlib.h> /* for exit() */

/* Error handling function */
void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

#endif

