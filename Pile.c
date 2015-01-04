
#include "Pile.h"

Pile * creationPile(int taille)
{
	Pile * p;
	ALLOUER(p,1);
	ALLOUER(p->m_tab_simplex, taille);
	p->m_nb_elements = 0;
	return p;
}

int estDans(Pile * p, Simplex * s)
{
	int i;
	for(i = 0; i < p->m_nb_elements; i++)
	{
		if(egaliteSimplex(s, p->m_tab_simplex[i]))
			return 1;
	}
	return 0;
}

void empiler(Pile * p, Simplex * s)
{
	p->m_tab_simplex[p->m_nb_elements] = s;
	p->m_nb_elements++;
}

Simplex * depiler(Pile * p)
{
	int i;
	Simplex * s = p->m_tab_simplex[0];
	p->m_nb_elements--;
	for(i = 0; i < p->m_nb_elements; i++)
	{
		p->m_tab_simplex[i] = p->m_tab_simplex[i+1];
	}
	return s;
}

void affichagePile(Pile * p)
{
	fprintf(stderr, "*** Affichage pile ***\n");
	int i;
	fprintf(stderr, "nbElementPile: %d\n", p->m_nb_elements);
	for(i = 0; i < p->m_nb_elements; i++)
	{
		affichageSimplex(p->m_tab_simplex[i]);
		fprintf(stderr, "\n");
	}
}