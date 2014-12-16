#ifndef _TOOL_H_
#define _TOOL_H_

#include <stdlib.h>

#define ALLOUER(X,NB) do if ( (X = malloc(sizeof(*(X)) * (NB))) == 0 )\
							{ fprintf(stderr, "Plus de memoire\n") ; \
							 exit(1); } \
					while(0)

#endif