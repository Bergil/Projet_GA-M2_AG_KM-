#ifndef _PILE_H_
#define _PILE_H_

#include <stdlib.h>

typedef struct s_pile
{
	Node* First;
	Node* Last;
	int m_nb_elements;	
} Pile;

#endif