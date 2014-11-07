#pragma once
#include <stdio.h>
#include <stdlib.h>
#define DIM 3
#define ADROITE 1
#define ALIGNE 0
#define AGAUCHE -1


typedef struct s_vertex
{
  double coords[DIM];
} vertex;

double determinant(const vertex *A, const vertex *B, const vertex *C);

//Retourne l'orientation entre 3 points (pour le 1er quadrant)
int orientationPolaire(const vertex *A, const vertex *B, const vertex *C);

//Teste si le point est compris dans un triangle
int estDansTriangle(vertex *a, vertex *b, vertex *c, vertex *pt);

int estAuDessusX(vertex* a, vertex *b);

int estAuDessusY(vertex* a, vertex* b);

int estAuDessusZ(vertex* a, vertex* b);

int egalite(vertex* a, vertex* b);

double calculLongueur(vertex* a, vertex* b);

void affichageVertex(vertex * v);
