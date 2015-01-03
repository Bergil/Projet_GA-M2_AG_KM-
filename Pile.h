#ifndef _PILE_H_
#define _PILE_H_

#include <stdlib.h>
#include "Simplex.h"
#include "Tool.h"

typedef struct s_pile
{
	Simplex ** m_tab_simplex;
	int m_nb_elements;	
} Pile;

Pile * creationPile(int nb_elements);
int estDans(Pile * p, Simplex * s);
void empiler(Pile * p, Simplex * s);
Simplex * depiler(Pile * p);
void affichagePile(Pile * p);

#endif