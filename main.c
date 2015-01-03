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
int nbPoints = 50;

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


//////////////////////////////////////// Affichages ///////////////////////////////////////
 /*warning Particularite: "callback function", ie pas d'argument transmis... Tout en global, yeurk!
 */

void displaySimplex2D(void)
{
	fprintf(stderr, "Dessin 2D\n");
	int i;

	glColor3f(0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	fprintf(stderr, "nbSimplexAjoute: %d\n", nbSimplexAjoute);
	for(i = 0; i < nbSimplexAjoute; i++)
	{
		affichageSimplex(&TSimplex[i]);
		fprintf(stderr, "\n");
		affichageSimplex2D(&TSimplex[i]);
	}

	for(i = 0; i<nbPoints; i++)
	{
		affichageVertex2D(&TVertex[i]);
	}

	glFlush();
}


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

//Draws the 3D scene
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

int testAngle(Simplex * s)
{
	int k;
	int j;
	int i;
	int somme = 0;
	Simplex * s_voisin;
	for(k = 0; k < 3; k++)//k est l'indice du point
	{	
		s_voisin = s->m_tab_voisins[k];
		if(s_voisin != NULL)
		{
			for(i = 0; i < 3; i++)
			{
				if(s_voisin->m_tab_voisins[i] != NULL)
					if(egaliteSimplex(s, s_voisin->m_tab_voisins[i]) == 1)
						break; //i est l'indice du point du voisin
			}

			//Test de l'angle
			for(j = 0; j < 3; j++)
			{
				if(j != k)
				{
					somme += Angle(s->m_tab_points[k], s->m_tab_points[j], s_voisin->m_tab_points[i]);
				}
			}
			if( somme == 0 )
				return 1;
			somme = 0;
		}
	}
	return 0;
}

void inversionDiagSimplex(Simplex * s, Simplex * s_voisin, int indice_diag, int indice_diag_voisin)
{
	fprintf(stderr, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
	affichageSimplex(s);
	affichageSimplex(s_voisin);
	fprintf(stderr, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");

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

	//Mise a jour des voisins
	ajouteVoisin(s1, s2);
	ajouteVoisin(s1, s->m_tab_voisins[dernier_point_s2]);
	if(s->m_tab_voisins[dernier_point_s2] != NULL)
		ajouteVoisin(s->m_tab_voisins[dernier_point_s2], s1);
	ajouteVoisin(s1, s_voisin->m_tab_voisins[dernier_point_s2]);
	if(s_voisin->m_tab_voisins[dernier_point_s2] != NULL)
		ajouteVoisin(s_voisin->m_tab_voisins[dernier_point_s2], s1);

	ajouteVoisin(s2, s1);
	ajouteVoisin(s2, s->m_tab_voisins[dernier_point_s1]);
	if(s->m_tab_voisins[dernier_point_s1] != NULL)
		ajouteVoisin(s->m_tab_voisins[dernier_point_s1], s2);
	ajouteVoisin(s2, s_voisin->m_tab_voisins[dernier_point_s1]);
	if(s_voisin->m_tab_voisins[dernier_point_s1] != NULL)
		ajouteVoisin(s_voisin->m_tab_voisins[dernier_point_s1], s2);

	// fprintf(stderr, "avant redistr s1: %d\n", s1->m_list_candidats->Count);
	// fprintf(stderr, "avant redistr s2: %d\n", s2->m_list_candidats->Count);
	// fprintf(stderr, "s: %d    s_voisin: %d\n", s->m_list_candidats->Count, s_voisin->m_list_candidats->Count);

	s1->m_list_candidats->Count = 0;
	s2->m_list_candidats->Count = 0;
	//Reatribution des points
	reattributionPoints2Simplex(s1, s2, s->m_list_candidats);
	reattributionPoints2Simplex(s1, s2, s_voisin->m_list_candidats);
	// fprintf(stderr, "apres redistr s1: %d\n", s1->m_list_candidats->Count);
	// fprintf(stderr, "apres redistr s2: %d\n", s2->m_list_candidats->Count);

	supprElement(f, s);
	supprElement(f, s_voisin);

	if(s1->m_list_candidats->First != NULL)
		insertSimplex(f, s1);
	if(s2->m_list_candidats->First != NULL)
		insertSimplex(f, s2);

	empiler(pile, s1);
	empiler(pile, s2);

	int i;
	for(i = 0; i < nbSimplexAjoute; i++)
	{
		if(egaliteSimplex(&TSimplex[i], s) == 1)
			TSimplex[i] = *s1;
		else if(egaliteSimplex(&TSimplex[i], s_voisin) == 1)
			TSimplex[i] = *s2;
	}
}

void inversionDiagonale(Simplex * s, Simplex * s1, Simplex * s2)
{
	int i;
	int j;
	int k;
	int res;
	Simplex * simplex;
	Simplex * s_voisin;
	int somme = 0;

	//if(testAngle(s) == 1)
	empiler(pile, s);
	//if(testAngle(s1) == 1)
	empiler(pile, s1);
	//if(testAngle(s2) == 1)
	empiler(pile, s2);

	while(pile->m_nb_elements != 0)
	{
		// fprintf(stderr, "---------- taille pile: %d ----------\n", pile->m_nb_elements);
		// affichagePile(pile);
		// fprintf(stderr, "-------------------------------------\n");
		// fprintf(stderr, "----- ELEMENT DEPILER -----\n");
		simplex = depiler(pile);
		// affichageSimplex(simplex);
		// fprintf(stderr, "---------------------------\n");

		// fprintf(stderr, "*** PILE ***\n");

		for(k = 0; k < 3; k++)
		{				
			s_voisin = simplex->m_tab_voisins[k];

			//k est l'indice du voisin dans le simplex
			if(s_voisin != NULL)
			{
				// fprintf(stderr, "k: %d\n", k);
				// affichageSimplex(s_voisin);
				/*for(i = 0; i < 3; i++)
				{
					fprintf(stderr, "i: %d\n", i);
					if(s_voisin->m_tab_voisins[i] != NULL)
					{	
						if(egaliteSimplex(simplex, s_voisin->m_tab_voisins[i]) == 1)
						{
							fprintf(stderr, "eg i: %d\n", i);*/
							if(estDans(pile, s_voisin) == 1)
							{
								// fprintf(stderr, "Il est dans la pile\n");	
								continue; //i est l'indice du point du voisin
							}
							/*fprintf(stderr, "Il est dans la pile\n");
						}
					}
				}
				if(i == 3)
					continue;
				*/

				i = indiceDiff(s_voisin, simplex);
				// fprintf(stderr, "*** DEBUT ANGLE i: %d k: %d***\n", i, k);	

				//Test de l'angle
				for(j = 0; j < 3; j++)
				{
					if(j != k)
					{
						// fprintf(stderr, "*** ANGLE ***\n");
						// affichageVertex(simplex->m_tab_points[k]);
						// affichageVertex(simplex->m_tab_points[j]);
						// affichageVertex(s_voisin->m_tab_points[i]);
						// fprintf(stderr, "*** FIN ANGLE ***\n");
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
						//fprintf(stderr, "*** Dans le cercle! ***\n");
						//Inverser les triangles

						inversionDiagSimplex(simplex, s_voisin, k, i);
						break;
					}
					else
					{
						//fprintf(stderr, "*** Pas dans le cercle! ***\n"); 
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
	//fprintf(stderr, "Je suis au debut de division \n");
	Vertex * v = nodeGetData(s->m_list_candidats->First);
	//creer les 3 nouveaux
	Simplex * s1 = newSimplexWithPoint(s->m_tab_points[0], s->m_tab_points[1], v);
	Simplex * s2 = newSimplexWithPoint(s->m_tab_points[1], s->m_tab_points[2], v);
	Simplex * s3 = newSimplexWithPoint(s->m_tab_points[2], s->m_tab_points[0], v);

	//fprintf(stderr, "J'ai ajoute\n");

	reattributionPoints3Simplex(s1, s2, s3, s->m_list_candidats);

	//fprintf(stderr, "J'ai redistribue\n");

	//Suppression du point choisis dans la liste
	lstPopFront(s->m_list_candidats);

	ajouteVoisin(s1,s2);
	ajouteVoisin(s1,s3);
	ajouteVoisin(s1,s->m_tab_voisins[2]);

	ajouteVoisin(s2,s1);
	ajouteVoisin(s2,s3);
	ajouteVoisin(s2,s->m_tab_voisins[0]);

	ajouteVoisin(s3,s2);
	ajouteVoisin(s3,s1);
	ajouteVoisin(s3,s->m_tab_voisins[1]);


	//ajouter au tab
	TSimplex[nbSimplexAjoute] = *s1;
	nbSimplexAjoute++;
	TSimplex[nbSimplexAjoute] = *s2;
	nbSimplexAjoute++;
	int i;
	for(i = 0; i<nbSimplexAjoute; i++)
		if(egaliteSimplex(s, &TSimplex[i]) == 1)
			break;
	copy(s, s3);
	TSimplex[i] = *s;
	//free(s3);

	//fprintf(stderr, "Je plante sur l'insertion\n");

	//ajouter a la fdp si le simplex a des points candidats
	if(s1->m_list_candidats->First != NULL)
		insertSimplex(f, s1);
	if(s2->m_list_candidats->First != NULL)
		insertSimplex(f, s2);
	if(s->m_list_candidats->First != NULL)
		insertSimplex(f, s);
	//fprintf(stderr, "Fin de la division\n" );

	inversionDiagonale(s, s1, s2);
}




void GestionClavier2D(unsigned char key, int x, int y)
{ 		 
	fprintf(stderr, "Gestion du clavier\n");
	if (key == 's')
	{
		fprintf(stderr, "f->nbSimplex: %d \n", f->nbSimplex);
		if(f->nbSimplex > 0)
		{
			fprintf(stderr, "******************** AJOUT D'UN POINT ******************** \n");
			divisionSimplex(getTete(f));

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

void GestionClavier3D(unsigned char key, int x, int y)
{ 		 
	fprintf(stderr, "Gestion du clavier\n");
	if (key == 's')
	{
		if(f->nbSimplex > 0)
		{
			fprintf(stderr, "***** AJOUT D'UN POINT ***** \n");
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
	pile = creationPile(1000);

	assert(TVertex != NULL);
	selectPoints(nbPoints);
	printf("Veuillez choisir une option : Rien encore\n"); 
	
	initialisePremiersSimplex();
	List *l = listerVertex();
	//fprintf(stderr, "liste vertex: %d\n", lstCount(l));
	reattributionPoints2Simplex(&TSimplex[0], &TSimplex[1], l);
	//fprintf(stderr, "S1: %d   S2: %d\n", lstCount(TSimplex[0].m_list_candidats), lstCount(TSimplex[1].m_list_candidats));

	f = allouerFDP(1000);
	insertSimplex(f, &TSimplex[0]);
	insertSimplex(f, &TSimplex[1]);
	
	/*while(f->nbSimplex > 0)
	{
		fprintf(stderr, "***** AJOUT D'UN POINT ***** \n");
		//fprintf(stderr, "Taille de la fdp: %d\n", f->nbSimplex);
		divisionSimplex(getTete(f));
	}*/
	//affichageHauteurFDP(f);

	//clock_t temps;
	
	//-------------Affichage 2D
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);  
	glutInitWindowPosition(5,5);  
	glutInitWindowSize(600, 600);  
	glutCreateWindow("My first OpenGL window...");  // Là, c'est une incantation (sic)* de fenêtre !
	winInit();

	fprintf(stderr, "f->nbSimplex: %d\n", f->nbSimplex);
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
