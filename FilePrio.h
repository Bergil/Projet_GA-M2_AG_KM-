#ifndef __FP_H__
#define __FP_H__

#define EGAL_HAUTEUR 0
#define SUP_HAUTEUR 1
#define INF_HAUTEUR -1

#define TAILLE_MAX 10000

#include <stdio.h>
#include <stdlib.h>
#include "Simplex.h"


typedef struct
{
  Simplex** tableau_Simplex;
  int nbSimplex;
} fdp;

////////////////////////////// FILE DE PRIORITE //////////////////////////////////////////////////////

fdp * allouerFDP(int nb_element);

void initFDP(fdp * f, int nb_element);

int ordreHauteur(Simplex *a, Simplex *b);

void upHeap(fdp *f, int position);

void downHeap(fdp *f);

void insertSimplex(fdp *f, Simplex *s);

Simplex* getTete(fdp *f);

void creationPointsFDP(fdp *f);

void affichageHauteurFDP(fdp * f);

void cleanFDP(fdp *f);

void destructionFDP(fdp * f);

void supprElement(fdp * f, Simplex * s);

void affichageFDP(fdp * f);
#endif

