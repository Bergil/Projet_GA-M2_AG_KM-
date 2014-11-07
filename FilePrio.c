#include "FilePrio.h"
#include <math.h>



#define ALLOUER(X,NB) do if ( (X = malloc(sizeof(*(X)) * (NB))) == 0 )\
                             { fprintf(stderr, "Plus de memoire\n") ; \
                                exit(1); } \
                      while(0)


////////////////////////////// FILE DE PRIORITE //////////////////////////////////////////////////////

void initFDP(fdp * f){
	ALLOUER(f->tableau_vertex, TAILLE_MAX);
	f->nbPoints = 0;
}

int ordreLexico(vertex *a, vertex *b) //en fonction de Z puis de Y puis de X
{
	int i;
	for (i = DIM; i >0; --i)
	{
		if(a->coords[i] == b->coords[i])
			break;
		else if (a->coords[i] < b->coords[i])
			return INF_LEXICO;	
		else 
			return SUP_LEXICO;
	}
	return EGAL_LEXICO;
}

void remplissageListeParTab(fdp *f, vertex * T, int nbPoints){
	int i;
	for(i =0; i < nbPoints; i++){
		insertionLexico(f, &T[i]);
	}
}

void upHeap(fdp *f, int position){
	vertex* tmp;
	int pospere = position /2;
	while ((position>1) && (ordreLexico(f->tableau_vertex[position], f->tableau_vertex[pospere]) == INF_LEXICO))
	{
		tmp = f->tableau_vertex[pospere];
		f->tableau_vertex[pospere] = f->tableau_vertex[position];
		f->tableau_vertex[position] = tmp;
		position = pospere;
		pospere = position/2;		
	}
		
}

void downHeap(fdp *f)
{
	vertex* temp;
	int position = 1;
	int posfilsgauche = position*2;
	int posfilsdroit = position*2+1;
	int marqueurPosition;
	
	while(posfilsgauche <= f->nbPoints && posfilsdroit <= f->nbPoints+1 &&
		 (ordreLexico(f->tableau_vertex[position], f->tableau_vertex[posfilsgauche]) == SUP_LEXICO ||
		  ordreLexico(f->tableau_vertex[position], f->tableau_vertex[posfilsdroit]) == SUP_LEXICO))
	{	
		if(ordreLexico(f->tableau_vertex[posfilsgauche], f->tableau_vertex[posfilsdroit]) == SUP_LEXICO)
		{
			marqueurPosition = posfilsdroit;
		}else{
			marqueurPosition = posfilsgauche;
		}
		temp = f->tableau_vertex[marqueurPosition];
		f->tableau_vertex[marqueurPosition] = f->tableau_vertex[position];
		f->tableau_vertex[position] = temp;
		
		position = marqueurPosition;
		posfilsgauche = position*2;
		posfilsdroit = position*2+1;	
	}
}

void insertionLexico(fdp *f, vertex *pt)
{
	if(f->nbPoints < TAILLE_MAX)
	{		
		f->nbPoints = f->nbPoints + 1;	
		f->tableau_vertex[f->nbPoints] = pt;
		upHeap(f, f->nbPoints);
	}	

}

vertex* suppressionLexico(fdp *f)
{
	vertex* pt;
	pt = f->tableau_vertex[1];
	f->tableau_vertex[1] = f->tableau_vertex[f->nbPoints];
	f->nbPoints = f->nbPoints-1;

	downHeap(f);
	return pt;
}

fdp * allouerFDP()
{
  fdp* f;
  ALLOUER(f, 1);
  initFDP(f);
  return f;
}

void cleanFDP(fdp *f)
{
  free(f->tableau_vertex);
  f->nbPoints = 0;
}

void destructionFDP(fdp * f)
{
  cleanFDP(f);
  free(f);
}


void affichageFDP(fdp * f)
{
	int i;
	for(i = 0; i < f->nbPoints; i++)
	{
		fprintf(stderr, "pt %d: ", i);
		affichageVertex(f->tableau_vertex[i]);
		//fprintf(stderr, "  %f - %f \n", f->tableau_vertex[i]->coords[0], f->tableau_vertex[i]->coords[1]);
	}
}
