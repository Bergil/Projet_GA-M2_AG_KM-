#include "FilePrio.h"
#include <math.h>



#define ALLOUER(X,NB) do if ( (X = malloc(sizeof(*(X)) * (NB))) == 0 )\
                             { fprintf(stderr, "Plus de memoire\n") ; \
                                exit(1); } \
                      while(0)


////////////////////////////// FILE DE PRIORITE //////////////////////////////////////////////////////

void initFDP(fdp * f){
	ALLOUER(f->tableau_Vertex, TAILLE_MAX);
	f->nbPoints = 0;
}

int ordreLexico(Vertex *a, Vertex *b) //en fonction de Z puis de Y puis de X
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

void remplissageListeParTab(fdp *f, Vertex * T, int nbPoints){
	int i;
	for(i =0; i < nbPoints; i++){
		insertionLexico(f, &T[i]);
	}
}

void upHeap(fdp *f, int position){
	Vertex* tmp;
	int pospere = position /2;
	while ((position>1) && (ordreLexico(f->tableau_Vertex[position], f->tableau_Vertex[pospere]) == INF_LEXICO))
	{
		tmp = f->tableau_Vertex[pospere];
		f->tableau_Vertex[pospere] = f->tableau_Vertex[position];
		f->tableau_Vertex[position] = tmp;
		position = pospere;
		pospere = position/2;		
	}
		
}

void downHeap(fdp *f)
{
	Vertex* temp;
	int position = 1;
	int posfilsgauche = position*2;
	int posfilsdroit = position*2+1;
	int marqueurPosition;
	
	while(posfilsgauche <= f->nbPoints && posfilsdroit <= f->nbPoints+1 &&
		 (ordreLexico(f->tableau_Vertex[position], f->tableau_Vertex[posfilsgauche]) == SUP_LEXICO ||
		  ordreLexico(f->tableau_Vertex[position], f->tableau_Vertex[posfilsdroit]) == SUP_LEXICO))
	{	
		if(ordreLexico(f->tableau_Vertex[posfilsgauche], f->tableau_Vertex[posfilsdroit]) == SUP_LEXICO)
		{
			marqueurPosition = posfilsdroit;
		}else{
			marqueurPosition = posfilsgauche;
		}
		temp = f->tableau_Vertex[marqueurPosition];
		f->tableau_Vertex[marqueurPosition] = f->tableau_Vertex[position];
		f->tableau_Vertex[position] = temp;
		
		position = marqueurPosition;
		posfilsgauche = position*2;
		posfilsdroit = position*2+1;	
	}
}

void insertionLexico(fdp *f, Vertex *pt)
{
	if(f->nbPoints < TAILLE_MAX)
	{		
		f->nbPoints = f->nbPoints + 1;	
		f->tableau_Vertex[f->nbPoints] = pt;
		upHeap(f, f->nbPoints);
	}	

}

Vertex* suppressionLexico(fdp *f)
{
	Vertex* pt;
	pt = f->tableau_Vertex[1];
	f->tableau_Vertex[1] = f->tableau_Vertex[f->nbPoints];
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
  free(f->tableau_Vertex);
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
		affichageVertex(f->tableau_Vertex[i]);
		//fprintf(stderr, "  %f - %f \n", f->tableau_Vertex[i]->coords[0], f->tableau_Vertex[i]->coords[1]);
	}
}
