#include "Simplex.h"

#define IN 1
#define OUT 0
#define CONF -1

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
	t->m_afficher = 1;
	return t;
}


Simplex* newSimplexWithPoint(Vertex* p1, Vertex* p2, Vertex* p3)
{
	Simplex* t = (Simplex*) malloc(sizeof(Simplex));
	ajoutPointsSimplex(t, p1, p2, p3);
	t->m_afficher = 1;
	return t;
}


void ajoutPointsSimplex(Simplex* t, Vertex* p1, Vertex* p2, Vertex* p3)
{
	t->m_afficher = 1;
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
	
	resolutionEquation(&(t->m_equation_plan), t->m_tab_points[0], t->m_tab_points[1], t->m_tab_points[2]);
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
int estDansSimplex(Simplex* t1, Vertex *pt1)
{
	Simplex t;
	float temp_pt_z;
	float temp_pt0_z;
	float temp_pt1_z;
	float temp_pt2_z;

	t.m_tab_points[0] = t1->m_tab_points[0];
	temp_pt0_z = t.m_tab_points[0]->coords[2];
	t.m_tab_points[0]->coords[2] = 0.0;

	t.m_tab_points[1] = t1->m_tab_points[1];
	temp_pt1_z = t.m_tab_points[1]->coords[2];
	t.m_tab_points[1]->coords[2] = 0.0;
	
	t.m_tab_points[2] = t1->m_tab_points[2];
	temp_pt2_z = t.m_tab_points[2]->coords[2];
	t.m_tab_points[2]->coords[2] = 0.0;
	
	Vertex pt = *pt1;
	temp_pt_z = pt.coords[2];
	pt.coords[2] = 0.0; 

    int orientationSimplex;
    orientationSimplex = orientationPolaire(t.m_tab_points[0],t.m_tab_points[1],t.m_tab_points[2]);
    if(	orientationPolaire(t.m_tab_points[0],t.m_tab_points[1],&pt) == orientationSimplex || 
    	orientationPolaire(t.m_tab_points[0],t.m_tab_points[1],&pt) == ALIGNE)
    {
		if(	orientationPolaire(t.m_tab_points[1],t.m_tab_points[2],&pt) == orientationSimplex || 
			orientationPolaire(t.m_tab_points[1],t.m_tab_points[2],&pt) == ALIGNE)
		{
			if(	orientationPolaire(t.m_tab_points[2],t.m_tab_points[0],&pt) == orientationSimplex || 
				orientationPolaire(t.m_tab_points[2],t.m_tab_points[0],&pt) == ALIGNE){
				if(	orientationPolaire(t.m_tab_points[0],t.m_tab_points[1],&pt) == ALIGNE || 
					orientationPolaire(t.m_tab_points[1],t.m_tab_points[2],&pt) == ALIGNE ||
				   	orientationPolaire(t.m_tab_points[2],t.m_tab_points[0],&pt) == ALIGNE)
				{
					pt.coords[2] = temp_pt_z;
					t.m_tab_points[0]->coords[2] = temp_pt0_z;
					t.m_tab_points[1]->coords[2] = temp_pt1_z;
					t.m_tab_points[2]->coords[2] = temp_pt2_z;
					return CONF;
				}
				pt.coords[2] = temp_pt_z;
				t.m_tab_points[0]->coords[2] = temp_pt0_z;
				t.m_tab_points[1]->coords[2] = temp_pt1_z;
				t.m_tab_points[2]->coords[2] = temp_pt2_z;
				return IN;
			}
		}
    }	
	pt.coords[2] = temp_pt_z;
	t.m_tab_points[0]->coords[2] = temp_pt0_z;
	t.m_tab_points[1]->coords[2] = temp_pt1_z;
	t.m_tab_points[2]->coords[2] = temp_pt2_z;
    return OUT;
}


void ajoutPointCandidatQueue(Simplex* t, Vertex *pt)
{
	lstAdd(t->m_list_candidats, pt);
}

void ajoutPointCandidatTete(Simplex* t, Vertex *pt)
{
	lstAddAtBeginning(t->m_list_candidats, pt);
}

void reattributionPoints2Simplex(Simplex *t1, Simplex* t2, List *l)
{
	int i;
	double hauteur;
	Node *n_temp = l->First;
	Vertex *v_temp = NULL;
	Simplex *simplex_choisi = NULL;

	for(i = 0; i < l->Count; i++)
	{
		v_temp = nodeGetData(n_temp);
		//Si le point est dans le premier simplex
		if(estDansSimplex(t1, v_temp) == IN)
		{
			simplex_choisi = t1;
		}
		//Si le point est dans le deuxieme simplex
		else if(estDansSimplex(t2, v_temp) == IN)
		{
			simplex_choisi = t2;
		}

		if(simplex_choisi != NULL)
		{
			hauteur = calculHauteur(simplex_choisi,v_temp);
			if(hauteur > simplex_choisi->m_hauteur || simplex_choisi->m_hauteur == 0.0) //Si la hauteur du point est superieur
			{
				//ajoute en tete
				ajoutPointCandidatTete(simplex_choisi, v_temp);
				simplex_choisi->m_hauteur = hauteur;
			}
			else
			{
				//ajoute en queue
				ajoutPointCandidatQueue(simplex_choisi, v_temp);
			}
		}
		simplex_choisi = NULL;
		n_temp = nodeGetNext(n_temp);
	}	
}


void reattributionPoints3Simplex(Simplex *t1, Simplex* t2, Simplex* t3, List *l)
{
	int i;
	double hauteur;
	Node *n_temp = l->First;
	Vertex *v_temp = NULL;
	Simplex *simplex_choisi = NULL;
	for(i = 0; i < l->Count; i++)
	{
		v_temp = nodeGetData(n_temp);

		//Si le point est dans le premier simplex
		if(estDansSimplex(t1, v_temp) == IN)
		{
			simplex_choisi = t1;
		}
		//Si le point est dans le deuxieme simplex
		else if(estDansSimplex(t2, v_temp) == IN)
		{
			simplex_choisi = t2;
		}
		//Si le point est dans le troisieme simplex
		else if(estDansSimplex(t3, v_temp) == IN)
		{
			simplex_choisi = t3;
		}

		if(simplex_choisi != NULL)
		{
			hauteur = calculHauteur(simplex_choisi,v_temp);
			if(hauteur > simplex_choisi->m_hauteur || simplex_choisi->m_hauteur == 0.0) //Si la hauteur du point est superieur
			{
				//ajoute en tete
				ajoutPointCandidatTete(simplex_choisi, v_temp);
				simplex_choisi->m_hauteur = hauteur;
			}
			else
			{
				//ajoute en queue
				ajoutPointCandidatQueue(simplex_choisi, v_temp);
			}
		}
		simplex_choisi = NULL;
		n_temp = nodeGetNext(n_temp);
	}	
}

double calculHauteur(Simplex *s, Vertex *pt)
{
	//calcul la hauteur du point par rapport au triangle
	double hauteur;
	double z;
	z = (double)(- (s->m_equation_plan.m_a * pt->coords[0]
		 		+ s->m_equation_plan.m_b * pt->coords[1]
		 		+ s->m_equation_plan.m_d)) / s->m_equation_plan.m_c;y-d)/c

	hauteur = pt->coords[2] - z; //Soustraction de la coordonnée

	return hauteur;
}


void affichageSimplex(Simplex * s)
{
	affichageVertex(s->m_tab_points[0]);
	affichageVertex(s->m_tab_points[1]);
	affichageVertex(s->m_tab_points[2]);
}
/**
 * \fn randomColor();
 *
 * \brief Fonction renvoyant une valeur : 0 ou 1.
 *
 * \return flottant égal à 0 ou 1
 */
double randomColor()
{
	return rand() % (1 + 1);
}

void affichageSimplex2D(Simplex * s)
{
	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 1.0, 0.0);

	glVertex2f(s->m_tab_points[0]->coords[0], s->m_tab_points[0]->coords[1]);
	glVertex2f(s->m_tab_points[1]->coords[0], s->m_tab_points[1]->coords[1]);
	glVertex2f(s->m_tab_points[2]->coords[0], s->m_tab_points[2]->coords[1]);

	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(1.0, 1.0, 1.0);

	glVertex2f(s->m_tab_points[0]->coords[0], s->m_tab_points[0]->coords[1]);
	glVertex2f(s->m_tab_points[1]->coords[0], s->m_tab_points[1]->coords[1]);
	glVertex2f(s->m_tab_points[2]->coords[0], s->m_tab_points[2]->coords[1]);
	glVertex2f(s->m_tab_points[0]->coords[0], s->m_tab_points[0]->coords[1]);

	glEnd();
}

void affichageSimplex3DInterieur(Simplex * s)
{
	
	glBegin(GL_TRIANGLES);
	glVertex3f(s->m_tab_points[0]->coords[0], s->m_tab_points[0]->coords[1], s->m_tab_points[0]->coords[2]);
	glVertex3f(s->m_tab_points[1]->coords[0], s->m_tab_points[1]->coords[1], s->m_tab_points[1]->coords[2]);
	glVertex3f(s->m_tab_points[2]->coords[0], s->m_tab_points[2]->coords[1], s->m_tab_points[2]->coords[2]);

	glEnd();
}

void affichageSimplex3DLigne(Simplex * s)
{
	glBegin(GL_LINE_STRIP);
	glVertex3f(s->m_tab_points[0]->coords[0], s->m_tab_points[0]->coords[1], s->m_tab_points[0]->coords[2]);
	glVertex3f(s->m_tab_points[1]->coords[0], s->m_tab_points[1]->coords[1], s->m_tab_points[1]->coords[2]);
	glVertex3f(s->m_tab_points[2]->coords[0], s->m_tab_points[2]->coords[1], s->m_tab_points[2]->coords[2]);
	glVertex3f(s->m_tab_points[0]->coords[0], s->m_tab_points[0]->coords[1], s->m_tab_points[0]->coords[2]);
	glEnd();
}

float getHauteur(Simplex * s)
{
	return s->m_hauteur;
}

void copy(Simplex * s1, Simplex * s2)
{
	s1->m_tab_points[0] = s2->m_tab_points[0];
	s1->m_tab_points[1] = s2->m_tab_points[1];
	s1->m_tab_points[2] = s2->m_tab_points[2];
	s1->m_tab_voisins[0] = s2->m_tab_voisins[0];
	s1->m_tab_voisins[1] = s2->m_tab_voisins[1];
	s1->m_tab_voisins[2] = s2->m_tab_voisins[2];
	s1->m_list_candidats = s2->m_list_candidats;
	s1->m_hauteur = s2->m_hauteur;
	s1->m_afficher = s2->m_afficher;
	s1->m_equation_plan = s2->m_equation_plan;
}

void ajouteVoisin(Simplex * s, Simplex * s_voisin)
{
	int i;
	int j;
	int nb_diff = 0;
	if(s_voisin != NULL)
	{
		for(i = 0; i < 3; i++)
		{
			for(j = 0; j < 3; j++)
			{
				if(egalite(s->m_tab_points[i], s_voisin->m_tab_points[j]) == 0)
					nb_diff++;
			}
			if(nb_diff == 3)
				break;	
			nb_diff = 0;
		}
		s->m_tab_voisins[i] = s_voisin;
	}
}


int egaliteSimplex(Simplex * s1, Simplex * s2)
{
	if(egalite(s1->m_tab_points[0], s2->m_tab_points[0]) == 0)
		return 0;
	if(egalite(s1->m_tab_points[1], s2->m_tab_points[1]) == 0)
		return 0;
	if(egalite(s1->m_tab_points[2], s2->m_tab_points[2]) == 0)
		return 0;
	return 1;
}


int indiceDiff(Simplex * s1, Simplex * s2)
{
	int i;
	int j;
	int nb_diff = 0;
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			if(egalite(s1->m_tab_points[i], s2->m_tab_points[j]) == 0)
				nb_diff++;
		}
		if(nb_diff == 3)
			return i;
		nb_diff = 0;
	}
	return -1;
}

int indicePosition(Simplex * s, Vertex * v)
{
	int i;
	for(i = 0; i < 3; i++)
	{
		if(egalite(s->m_tab_points[i], v) == 1)
			return i; 
	}
	return -1;
}
