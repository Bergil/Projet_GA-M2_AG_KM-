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
int nbFacettes = -1;

GLint GlobalePrimitiveDessin;

/*! Tableau gobal des sommets */
Vertex *TVertex = NULL;
Simplex *TSimplex = NULL;
fdp * f = NULL;
Pile * pile;
int nbSimplexAjoute = 0;


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
	 TVertex[i].coords[0] = (i%2)*maxX;
     TVertex[i].coords[1] = (i/2)*maxY;
     //fprintf(stderr, "i: %f - %f\n", TVertex[i].coords[0], TVertex[i].coords[1]);
     TVertex[i].coords[2] = myRandom(minZ, maxZ);
  }
  int n = nbPoints;
  while (--n >= 4)
   {
     TVertex[n].coords[0] = myRandom(minX, maxX);
     TVertex[n].coords[1] = myRandom(minY, maxY);
     TVertex[n].coords[2] = myRandom(minZ, maxZ/*-10*/);
   }
}



/**
 * \fn void displaySimplex2D(void)
 *
 * \brief Affichage 2D des simplex
 */
void displaySimplex2D(void)
{
	//fprintf(stderr, "Dessin 2D\n");
	int i;

	glColor3f(0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//fprintf(stderr, "nbSimplexAjoute: %d\n", nbSimplexAjoute);
	for(i = 0; i < nbSimplexAjoute; i++)
	{
		//affichageSimplex(&TSimplex[i]);
		//fprintf(stderr, "\n");
		affichageSimplex2D(&TSimplex[i]);
	}

	for(i = 0; i<nbPoints; i++)
	{
		affichageVertex2D(&TVertex[i]);
	}

	glFlush();
}


/**
 * \fn void init(int argc, char** argv)
 *
 * \brief initialisation de la fenetre
 */
void init(int argc, char** argv)
{
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 600); //Window size
	glutCreateWindow("Introduction to OpenGL"); //Create a window

	//glMaterial();
	glEnable(GL_DEPTH_TEST); //Make sure 3D drawing works when one object is in front of another
  	//glEnable(GL_LIGHTING); 	// Active l'éclairage
  	//glEnable(GL_LIGHT0); 	// Allume la lumière n°1
}


/**
 * \fn void displaySimplex2D(void)
 *
 * \brief Affichage 2D des simplex
 */
void displaySimplex3D()
{
    //Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int i;
	for(i = 0; i < nbSimplexAjoute; i++)
	{
		affichageSimplex3D(&TSimplex[i]);
	}
	glutSwapBuffers(); //Send scene to the screen to be shown
}


/**
 * \fn void initialisePremiersSimplex()
 *
 * \brief Initialise les deux premiers simplex à l'aide des 4 premiers points
 */
void initialisePremiersSimplex()
{
	//fprintf(stderr, "----- Initialisation Premier Simplex -----\n");
	ajoutPointsSimplex(&TSimplex[0], &TVertex[0], &TVertex[1], &TVertex[2]);
	ajoutPointsSimplex(&TSimplex[1], &TVertex[1], &TVertex[2], &TVertex[3]);
	ajouteVoisin(&TSimplex[0], &TSimplex[1]);
	ajouteVoisin(&TSimplex[1], &TSimplex[0]);
	nbSimplexAjoute = 2;
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
	for(i = 4; i < nbPoints+4; i++)
	{
		lstAdd(l, &TVertex[i]);
	}
	return l;
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
	// fprintf(stderr, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
	// affichageSimplex(s);
	// affichageSimplex(s_voisin);
	// fprintf(stderr, "indice_diag: %d\n", indice_diag);
	// fprintf(stderr, "indice_diag_voisin: %d\n", indice_diag_voisin);
	// fprintf(stderr, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");

	//fprintf(stderr, "Debut\n");
	Simplex * s1;
	Simplex * s2;
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
	//Creation des nouveaux simplex
	s1 = newSimplexWithPoint(s->m_tab_points[indice_diag], s_voisin->m_tab_points[indice_diag_voisin], s->m_tab_points[dernier_point_s1]);
	s2 = newSimplexWithPoint(s->m_tab_points[indice_diag], s_voisin->m_tab_points[indice_diag_voisin], s->m_tab_points[dernier_point_s2]);		

	ajouteVoisin(s1, s2);
	ajouteVoisin(s1, s->m_tab_voisins[dernier_point_s2]);
	if(s->m_tab_voisins[dernier_point_s2] != NULL)
		ajouteVoisin(s->m_tab_voisins[dernier_point_s2], s1);

	//fprintf(stderr, "Avant ajout voisin\n");

	int position = indicePosition(s_voisin, s->m_tab_points[dernier_point_s2]);
	//fprintf(stderr, "pos: %d\n", position);
	if(position != -1)
	{
		ajouteVoisin(s1, s_voisin->m_tab_voisins[position]);
		if(s_voisin->m_tab_voisins[position] != NULL)
			ajouteVoisin(s_voisin->m_tab_voisins[position], s1);
	}

	//fprintf(stderr, "Avant ajout voisin\n");

	ajouteVoisin(s2, s1);	
	ajouteVoisin(s2, s->m_tab_voisins[dernier_point_s1]);
	if(s->m_tab_voisins[dernier_point_s1] != NULL)
		ajouteVoisin(s->m_tab_voisins[dernier_point_s1], s2);
	
	position = indicePosition(s_voisin, s->m_tab_points[dernier_point_s1]);
	if(position != -1)
	{
		ajouteVoisin(s2, s_voisin->m_tab_voisins[position]);
		if(s_voisin->m_tab_voisins[position] != NULL)
			ajouteVoisin(s_voisin->m_tab_voisins[position], s2);
	}

	s1->m_list_candidats->Count = 0;
	s2->m_list_candidats->Count = 0;

	//supprElement(f, s);
	//supprElement(f, s_voisin);

	//Reatribution des points
	reattributionPoints2Simplex(s1, s2, s->m_list_candidats);
	reattributionPoints2Simplex(s1, s2, s_voisin->m_list_candidats);
	
	//*****************
	/*int position_fdp_s = positionDansFDP(f, s);
	int position_fdp_s_voisin = positionDansFDP(f, s_voisin);
	
	f->tableau_Simplex[position_fdp_s] = s1;
	f->tableau_Simplex[position_fdp_s_voisin] = s2;		

	upHeap(f, position_fdp_s);
	downHeapPos(f, position_fdp_s);
	upHeap(f, position_fdp_s_voisin);
	downHeapPos(f, position_fdp_s_voisin);

	empiler(pile, s1);
	empiler(pile, s2);*/

	//*****************

	s->m_afficher = 0;
	s_voisin->m_afficher = 0;

	if(s1->m_list_candidats->Count > 0)
		insertSimplex(f, s1);
	if(s2->m_list_candidats->Count > 0)
		insertSimplex(f, s2);

	empiler(pile, s1);
	empiler(pile, s2);

	int i;
	for(i = 0; i < nbSimplexAjoute; i++)
	{
		if(egaliteSimplex(&TSimplex[i], s) == 1)
			TSimplex[i] = *s1;
		if(egaliteSimplex(&TSimplex[i], s_voisin) == 1)
			TSimplex[i] = *s2;
	}

	//fprintf(stderr, "Fin ajout voisin\n");
	//fprintf(stderr, "Fin\n");
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
		//fprintf(stderr, "\nSimplex\n");
		//affichageSimplex(simplex);

		for(k = 0; k < 3; k++)
		{				
			s_voisin = simplex->m_tab_voisins[k];

			//k est l'indice du voisin dans le simplex
			if(s_voisin != NULL)
			{
				//fprintf(stderr, "voisin: \n");
				//affichageSimplex(s_voisin);
				if(estDans(pile, s_voisin) == 1)
				{
					// fprintf(stderr, "Il est dans la pile\n");	
					continue; //i est l'indice du point du voisin
				}

				i = indiceDiff(s_voisin, simplex);
				// fprintf(stderr, "*** DEBUT ANGLE i: %d k: %d***\n", i, k);	

				//Test de l'angle
				for(j = 0; j < 3; j++)
				{
					if(j != k)
					{
						somme += Angle(simplex->m_tab_points[k], simplex->m_tab_points[j], s_voisin->m_tab_points[i]);
					}
				}
				//fprintf(stderr, "somme: %d\n", somme);
				
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
	//fprintf(stderr, "J'ai tout fini\n");
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
	Vertex * v = nodeGetData(s->m_list_candidats->First);

	//creer les 3 nouveaux
	Simplex * s1 = newSimplexWithPoint(s->m_tab_points[0], s->m_tab_points[1], v);
	Simplex * s2 = newSimplexWithPoint(s->m_tab_points[1], s->m_tab_points[2], v);
	Simplex * s3 = newSimplexWithPoint(s->m_tab_points[2], s->m_tab_points[0], v);

	//fprintf(stderr, "J'ai ajoute\n");
	lstPopFront(s->m_list_candidats);
	reattributionPoints3Simplex(s1, s2, s3, s->m_list_candidats);

	/*affichageSimplex(s);
	int i;
	for(i = 0; i<3; i++)
	{
		if(s->m_tab_voisins[i] != NULL)
			affichageSimplex(s->m_tab_voisins[i]);
	}*/

	//fprintf(stderr, "***** Les 3 nouveaux simplex *****\n");
	ajouteVoisin(s1,s2);
	ajouteVoisin(s1,s3);
	ajouteVoisin(s1,s->m_tab_voisins[2]);
	if(s->m_tab_voisins[2] != NULL)
		ajouteVoisin(s->m_tab_voisins[2], s1);

	ajouteVoisin(s2,s1);
	ajouteVoisin(s2,s3);
	ajouteVoisin(s2,s->m_tab_voisins[0]);
	if(s->m_tab_voisins[0] != NULL)
		ajouteVoisin(s->m_tab_voisins[0], s2);

	/*fprintf(stderr, "s\n");
	affichageSimplex(s);
	fprintf(stderr, "s2\n");
	affichageSimplex(s2);
	int i;
	for(i = 0; i<3; i++)
	{
		if(s2->m_tab_voisins[i] != NULL)
			affichageSimplex(s2->m_tab_voisins[i]);
	}*/


	ajouteVoisin(s3,s2);
	ajouteVoisin(s3,s1);
	ajouteVoisin(s3,s->m_tab_voisins[1]);
	if(s->m_tab_voisins[1] != NULL)
		ajouteVoisin(s->m_tab_voisins[1], s3);
	//fprintf(stderr, "***** FIN Les 3 nouveaux simplex *****\n");

	//ajouter au tab
	TSimplex[nbSimplexAjoute] = *s1;
	nbSimplexAjoute++;
	TSimplex[nbSimplexAjoute] = *s2;
	nbSimplexAjoute++;
	
	int i;
	for(i = 0; i<nbSimplexAjoute; i++)
		if(egaliteSimplex(s, &TSimplex[i]) == 1)
			break;
	//copy(s, s3);
	s->m_afficher = 0;
	TSimplex[i] = *s3;
	//free(s3)

	//ajouter a la fdp si le simplex a des points candidats
	if(s1->m_list_candidats->Count > 0)
		insertSimplex(f, s1);
	if(s2->m_list_candidats->Count > 0)
		insertSimplex(f, s2);
	if(s3->m_list_candidats->Count > 0)
		insertSimplex(f, s3);
	//fprintf(stderr, "Fin de la division\n" );
	inversionDiagonale(s3, s1, s2);
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
	if (key == 's')
	{
		if(f->nbSimplex > 0)
		{
			//fprintf(stderr, "******************** AJOUT D'UN POINT ******************** \n");
			affichageFDP(f);
			s = getTete(f);
			if(s != NULL)
			{
				while(s->m_afficher == 0)
				{
					s = getTete(f);
					if (s == NULL)
						break;
				}
				if(s != NULL)
				{
					//affichageSimplex(s);
					if(s->m_list_candidats->Count > 0)
						divisionSimplex(s);
				}
				else
					fprintf(stderr, "YA PLUS RIEN !!!\n");
			}
			glClear(GL_COLOR_BUFFER_BIT);
			glutPostRedisplay();
		}  
	}
	if (key == 'r')
	{
		glClear(GL_COLOR_BUFFER_BIT);
		//glutPostRedisplay();
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
	//fprintf(stderr, "Gestion du clavier\n");
	if (key == 's')
	{
		if(f->nbSimplex > 0)
		{
			//fprintf(stderr, "***** AJOUT D'UN POINT ***** \n");
			divisionSimplex(getTete(f));
		}  
	}
	else if(key == 'm')
	{
		glRotatef(10, 1.0f, 0.0f, 0.0f);
	}
	else if (key == 'l')
	{
		glRotatef(10, 0.0f, 1.0f, 0.0f);
	}
	else if (key == 'k')
	{
		glRotatef(10, 0.0f, 0.0f, 1.0f);
	}

	else if (key == 'p')
	{
		glTranslatef(1.0f, 0.0f, 0.0f);
	}
	else if (key == 'o')
	{
		glTranslatef(0.0f, 1.0f, 0.0f);
	}
	else if (key == 'i')
	{
		glTranslatef(0.0f, 0.0f, 1.0f);
	}

	else if (key == 'u')
	{
		glTranslatef(-1.0f, 0.0f, 0.0f);
	}
	else if (key == 'y')
	{
		glTranslatef(0.0f, -1.0f, 0.0f);
	}
	else if (key == 't')
	{
		glTranslatef(0.0f, 0.0f, -1.0f);
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
	while ((c = getopt(argc, argv, "n:")) != EOF)
	{
		switch (c)
		{
			case 'n': if ((sscanf(optarg, "%d", &nbPoints) != 1) || nbPoints <= 0)
						nbPoints = 50;
					  	break;
			case 'f': if ((sscanf(optarg, "%d", &nbFacettes) != 1) || nbFacettes <= 0)
						nbFacettes = 10;
						break;
			case '?': printf("use option -nX (no space), with 0 < X.\n");
					  break;
			default : printf("Shouldn't be here, really...\n");
					  break;
		}
	}

	//############### TEST ###################
	/*Vertex pt1;
	pt1.coords[0] = 0;
	pt1.coords[1] = 0;
	pt1.coords[2] = 0;
	Vertex pt2;
	pt2.coords[0] = 2;
	pt2.coords[1] = 0;
	pt2.coords[2] = 2;
	Vertex pt3;
	pt3.coords[0] = 0;
	pt3.coords[1] = 2;
	pt3.coords[2] = 2;
	Simplex* s = newSimplexWithPoint( &pt1, &pt2, &pt3 );
	
	Vertex pt4;
	pt4.coords[0] = 1;
	pt4.coords[1] = 1;
	pt4.coords[2] = 3;
	fprintf(stderr, "Calcul hauteur: %f\n", calculHauteur(s, &pt4));*/

	//int option = 0;
	assert(nbPoints > 0);
	//fprintf(stderr,"nbPoints = %d\n", nbPoints);
	TVertex = (Vertex *) malloc(sizeof(Vertex)*nbPoints+5);
	TSimplex = (Simplex *) malloc(sizeof(Simplex)*nbPoints*4); //x3 normalement
	pile = creationPile(2*nbPoints+6);

	assert(TVertex != NULL);
	selectPoints(nbPoints);
	printf("Veuillez choisir une option : Rien encore\n"); 
	
	initialisePremiersSimplex();
	List *l = listerVertex();
	//fprintf(stderr, "liste vertex: %d\n", lstCount(l));
	reattributionPoints2Simplex(&TSimplex[0], &TSimplex[1], l);
	//fprintf(stderr, "S1: %d   S2: %d\n", lstCount(TSimplex[0].m_list_candidats), lstCount(TSimplex[1].m_list_candidats));

	f = allouerFDP(2*nbPoints+6);
	insertSimplex(f, &TSimplex[0]);
	insertSimplex(f, &TSimplex[1]);

	//Normal
	Simplex *s;
	int boucle = 0;
	while(f->nbSimplex > 0)
	{
		//fprintf(stderr, "boucle: %d\n", boucle);
		//affichageFDP(f);
		//fprintf(stderr, "******************** AJOUT D'UN POINT ******************** \n");
		//fprintf(stderr, "taille de la file: %d\n", f->nbSimplex);
		s = getTete(f);
		if(s != NULL)
		{
			while(s->m_afficher == 0)
			{
				s = getTete(f);
				if (s == NULL)
					break;
			}
			if(s != NULL)
			{
				//affichageSimplex(s);
				if(s->m_list_candidats->Count > 0)
				{
					boucle++;
					divisionSimplex(s);
				}
			}
			else
				fprintf(stderr, "YA PLUS RIEN !!!\n");
		}
	}

	//clock_t temps;
	
	//-------------Affichage 2D
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);  
	glutInitWindowPosition(5,5);  
	glutInitWindowSize(600, 600);  
	glutCreateWindow("My first OpenGL window...");  // Là, c'est une incantation (sic)* de fenêtre !
	winInit();

	//fprintf(stderr, "f->nbSimplex: %d\n", f->nbSimplex);

	//Affichage pas a pas
	glutKeyboardFunc(GestionClavier2D); 	

    glutDisplayFunc(displaySimplex2D);
    glutMainLoop(); 
    //-------------------------
	
	//-------------Affichage 3D
	// init GLUT and create Window
	/*glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(600,600);
	glutCreateWindow("Lighthouse3D - GLUT Tutorial");

	// register callbacks
	glutKeyboardFunc(GestionClavier3D);
	glutDisplayFunc(displaySimplex3D);

	// enter GLUT event processing cycle
	glutMainLoop();*/
    //-------------------------

  	
  
  	destructionFDP(f);
  return EXIT_SUCCESS;  
}  

// *invocation
