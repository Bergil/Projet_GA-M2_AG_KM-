 
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

//Teste si le point est compris dans un triangle
/*int estDansTriangle(Vertex *a, Vertex *b, Vertex *c, Vertex *pt)
{
int orientationTriangle;
orientationTriangle = orientationPolaire(a,b,c);
if(orientationPolaire(a,b,pt) == orientationTriangle || orientationPolaire(a,b,pt) == ALIGNE)
{
if(orientationPolaire(b,c,pt) == orientationTriangle || orientationPolaire(b,c,pt) == ALIGNE)
{
if(orientationPolaire(c,a,pt) == orientationTriangle || orientationPolaire(c,a,pt) == ALIGNE)
{
if(orientationPolaire(a,b,pt) == ALIGNE ||
orientationPolaire(b,c,pt) == ALIGNE ||
orientationPolaire(c,a,pt) == ALIGNE)
return -1;
return 1;
}
}
}
return 0;
}*/

int estAuDessusX(Vertex* a, Vertex* b)
{
	if(a->coords[0] > b->coords[0])
		return 1;
	return 0;
}
int estAuDessusY(Vertex* a, Vertex* b)
{
	if(a->coords[1] > b->coords[1])
		return 1;
	return 0;
}

int estAuDessusZ(Vertex* a, Vertex* b)
{
	if(a->coords[2] > b->coords[2])
		return 1;
	return 0;
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

double calculLongueur(Vertex* a, Vertex* b)
{
	return sqrt((b->coords[0] - a->coords[0])*(b->coords[0] - a->coords[0]) +
	(b->coords[1] - a->coords[1])*(b->coords[1] - a->coords[1]) +
	(b->coords[2] - a->coords[2])*(b->coords[2] - a->coords[2]));
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

void affichageVertex3D(Vertex * v)
{
	glBegin(GL_POINTS);
	
	glColor3f(1.0, 0.0, 1.0);
	
	//fprintf(stderr, "Je viens la \n");
	glVertex3f(v->coords[0], v->coords[1], v->coords[2]);
	//fprintf(stderr, "Je sors la \n");

	glEnd();
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

/*
double calculAngle3points(Vertex* A, Vertex* B, Vertex* C)
{
double l1 = calculLongueur(B, C);
double l2 = calculLongueur(B, A);
Vertex v1;
v1.coords[0] = (C->coords[0] - B->coords[0]) / l1;
v1.coords[1] = (C->coords[1] - B->coords[1]) / l1;
Vertex v2;
v2.coords[0] = (A->coords[0] - B->coords[0]) / l2;
v2.coords[1] = (A->coords[1] - B->coords[1]) / l2;
return atan2(v2.coords[1], v2.coords[0]) - atan2(v1.coords[1], v1.coords[0]);
}
double detLexico(Vertex* A, Vertex* B, Vertex* C)
{
double l1 = calculLongueur(B, C);
double l2 = calculLongueur(A, B);
Vertex v1;
v1.coords[0] = (C->coords[0] - B->coords[0]) / l1;
v1.coords[1] = (C->coords[1] - B->coords[1]) / l1;
Vertex v2;
v2.coords[0] = (B->coords[0] - A->coords[0]) / l2;
v2.coords[1] = (B->coords[1] - A->coords[1]) / l2;
return atan2(v2.coords[1], v2.coords[0]) - atan2(v1.coords[1], v1.coords[0]);
}
double equation(Vertex* p1, Vertex* p2, Vertex* p3)//Le point testé est eventuellement le 3
{
double res = 0;
//[-(y1 - y0)] * px + [(x1 - x0)] * py + [x0 * (y1 - y0) - y0 * (x1 - x0)]
res = (-(p2->coords[1] - p1->coords[1]))* p3->coords[0] + ((p2->coords[0]-p1->coords[0])) * p3->coords[1] +
(p1->coords[0] * (p2->coords[1]-p1->coords[1]) - p1->coords[1] * (p2->coords[0] - p1->coords[0]));
return res;
}
int intersectionSegment(Vertex* s1p1,Vertex* s1p2,Vertex* s2p1,Vertex* s2p2)
{
double eqS11 = equation(s1p1, s1p2, s2p1);
double eqS12 = equation(s1p1, s1p2, s2p2);
double eqS21 = equation(s2p1, s2p2, s1p1);
double eqS22 = equation(s2p1, s2p2, s1p2);
if((eqS11 <= 0 && eqS12 >= 0) || (eqS11 >= 0 && eqS12 <= 0))
{
if((eqS21 <= 0 && eqS22 >= 0) || (eqS21 >= 0 && eqS22 <= 0))
{
return 1;
}
else
return 0;
}
else
return 0;
}
double calculAnglePolaire(Vertex * p, Vertex * q){ /// FAUX A REFAIRE SANS COS AVEC DET QUI CASSE LES COUILLES !!!
Vertex t;
t.coords[0] = p->coords[0]+1;
t.coords[1] = p->coords[1];
return calculAngle3points(&t, p, q);
}
*/
