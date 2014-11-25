#include "Triangle.h"

Triangle* newTriangle()
{
	Triangle* t = (Triangle*) malloc(sizeof(Triangle));

	t->m_tab_points[0] = NULL;
	t->m_tab_points[1] = NULL;
	t->m_tab_points[2] = NULL;

	t->m_tab_voisins[0] = NULL;
	t->m_tab_voisins[1] = NULL;
	t->m_tab_voisins[2] = NULL;

	//t->m_list_candidats = NULL;
	return t;
}


Triangle* newTriangleWithPoint(vertex* p1, vertex* p2, vertex* p3)
{
	Triangle* t = (Triangle*) malloc(sizeof(Triangle));
	
	t->m_tab_points[0] = minLexico(p1,p2,p3);
	fprintf(stderr, "min lexico: %f %f %f\n", t->m_tab_points[0]->coords[0], t->m_tab_points[0]->coords[1], t->m_tab_points[0]->coords[2]);

	if(egalite(t->m_tab_points[0], p1))
	{
		fprintf(stderr, "c'est p1 le premier: \n");
		if(orientationPolaire(p1,p2,p3) == AGAUCHE)
		{
			t->m_tab_points[1] = p2;
			t->m_tab_points[2] = p3; 
		}
		else
		{
			t->m_tab_points[1] = p3;
			t->m_tab_points[2] = p2; 
		}
	}
	else if(egalite(t->m_tab_points[0], p2))
	{
		fprintf(stderr, "c'est p2 le premier: \n");
		if(orientationPolaire(p2,p1,p3) == AGAUCHE)
		{
			t->m_tab_points[1] = p1;
			t->m_tab_points[2] = p3; 
		}
		else
		{
			t->m_tab_points[1] = p3;
			t->m_tab_points[2] = p1; 
		}
	}
	else
	{
		fprintf(stderr, "c'est p3 le premier: \n");
		if(orientationPolaire(p3,p1,p2) == AGAUCHE)
		{
			t->m_tab_points[1] = p1;
			t->m_tab_points[2] = p2; 
		}
		else
		{
			t->m_tab_points[1] = p2;
			t->m_tab_points[2] = p1; 
		}
	}

	t->m_tab_voisins[0] = NULL;
	t->m_tab_voisins[1] = NULL;
	t->m_tab_voisins[2] = NULL;

	//t->m_list_candidats = NULL;
	
	return t;
}


void deleteAllTriangle(Triangle* t)
{
	free(t->m_tab_points[0]);
	free(t->m_tab_points[1]);
	free(t->m_tab_points[2]);

	free(t->m_tab_voisins[0]);
	free(t->m_tab_voisins[1]);
	free(t->m_tab_voisins[2]);

	//free(t->m_list_candidats);
}


void deleteTriangle(Triangle* t)
{
	deleteAllTriangle(t);
	free(t);
}



//Teste si le point est compris dans un triangle
int estDansTriangle(Triangle* t, vertex *pt)
{
    int orientationTriangle;
    orientationTriangle = orientationPolaire(t->m_tab_points[0],t->m_tab_points[1],t->m_tab_points[2]);
    if(orientationPolaire(t->m_tab_points[0],t->m_tab_points[1],pt) == orientationTriangle || orientationPolaire(t->m_tab_points[0],t->m_tab_points[1],pt) == ALIGNE)
    {
		if(orientationPolaire(t->m_tab_points[1],t->m_tab_points[2],pt) == orientationTriangle || orientationPolaire(t->m_tab_points[1],t->m_tab_points[2],pt) == ALIGNE)
		{
			if(orientationPolaire(t->m_tab_points[2],t->m_tab_points[0],pt) == orientationTriangle || orientationPolaire(t->m_tab_points[2],t->m_tab_points[0],pt) == ALIGNE){
				if(orientationPolaire(t->m_tab_points[0],t->m_tab_points[1],pt) == ALIGNE || orientationPolaire(t->m_tab_points[1],t->m_tab_points[2],pt) == ALIGNE ||
				   orientationPolaire(t->m_tab_points[2],t->m_tab_points[0],pt) == ALIGNE)
					return -1;
				return 1;
			}
		}
    }	
    return 0;
}
