#include "FilePrio.h"
#include <math.h>


////////////////////////////// FILE DE PRIORITE //////////////////////////////////////////////////////

void initFDP(fdp * f, int nb_element){
	ALLOUER(f->tableau_Simplex, nb_element);
	f->nbSimplex = 0;
}

/*void remplissageListeParTab(fdp *f, Simplex * T, int nbSimplex){
	int i;
	for(i =0; i < nbSimplex; i++){
		insertSimplex(f, &T[i]);
	}
}*/

int ordreHauteur(Simplex *a, Simplex *b)
{
	if(a->m_hauteur < b->m_hauteur)
		return INF_HAUTEUR;
	else if(a->m_hauteur > b->m_hauteur)
		return SUP_HAUTEUR;
	else
		return EGAL_HAUTEUR;
}

void upHeap(fdp *f, int position){
	Simplex* tmp;
	int pospere = position /2;
	while ((position>1) && (ordreHauteur(f->tableau_Simplex[position], f->tableau_Simplex[pospere]) == INF_HAUTEUR))
	{
		//fprintf(stderr, "Dans le while de l'insertion\n");
		tmp = f->tableau_Simplex[pospere];
		f->tableau_Simplex[pospere] = f->tableau_Simplex[position];
		f->tableau_Simplex[position] = tmp;
		position = pospere;
		pospere = position/2;		
	}
	//fprintf(stderr, "position: %d\n", position);
}

void downHeap(fdp *f)
{
	Simplex* temp;
	int position = 1;
	int posfilsgauche = position*2;
	int posfilsdroit = position*2+1;
	int marqueurPosition;
	
	while(posfilsgauche <= f->nbSimplex && posfilsdroit <= f->nbSimplex+1 &&
		 (ordreHauteur(f->tableau_Simplex[position], f->tableau_Simplex[posfilsgauche]) == SUP_HAUTEUR ||
		  ordreHauteur(f->tableau_Simplex[position], f->tableau_Simplex[posfilsdroit]) == SUP_HAUTEUR))
	{	
		if(ordreHauteur(f->tableau_Simplex[posfilsgauche], f->tableau_Simplex[posfilsdroit]) == SUP_HAUTEUR)
		{
			marqueurPosition = posfilsdroit;
		}else{
			marqueurPosition = posfilsgauche;
		}
		temp = f->tableau_Simplex[marqueurPosition];
		f->tableau_Simplex[marqueurPosition] = f->tableau_Simplex[position];
		f->tableau_Simplex[position] = temp;
		
		position = marqueurPosition;
		posfilsgauche = position*2;
		posfilsdroit = position*2+1;	
	}
}

void insertSimplex(fdp *f, Simplex *s)
{
	if(f->nbSimplex < TAILLE_MAX)
	{		
		//Augmente la taille de 1 pour commencer a 1
		f->nbSimplex = f->nbSimplex + 1;
		//Ajoute le simplex au tableau	
		f->tableau_Simplex[f->nbSimplex] = s;
		//fprintf(stderr, "*** insert *** hauteur: %f\n", getHauteur(s));
		//On le remonte
		upHeap(f, f->nbSimplex);
		//f->nbSimplex = f->nbSimplex + 1;
	}	

}

Simplex* getTete(fdp *f)
{
	Simplex* s;
	s = f->tableau_Simplex[1];
	f->tableau_Simplex[1] = f->tableau_Simplex[f->nbSimplex];
	f->nbSimplex = f->nbSimplex-1;
	downHeap(f);
	if(s == NULL)
		fprintf(stderr, "s NULL\n");
	return s;
}

fdp * allouerFDP(int nb_element)
{
  fdp* f;
  ALLOUER(f, 1);
  initFDP(f, nb_element);
  return f;
}

void cleanFDP(fdp *f)
{
  free(f->tableau_Simplex);
  f->nbSimplex = 0;
}

void destructionFDP(fdp * f)
{
  cleanFDP(f);
  free(f);
}


void affichageHauteurFDP(fdp * f)
{
	int i;
	//fprintf(stderr, "Nb simplex: %d\n", f->nbSimplex);
	for(i = 0; i < f->nbSimplex; i++)
	{
		fprintf(stderr, "pt: %d  ", i);
		fprintf(stderr, "*** Aff *** hauteur: %f\n", getHauteur(f->tableau_Simplex[i]));
		//fprintf(stderr, "  %f - %f \n", f->tableau_Simplex[i]->coords[0], f->tableau_Simplex[i]->coords[1]);
	}
}



void supprElement(fdp * f, Simplex * s)
{
	int i;
	for(i = 1; i <= f->nbSimplex; i++)
	{
		if(egaliteSimplex(f->tableau_Simplex[i], s) == 1)
			break;
	}
	while(i*2+1 <= f->nbSimplex)
	{
		if(i*2+1 == f->nbSimplex+1)
		{
			f->tableau_Simplex[i] = f->tableau_Simplex[i*2];
			i = i*2;
		}
		else if(f->tableau_Simplex[i*2+1]->m_hauteur >= f->tableau_Simplex[i*2]->m_hauteur)
		{
			f->tableau_Simplex[i] = f->tableau_Simplex[i*2+1];
			i = i*2+1;
		}
		else
		{
			f->tableau_Simplex[i] = f->tableau_Simplex[i*2];
			i = i*2;
		}
	}
	f->nbSimplex--;
}




void affichageFDP(fdp * f)
{
	int i;
	for(i = 0; i<f->nbSimplex +10; i++)
	{
		fprintf(stderr, "i: %d\n", i);
		if(f->tableau_Simplex[i] != NULL)
			affichageSimplex(f->tableau_Simplex[i]);
	}
}