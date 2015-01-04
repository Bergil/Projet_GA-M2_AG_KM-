#include "FilePrio.h"
#include <math.h>


////////////////////////////// FILE DE PRIORITE //////////////////////////////////////////////////////

void initFDP(fdp * f, int taille){
	ALLOUER(f->tableau_Simplex, taille);
	f->nbSimplex = 0;
}

void upHeap(fdp *f, int position){
	Simplex* tmp;
	int pospere = position /2;
	while ((position>1) && (ordreHauteur(f->tableau_Simplex[position], f->tableau_Simplex[pospere]) == INF_HAUTEUR))
	{
		tmp = f->tableau_Simplex[pospere];
		f->tableau_Simplex[pospere] = f->tableau_Simplex[position];
		f->tableau_Simplex[position] = tmp;
		position = pospere;
		pospere = position/2;		
	}
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

void downHeapPos(fdp *f, int position)
{
	Simplex* temp;
	int posfilsgauche = position*2;
	int posfilsdroit = position*2+1;
	int marqueurPosition;
	if(posfilsgauche <= f->nbSimplex && posfilsdroit <= f->nbSimplex+1 && position > 0)
	{
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
			if(posfilsgauche > f->nbSimplex || posfilsdroit > f->nbSimplex)
				break;
		}
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
		//On le remonte
		upHeap(f, f->nbSimplex);
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

fdp * allouerFDP(int taille)
{
  fdp* f;
  ALLOUER(f, 1);
  initFDP(f, taille);
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
	for(i = 0; i < f->nbSimplex; i++)
	{
		fprintf(stderr, "pt: %d  ", i);
		fprintf(stderr, "*** Aff *** hauteur: %f\n", getHauteur(f->tableau_Simplex[i]));
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



int positionDansFDP(fdp * f, Simplex *s)
{
	int i;
	for(i = 1; i<f->nbSimplex; i++)
	{
		if(egaliteSimplex(s, f->tableau_Simplex[i]) == 1)
			return i;
	}
	return -1;
}
