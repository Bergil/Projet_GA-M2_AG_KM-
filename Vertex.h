 
#pragma once
#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h> 
#include "Tool.h"

#define DIM 3
#define ADROITE 1
#define ALIGNE 0
#define AGAUCHE -1

typedef struct s_vertex
{
	double coords[DIM];
} Vertex;

Vertex * newVertex();
Vertex * newVertexWithCoords(double x, double y, double z);

double determinant(const Vertex *A, const Vertex *B, const Vertex *C);
//Retourne l'orientation entre 3 points (pour le 1er quadrant)
int orientationPolaire(const Vertex *A, const Vertex *B, const Vertex *C);
//Teste si le point est compris dans un triangle
//int estDansTriangle(Vertex *a, Vertex *b, Vertex *c, Vertex *pt);

int estAuDessusX(Vertex* a, Vertex *b);

int estAuDessusY(Vertex* a, Vertex* b);

int estAuDessusZ(Vertex* a, Vertex* b);

int egalite(Vertex* a, Vertex* b);

double calculLongueur(Vertex* a, Vertex* b);

void affichageVertex(Vertex * v);

void affichageVertex2D(Vertex * v);

void affichageVertex3D(Vertex * v);

Vertex * minLexico(Vertex *a, Vertex *b, Vertex *c);
