 
#include "Vertex.h"
#include <math.h>
					
Vertex * newVertex()
{
	Vertex* v = (Vertex *) malloc(sizeof(Vertex));
	int i;
	for(i = 0; i < DIM; i++)
		v->coords[i] = 0;
	return v;
}

Vertex * newVertexWithCoords(double x, double y, double z)
{
	Vertex* v = (Vertex *) malloc(sizeof(Vertex));
	v->coords[0] = x;
	v->coords[1] = y;
	v->coords[2] = y;
	return v;
}

double determinant(const Vertex *A, const Vertex *B, const Vertex *C)
{
	return (B->coords[0]-A->coords[0]) * (C->coords[1]-A->coords[1]) - (B->coords[1]-A->coords[1]) * (C->coords[0]-A->coords[0]);
}

int orientationPolaire(const Vertex *A, const Vertex *B, const Vertex *C)
{
	double det = determinant(A, B, C);
	if(det < 0) return ADROITE;
	else if(det > 0) return AGAUCHE;
	else return ALIGNE;
}

int egalite(Vertex* a, Vertex* b){
	if(a->coords[0] != b->coords[0])
		return 0;
	else if(a->coords[1] != b->coords[1])
		return 0;
	else if(a->coords[2] != b->coords[2])
		return 0;
	return 1;
}

Vertex * minLexico(Vertex *a, Vertex *b, Vertex *c)
{
	int i;
	Vertex * temp;
	Vertex * res = a;
	ALLOUER(temp, 3);
	temp[0] = *a;
	temp[1] = *b;
	temp[2] = *c;
	for(i = 0; i < 3; i++)
	{
		if(temp[i].coords[0] < res->coords[0])
		{
			res = &temp[i];
		}
		else if (res->coords[0] == temp[i].coords[0])
		{
			if (temp[i].coords[1] < res->coords[1])
				res = &temp[i];
		}
	}
	return res;
}

void affichageVertex(Vertex * v)
{
	fprintf(stderr, "Point : %f %f %f\n", v->coords[0], v->coords[1], v->coords[2]);
}

void affichageVertex2D(Vertex * v)
{
	glBegin(GL_POINTS);
	
	glColor3f(0.0, 1.0, 0.0);
	
	//fprintf(stderr, "Je viens la \n");
	glVertex2f(v->coords[0], v->coords[1]);
	//fprintf(stderr, "Je sors la \n");

	glEnd();
}