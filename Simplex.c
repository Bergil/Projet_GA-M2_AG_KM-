#include "Simplex.h"

Simplex* newSimplex()
{
	Simplex* t = (Simplex*) malloc(sizeof(Simplex));

	t->m_tab_points[0] = NULL;
	t->m_tab_points[1] = NULL;
	t->m_tab_points[2] = NULL;

	t->m_tab_voisins[0] = NULL;
	t->m_tab_voisins[1] = NULL;
	t->m_tab_voisins[2] = NULL;

	t->m_list_candidats = newList();
	return t;
}


Simplex* newSimplexWithPoint(Vertex* p1, Vertex* p2, Vertex* p3)
{
	Simplex* t = (Simplex*) malloc(sizeof(Simplex));
	
	t->m_tab_points[0] = minLexico(p1,p2,p3);

	if(egalite(t->m_tab_points[0], p1))
	{
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

	t->m_list_candidats = newList();
	
	return t;
}


void deleteAllSimplex(Simplex* t)
{
	free(t->m_tab_points[0]);
	free(t->m_tab_points[1]);
	free(t->m_tab_points[2]);

	free(t->m_tab_voisins[0]);
	free(t->m_tab_voisins[1]);
	free(t->m_tab_voisins[2]);

	free(t->m_list_candidats);
}


void deleteSimplex(Simplex* t)
{
	deleteAllSimplex(t);
	free(t);
}



//Teste si le point est compris dans un Simplex
int estDansSimplex(Simplex* t, Vertex *pt)
{
    int orientationSimplex;
    orientationSimplex = orientationPolaire(t->m_tab_points[0],t->m_tab_points[1],t->m_tab_points[2]);
    if(orientationPolaire(t->m_tab_points[0],t->m_tab_points[1],pt) == orientationSimplex || orientationPolaire(t->m_tab_points[0],t->m_tab_points[1],pt) == ALIGNE)
    {
		if(orientationPolaire(t->m_tab_points[1],t->m_tab_points[2],pt) == orientationSimplex || orientationPolaire(t->m_tab_points[1],t->m_tab_points[2],pt) == ALIGNE)
		{
			if(orientationPolaire(t->m_tab_points[2],t->m_tab_points[0],pt) == orientationSimplex || orientationPolaire(t->m_tab_points[2],t->m_tab_points[0],pt) == ALIGNE){
				if(	orientationPolaire(t->m_tab_points[0],t->m_tab_points[1],pt) == ALIGNE || 
					orientationPolaire(t->m_tab_points[1],t->m_tab_points[2],pt) == ALIGNE ||
				   	orientationPolaire(t->m_tab_points[2],t->m_tab_points[0],pt) == ALIGNE)
					return -1;
				return 1;
			}
		}
    }	
    return 0;
}


void ajoutPointCandidat(Simplex* t, Vertex *pt)
{
	lstAdd(t->m_list_candidats, pt);
}


/*void inversionNeccessaire(Simplex *t, Vertex *pt)
{
	
}*/



