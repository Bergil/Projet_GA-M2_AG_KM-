#include <assert.h>  
#include <unistd.h>  
#include <GL/glut.h> 
#include <time.h>

#include "FilePrio.h"
#include "Pile.h"
#include "Simplex.h"
#include "geom.h"

/*! variable externe permettant de lire les parametres sur le ligne de commande.*/
extern char *optarg;

/*! bascule pour autoriser ou interdire (0) les messages d'erreurs envoyes par getopt. */
extern int opterr;

/*! Tailles diverses du systemes de fenetrage .*/
const double minX = 0,
             maxX = 1,
			 minY = 0,
			 maxY = 1,
			 minZ = 0/*-500*/,
			 maxZ = 1/*500*/,
			 margin = 10;

/*! Nombre de "sites" dans le programme, modifiable par la ligne
 * de commande de la fonction main().
 */
int nbPoints = 0;
int nbFacettes = 10000;
int dimension = 3;
int pas = 0;
float rotate[3];
float translate[3];

GLint GlobalePrimitiveDessin;

/*! Tableau gobal des sommets */
Vertex **TVertex = NULL;
Simplex **TSimplex = NULL;
fdp * f = NULL;
Pile * pile;
int nbSimplexAjoute = 0;

int nbPoints_traiter = 4;


/**
 * \fn double myRandom (double a, double b)
 *
 * \brief random entre deux bornes
 *
 * \param a borne min
 * \param b borne max
 */
double myRandom (double a, double b)
{
  double tmp = rand(); /* long int in [0, RAND_MAX] */

  return a+tmp*((b-a)/RAND_MAX);
}


/**
 * \fn void winInit()
 *
 * \brief Incantation d'ouverture de fenetre OpenGL 
 */
void winInit()
{
  gluOrtho2D(minX, maxX, minY, maxY);
}

void winInit3D()
{
  glOrtho(minX, maxX, minY, maxY, -500, 500);
}

/**
 *	\fn void selectPoints(int nbPoints)
 *
 * \brief Cree tous les points et les insert dans le tableau
 *
 * \param nbPoints Le nombre de points dont on a besoin
 */
void selectPoints(int nbPoints)
{
  
  int i;
  for(i = 0; i < 4; i++){
  	//TVertex[i] = newVertexWithCoords((i%2)*maxX, (i/2)*maxY, (float)myRandom(minZ, maxZ));
	 TVertex[i] = newVertex();
	 TVertex[i]->coords[0] = (i%2)*maxX;
     TVertex[i]->coords[1] = (i/2)*maxY;
     TVertex[i]->coords[2] = myRandom(minZ, maxZ);
  }
  int n = nbPoints;
  while (--n >= 4)
   {
   	//TVertex[n] = newVertexWithCoords((float)myRandom(minX, maxX), (float)myRandom(minY, maxY), (float)myRandom(minZ, maxZ/*-10*/));
     TVertex[n] = newVertex();
     TVertex[n]->coords[0] = myRandom(minX, maxX);
     TVertex[n]->coords[1] = myRandom(minY, maxY);
     TVertex[n]->coords[2] = myRandom(minZ, maxZ/*-10*/);
   }
}



/**
 * \fn void displaySimplex2D(void)
 *
 * \brief Affichage 2D des simplex
 */
void displaySimplex2D(void)
{
	int i;

	glColor3f(0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	fprintf(stderr, "Nb simplex dessiné: %d\n", nbSimplexAjoute);

	for(i = 0; i < nbSimplexAjoute; i++)
	{
		affichageSimplex2D(TSimplex[i]);
		// if(TSimplex[i]->m_list_candidats->Count > 0)
		// 	fprintf(stderr, "%d ", TSimplex[i]->m_list_candidats->Count);
	}

	for(i = 0; i<nbPoints; i++)
	{
		affichageVertex2D(TVertex[i]);
	}

	glFlush();
}



/**
 * \fn void displaySimplex2D(void)
 *
 * \brief Affichage 2D des simplex
 */
void displaySimplex3D()
{
    //Clear screen
    glColor3f(0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(translate[0], translate[1], translate[2]);
	glRotatef(rotate[0], 1, 0,0);
	glRotatef(rotate[1], 0, 1,0);
	glRotatef(rotate[2], 0, 0,1);
	int i;
	glColor3f(0.0,1.0,0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0, 1.0);
	for(i = 0; i < nbSimplexAjoute; i++)
	{
		affichageSimplex3DInterieur(TSimplex[i]);
	}
	
	glDisable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1.0,1.0,1.0);
	for(i = 0; i < nbSimplexAjoute; i++)
	{
		affichageSimplex3DLigne(TSimplex[i]);
	}
	for(i = 0; i<nbPoints; i++)
	{
		affichageVertex3D(TVertex[i]);
	}
	glPopMatrix();
	glutSwapBuffers(); //Send scene to the screen to be shown
	glutPostRedisplay();
}


/**
 * \fn listerVertex()
 *
 * \brief Mettre dans une liste les 4 premiers simplexs
 */
List* listerVertex()
{
	List * l =newList();
	int i;
	for(i = 4; i < nbPoints; i++)
	{
		lstAdd(l, TVertex[i]);
	}
	return l;
}


/**
 * \fn void initialisePremiersSimplex()
 *
 * \brief Initialise les deux premiers simplex à l'aide des 4 premiers points
 */
void initialisePremiersSimplex()
{
	Simplex * s1 = newSimplexWithPoint(TVertex[0], TVertex[1], TVertex[2]);
	Simplex * s2 = newSimplexWithPoint(TVertex[1], TVertex[2], TVertex[3]);

	//Ajout voisin
	ajouteVoisin(s1, s2);
	ajouteVoisin(s2, s1);

	//Distribution point
	List *l = listerVertex();
	reattributionPoints2Simplex(s1, s2, l);

	//insertion file de prio
	insertSimplex(f, s1);
	insertSimplex(f, s2);

	//Ajout dans tableau
	TSimplex[nbSimplexAjoute] = s1;
	nbSimplexAjoute++;
	TSimplex[nbSimplexAjoute] = s2;
	nbSimplexAjoute++;
}


/**
 * \fn void inversionDiagSimplex(Simplex * s, Simplex * s_voisin, int indice_diag, int indice_diag_voisin)
 *
 * \brief inversion de la diagonale entre deux simplex
 * - creation des deux nouveaux simplexs
 * - mise a jour des voisins
 * - redistribution des points candidats
 * - rajoute dans la file de prio
 * - rajoute dans la pile
 *
 * \param s pointeur sur le premier simplex
 * \param s_voisin pointeur sur le deuxieme simplex voisin du premier
 * \param indice_diag indice du point de la nouvelle diag dans le simplex
 * \param indice_diag_voisin indice du point de la nouvelle diag dans le simplex voisin
 */
void inversionDiagSimplex(Simplex * s, Simplex * s_voisin, int indice_diag, int indice_diag_voisin)
{

	int dernier_point_s1;
	int dernier_point_s2;

	if(indice_diag == 0)
	{
		dernier_point_s1 = 1;
		dernier_point_s2 = 2;	
	}
	else if(indice_diag == 1)
	{
		dernier_point_s1 = 0;
		dernier_point_s2 = 2;	
	}
	else
	{
		dernier_point_s1 = 0;
		dernier_point_s2 = 1;
	}
	Simplex * s1 = newSimplexWithPoint(s->m_tab_points[indice_diag], s_voisin->m_tab_points[indice_diag_voisin], s->m_tab_points[dernier_point_s1]);
	Simplex * s2 = newSimplexWithPoint(s->m_tab_points[indice_diag], s_voisin->m_tab_points[indice_diag_voisin], s->m_tab_points[dernier_point_s2]);		

	//affichageSimplex(s);
	//affichageSimplex(s_voisin);

	//ajouteVoisin(s1, s2);
	ajouteVoisin(s1, s->m_tab_voisins[dernier_point_s2]);
	int position = indicePosition(s_voisin, s->m_tab_points[dernier_point_s2]);
	if(position == -1)
		return;
	//fprintf(stderr, "position: %d\n", position);
	ajouteVoisin(s1, s_voisin->m_tab_voisins[position]);

	Simplex * s_1 = NULL;
	Simplex * s_2 = NULL;
	if(s->m_tab_voisins[dernier_point_s2] != NULL)
		s_1 = s->m_tab_voisins[dernier_point_s2];
	if(s_voisin->m_tab_voisins[position] != NULL)
		s_2 = s_voisin->m_tab_voisins[position];

	//ajouteVoisin(s2, s1);	
	ajouteVoisin(s2, s->m_tab_voisins[dernier_point_s1]);
	position = indicePosition(s_voisin, s->m_tab_points[dernier_point_s1]);
	if(position == -1)
		return;
	ajouteVoisin(s2, s_voisin->m_tab_voisins[position]);

	Simplex * svoisin_1 = NULL;
	Simplex * svoisin_2 = NULL;
	if(s->m_tab_voisins[dernier_point_s1] != NULL)
		svoisin_1 = s->m_tab_voisins[dernier_point_s1];
	if(s_voisin->m_tab_voisins[position] != NULL)
		svoisin_2 = s_voisin->m_tab_voisins[position];

	//Reatribution des points
	reattributionPoints2Simplex(s1, s2, s->m_list_candidats);
	reattributionPoints2Simplex(s1, s2, s_voisin->m_list_candidats);

	*s = *s1;
	*s_voisin = *s2;

	ajouteVoisin(s, s_voisin);
	if(s_1 != NULL)
		ajouteVoisin(s_1, s);
	if(s_2 != NULL)
		ajouteVoisin(s_2, s);

	ajouteVoisin(s_voisin, s);
	if(svoisin_1 != NULL)
		ajouteVoisin(svoisin_1, s_voisin);
	if(svoisin_2 != NULL)
		ajouteVoisin(svoisin_2, s_voisin);

	empiler(pile, s);

	if(s->m_list_candidats->Count > 0)
	{	
		// int pos = positionDansFDP(f, s);
		// fprintf(stderr, "pos: %d\n", pos);
		// if(pos < 0)
		// {
		// 	if(s->m_list_candidats->Count >= taille_s)
		// 		upHeap(f, pos);
		// 	else
		// 		downHeapPos(f, pos);
		// }
		// else
			insertSimplex(f, s);
	}
	if(s_voisin->m_list_candidats->Count > 0)
	{	
		// int pos = positionDansFDP(f, s_voisin);
		// fprintf(stderr, "pos: %d\n", pos);
		// if(pos > 0)
		// {
		// 	if(s_voisin->m_list_candidats->Count >= taille_s_v)
		// 		upHeap(f, pos);
		// 	else
		// 		downHeapPos(f, pos);
		// }
		// else
			insertSimplex(f, s_voisin);
	}	
	
	// insertSimplex(f, s);
	// insertSimplex(f, s_voisin);

	free(s1);
	free(s2);
}

/**
 * \fn void inversionDiagonale(Simplex * s, Simplex * s1, Simplex * s2)
 *
 * \brief - ajoute les trois nouveaux simplex créés
 * - on depile
 * - onn choisit les simplex et le voisin avec qui on veut flip la diagonale
 * - test de l'angle
 * - test de la diagonale
 *
 * \param s pointeur sur le premier simplex
 * \param s1 pointeur sur le second simplex
 * \param s2 pointeur sur le troisieme simplex
 */
void inversionDiagonale(Simplex * s, Simplex * s1, Simplex * s2)
{
	int i;
	int j;
	int k;
	int res;
	Simplex * simplex;
	Simplex * s_voisin;
	int somme = 0;

	empiler(pile, s);
	empiler(pile, s1);
	empiler(pile, s2);
	
	while(pile->m_nb_elements != 0)
	{
		simplex = depiler(pile);
		for(k = 0; k < 3; k++)
		{				
			s_voisin = simplex->m_tab_voisins[k];

			if(s_voisin != NULL)
			{
				if(estDans(pile, s_voisin) == 1)
				{
					continue; 
				}
				i = indiceDiff(s_voisin, simplex);
				//Test de l'angle
				for(j = 0; j < 3; j++)
				{
					if(j != k)
					{
						somme += Angle(simplex->m_tab_points[k], simplex->m_tab_points[j], s_voisin->m_tab_points[i]);
					}
				}
				if( somme == 0 )
				{		
					//On fait le test du cercle
					res = InCircle(simplex->m_tab_points[0],simplex->m_tab_points[1],simplex->m_tab_points[2],s_voisin->m_tab_points[i]);
					if(res == 1)
					{
						inversionDiagSimplex(simplex, s_voisin, k, i);
						break;
					}
				}
				somme = 0;
				
			}
		}
	}
}


/**
 * \fn void divisionSimplex(Simplex * s)
 *
 *	\brief Divise un simplex en trois, 
 *	l'enleve de la file de prio,
 *	passe sa variable affichage a 0,
 *	insert les 3 nouveaux simplex dans la file de prio et dans le tableau de simplex
 *
 */
void divisionSimplex(Simplex * s)
{

	nbPoints_traiter++;

	Vertex * v = nodeGetData(s->m_list_candidats->First);
	lstPopFront(s->m_list_candidats);

	//creer les 3 nouveaux
	Simplex * s1 = newSimplexWithPoint(s->m_tab_points[0], s->m_tab_points[1], v);
	Simplex * s2 = newSimplexWithPoint(s->m_tab_points[1], s->m_tab_points[2], v);
	Simplex * s3 = newSimplexWithPoint(s->m_tab_points[2], s->m_tab_points[0], v);


	//On repartit les points candidats du simplex
	// fprintf(stderr, "vieu s: %d\n", s->m_list_candidats->Count);
	reattributionPoints3Simplex(s1, s2, s3, s->m_list_candidats);
	// fprintf(stderr, "s1: %d\n", s1->m_list_candidats->Count);
	// fprintf(stderr, "s2: %d\n", s2->m_list_candidats->Count);
	// fprintf(stderr, "s3: %d\n", s3->m_list_candidats->Count);

	//On ajoute les voisins
	ajouteVoisin(s1,s2);
	ajouteVoisin(s1,s->m_tab_voisins[2]);
	if(s->m_tab_voisins[2] != NULL)
		ajouteVoisin(s->m_tab_voisins[2], s1);

	ajouteVoisin(s2,s1);
	ajouteVoisin(s2,s->m_tab_voisins[0]);
	if(s->m_tab_voisins[0] != NULL)
		ajouteVoisin(s->m_tab_voisins[0], s2);

	ajouteVoisin(s3,s2);
	ajouteVoisin(s3,s1);
	ajouteVoisin(s3,s->m_tab_voisins[1]);

	Simplex * s_1 = NULL;
	if(s->m_tab_voisins[1] != NULL)
		s_1 = s->m_tab_voisins[1];

	//ajouter au tab
	TSimplex[nbSimplexAjoute] = s1;
	nbSimplexAjoute++;
	TSimplex[nbSimplexAjoute] = s2;
	nbSimplexAjoute++;

	//copy(s,s3);
	*s = *s3;

	ajouteVoisin(s1,s);
	ajouteVoisin(s2,s);
	if(s_1 != NULL)
		ajouteVoisin(s_1, s);

	//ajouter a la fdp si le simplex a des points candidats
	//if(s1->m_list_candidats->Count > 0)
	//if(s2->m_list_candidats->Count > 0)
	//if(s->m_list_candidats->Count > 0)


	// fprintf(stderr, "S: %d\n", s->m_list_candidats->Count);
	// fprintf(stderr, "S1: %d\n", s1->m_list_candidats->Count);
	// fprintf(stderr, "S2: %d\n", s2->m_list_candidats->Count);

	// fprintf(stderr, "----------\n");
	insertSimplex(f, s);
	// if(positionDansFDP(f, s) == -1)
	// 	fprintf(stderr, "J'ai perdu S: %d\n", s->m_list_candidats->Count);
	insertSimplex(f, s1);
	// if(positionDansFDP(f, s1) == -1)
	// 	fprintf(stderr, "J'ai perdu S1: %d\n", s1->m_list_candidats->Count);
	insertSimplex(f, s2);
	// if(positionDansFDP(f, s2) == -1)
	// 	fprintf(stderr, "J'ai perdu S2: %d\n", s2->m_list_candidats->Count);
	// fprintf(stderr, "----------\n");

	inversionDiagonale(s, s1, s2);

	free(s3);
}

/**
 * \fn void GestionClavier2D(unsigned char key, int x, int y)
 *
 * \brief gere la gestion des touches lors de l'execution en 2D
 *
 * \param parametres obligatoires
 */
void GestionClavier2D(unsigned char key, int x, int y)
{ 		 
	Simplex *s;
	if (key == 'p')
	{
		if(f->nbSimplex > 0 && nbFacettes > nbSimplexAjoute)
		{
			s = getTete(f);
			if(s != NULL)
			{
				while(s->m_list_candidats->Count <= 0)
				{
					s = getTete(f);
					if(s == NULL)
						break;
				}
				if(s != NULL)
					divisionSimplex(s);
			}

			glClear(GL_COLOR_BUFFER_BIT);
			glutPostRedisplay();
		}
		else if(nbFacettes <= nbSimplexAjoute)
		{
			fprintf(stderr, "Trop de facettes\n"); 

			fprintf(stderr, "NB POINTS TRAITER: %d\n", nbPoints_traiter);
			fprintf(stderr, "NB SIMPLEX AJOUTE: %d\n", nbSimplexAjoute);
		} 
		else
		{
			fprintf(stderr, "Pile vide\n");

			fprintf(stderr, "NB POINTS TRAITER: %d\n", nbPoints_traiter);
			fprintf(stderr, "NB SIMPLEX AJOUTE: %d\n", nbSimplexAjoute);
		}
	}
	if (key == 'r')
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

/**
 * \fn void GestionClavier3D(unsigned char key, int x, int y)
 *
 * \brief gere la gestion des touches lors de l'execution en 3D
 *
 * \param parametres obligatoires
 */
void GestionClavier3D(unsigned char key, int x, int y)
{ 		 
	Simplex * s;
	if (key == 'p')
	{
		if(f->nbSimplex > 0 && nbSimplexAjoute <= nbFacettes)
		{
			s = getTete(f);

			if(s->m_list_candidats->Count > 0)
				divisionSimplex(s);

			//divisionSimplex(getTete(f));
			glClear(GL_COLOR_BUFFER_BIT);
			glutPostRedisplay();
		}
		else if(f->nbSimplex <= 0)
		{
			fprintf(stderr, "File de prio vide\n");
		}
		else
		{
			fprintf(stderr, "trop de facettes\n");
		}
	}
	else if(key == 'l')
	{
		rotate[1] -=1.0;
	}
	else if (key == 'i')
	{
		rotate[0] -=1.0;
	}
	else if (key == 'o')
	{
		rotate[2] +=1.0;
	}
	else if(key == 'j')
	{
		rotate[1] +=1.0;
	}
	else if (key == 'k')
	{
		rotate[0] +=1.0;
	}
	else if (key == 'u')
	{
		rotate[2] -=1.0;
	}

	else if (key == 'd')
	{
		translate[0] += 0.01;
	}
	else if (key == 'z')
	{
		translate[1] += 0.01;
	}
	else if (key == 'e')
	{
		translate[2] += 0.01;
	}

	else if (key == 'q')
	{
		translate[0] -= 0.01;
	}
	else if (key == 's')
	{
		translate[1] -= 0.01;
	}
	else if (key == 'a')
	{
		translate[2] -= 0.01;
	}
	glutPostRedisplay() ;
}

/*! \brief Fonction principale: on peut choisir le nombre de points
 * en utilisant l'option "-nX" où X est un entier strictement
 * positif.
 * \remark Mettre opterr a 0 equivaut a supprimer volontairement les messages d'erreur renvoyes par getopt 
 * lors de la lecture d'options non prevus sur la ligne de commande. Dans ces cas, l'erreur est traitee grace au
 * fait que getopt retourne la valeur '?', qui est donc traitee a part
 * \remark "n:" signifie que l'option n s'accompagne de la lecture immediate d'une valeur. Il ne faut pas
 * laisser d'espace entre -n et le nombre tape. Exemple: two -n1000.\par
 * La lecture est effectuee par un sscanf (lecture d'une chaine supposee formattee) sur l'argument ad hoc, optarg,
 * declare dans les routines de lecture de la ligne de commande.
 */
int main(int argc, char **argv)  
{  
	int c;

	opterr = 0;
	while ((c = getopt(argc, argv, "n:f:d:p:")) != EOF)
	{
		switch (c)
		{
			case 'n': if ((sscanf(optarg, "%d", &nbPoints) != 1) || nbPoints <= 0)
						nbPoints = 50;
					  	break;
			case 'f': if ((sscanf(optarg, "%d", &nbFacettes) != 1) || nbFacettes <= 0)
						nbFacettes = 1000000;
						break;
			case 'd': if ((sscanf(optarg, "%d", &dimension) > 3) || dimension < 2)
						dimension = 3;
						break;
			case 'p': if ((sscanf(optarg, "%d", &pas) < 0) || pas > 1)
						pas = 0;
						break;
			case '?': printf("use option -nX (no space), with 0 < X.\n");
					  break;
			default : printf("Shouldn't be here, really...\n");
					  break;
		}
	}
	clock_t start_t, end_t;
	double total_t;
	assert(nbPoints > 0);
	start_t = clock();
	TVertex =  malloc(sizeof(Vertex*)*nbPoints+5);
	TSimplex = malloc(sizeof(Simplex*)*nbPoints*2); 
	pile = creationPile(2*nbPoints-6);
	
	assert(TVertex != NULL);
	selectPoints(nbPoints);
	
	f = allouerFDP(2*nbPoints-6);
	initialisePremiersSimplex();

	if (pas != 1)
	{
		Simplex *s;
		while(f->nbSimplex > 0 /*&& nbFacettes > nbSimplexAjoute*/)
		{

			s = getTete(f);

			if(s->m_list_candidats->Count > 0)
				divisionSimplex(s);


			// if(f->nbSimplex <= 0)
			// {
			// 	fprintf(stderr, "d\n");
			// 	int i;
			// 	for (i = 0; i < nbSimplexAjoute; i++)
			// 	{
			// 		if(TSimplex[i]->m_list_candidats->Count > 0)
			// 			insertSimplex(f, TSimplex[i]);
			// 	}
			// 	fprintf(stderr, "f\n");
			// }
		}
		
		if(f->nbSimplex <= 0)
		{
			fprintf(stderr, "File de prio vide\n");
		}
		else
		{
			fprintf(stderr, "trop de facettes\n");
		}
		
		fprintf(stderr, "NB POINTS TRAITER: %d\n", nbPoints_traiter);
		fprintf(stderr, "NB SIMPLEX AJOUTE: %d\n", nbSimplexAjoute);
	}
	end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    fprintf(stderr,"Total time taken by CPU: %f\n", total_t );
  	int i;
  	for(i = 0; i < 3; i++)
  	{
		rotate[i] = 0;
		translate[i] = 0;
	}
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);  
	glutInitWindowPosition(5,5);  
	glutInitWindowSize(600, 600);  
	glutCreateWindow("My first OpenGL window..."); 
	switch(dimension) {
		case 2 : //-------------Affichage 2D
			winInit();
			//Affichage pas a pas
			glutKeyboardFunc(GestionClavier2D); 	
			glutDisplayFunc(displaySimplex2D);
			break;
			
		case 3 :
			glEnable(GL_DEPTH_TEST); //Make sure 3D drawing works when one object is in front of another
			glDepthMask(GL_TRUE);
			int i;
			for(i = 0; i < 3; i++){
				rotate[i] = 0;
				translate[i] = 0;
			}
			winInit3D();
			// register callbacks
			glutKeyboardFunc(GestionClavier3D);
			glutDisplayFunc(displaySimplex3D);
			break;
	}
	// enter GLUT event processing cycle
	glutMainLoop();
    

  	destructionFDP(f);
  	for(i = 0; i<nbSimplexAjoute; i++)
		free(TSimplex[i]);
	for(i = 0; i<nbPoints; i++)
		free(TVertex[i]);
	
  return EXIT_SUCCESS;  
}  
