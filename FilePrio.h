#ifndef __FP_H__
#define __FP_H__

#define EGAL_LEXICO 0
#define SUP_LEXICO 1
#define INF_LEXICO -1

#define TAILLE_MAX 10000

#include <stdio.h>
#include <stdlib.h>
#include "Vertex.h"


typedef struct
{
  vertex** tableau_vertex;
  int nbPoints;
} fdp;

////////////////////////////// FILE DE PRIORITE //////////////////////////////////////////////////////

void initFDP(fdp * f);

int ordreLexico(vertex *a, vertex *b);

void upHeap(fdp *f, int position);

void downHeap(fdp *f);

void insertionLexico(fdp *f, vertex *pt);

vertex* suppressionLexico(fdp *f);

void remplissageListeParTab(fdp *f, vertex * T, int nbPoints);

void creationPointsFDP(fdp *f);

fdp * allouerFDP();

void cleanFDP(fdp *f);

void destructionFDP(fdp * f);

void affichageFDP(fdp * f);

#endif
