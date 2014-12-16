#include <assert.h>  
#include <unistd.h>  
#include <GL/glut.h> 
#include <time.h>

#include "FilePrio.h"
#include "Simplex.h"

/*! variable externe permettant de lire les parametres sur le ligne de commande.*/
extern char *optarg;

/*! bascule pour autoriser ou interdire (0) les messages d'erreurs envoyes par getopt. */
extern int opterr;

/*! Tailles diverses du systemes de fenetrage .*/
const double minX = 0,
             maxX = 1,
			 minY = 0,
			 maxY = 1,
			 minZ = -500,
			 maxZ = 500,
			 margin = 10;

/*! Nombre de "sites" dans le programme, modifiable par la ligne
 * de commande de la fonction main().
 */
int nbPoints = 50;

GLint GlobalePrimitiveDessin;

/*! Tableau gobal des sommets */
Vertex *TVertex = NULL;
Simplex *TSimplex = NULL;
int nbSimplexAjoute = 0;


///Début des fonctions /////
double myRandom (double a, double b)
{
  double tmp = rand(); /* long int in [0, RAND_MAX] */

  return a+tmp*((b-a)/RAND_MAX);
}


/*! Incantation d'ouverture de fenetre OpenGL */
void winInit()
{
  gluOrtho2D(minX, maxX, minY, maxY);
}


/*! Generations des sites */
void selectPoints(int nbPoints)
{
  
  int i;
  for(i = 0; i < 4; i++){
	 TVertex[i].coords[0] = (i%2)*maxX;
     TVertex[i].coords[1] = (i/2)*maxY;
     TVertex[i].coords[2] = myRandom(minZ, maxZ);
  }
  int n = nbPoints;
  while (--n >= 4)
   {
     TVertex[n].coords[0] = myRandom(minX, maxX);
     TVertex[n].coords[1] = myRandom(minY, maxY);
     TVertex[n].coords[2] = myRandom(minZ, maxZ-10);
   }
}


/*Vertex* findFirstLexico(Vertex * T){
	int i;
	Vertex* temp = T;
	for(i = 1; i < nbPoints; i++){
		if(T[i].coords[0] < temp->coords[0]){
			temp= &T[i];
		}
		else if (temp->coords[0] == T[i].coords[0]){
			if (T[i].coords[1] < temp->coords[1])
				temp = &T[i];
		}
	}
	return temp;
}*/


//////////////////////////////////////// Affichages ///////////////////////////////////////
 /*warning Particularite: "callback function", ie pas d'argument transmis... Tout en global, yeurk!
 */
void displaySimplex(void)
{
	int n = nbPoints;
	int i;

	glColor3f(0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	/*glBegin(GL_LINES);
	
	glColor3f(1.0, 1.0, 1.0);
	
	for(i = 0; i < nbSimplexAjoute; i++)
	{
		glVertex2f(TVertex[0].coords[0], TVertex[0].coords[1]);
		glVertex2f(TVertex[1].coords[0], TVertex[1].coords[1]);
	}

	glEnd();*/

	for(i = 0; i < nbSimplexAjoute; i++)
	{
		affichageSimplex2D(&TSimplex[i]);
	}

	glBegin(GL_POINTS);

	glColor3f(1.0, 0.0, 0.0);
	affichage2DList(TSimplex[0].m_list_candidats);
	
	glColor3f(0.0, 1.0, 0.0);
	affichage2DList(TSimplex[1].m_list_candidats);

	/*while (--n >= 0){
		glColor3f(1.0, 0, 0);
		//affichageVertex(&TVertex[n]);
		glVertex2f(TVertex[n].coords[0], TVertex[n].coords[1]);
	}*/

	glEnd();

	glFlush();
}


void initialisePremiersSimplex()
{
	fprintf(stderr, "----- Initialisation Premier Simplex -----\n");
	affichageVertex(&TVertex[0]);
	affichageVertex(&TVertex[1]);
	affichageVertex(&TVertex[2]);
	affichageVertex(&TVertex[3]);
	ajoutPointsSimplex(&TSimplex[0], &TVertex[0], &TVertex[1], &TVertex[2]);
	ajoutPointsSimplex(&TSimplex[1], &TVertex[1], &TVertex[2], &TVertex[3]);
	
	nbSimplexAjoute = 2;
}

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

	Vertex pt1;
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
	fprintf(stderr, "Calcul hauteur: %f\n", calculHauteur(s, &pt4));


	//int option = 0;
	assert(nbPoints > 0);
	fprintf(stderr,"nbPoints = %d\n", nbPoints);
	TVertex = (Vertex *) malloc(sizeof(Vertex)*nbPoints+5);
	TSimplex = (Simplex *) malloc(sizeof(Simplex)*nbPoints);

	assert(TVertex != NULL);
	selectPoints(nbPoints);
	printf("Veuillez choisir une option : Rien encore\n"); 
	
	initialisePremiersSimplex();
	List *l = listerVertex();
	fprintf(stderr, "liste vertex: %d\n", lstCount(l));
	reattributionPoints(&TSimplex[0], &TSimplex[1], l);
	fprintf(stderr, "S1: %d   S2: %d\n", lstCount(TSimplex[0].m_list_candidats), lstCount(TSimplex[1].m_list_candidats));

	//clock_t temps;
	
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);  
	glutInitWindowPosition(5,5);  
	glutInitWindowSize(500, 500);  
	glutCreateWindow("My first OpenGL window...");  // Là, c'est une incantation (sic)* de fenêtre !
	winInit(); 
	
	glutDisplayFunc(displaySimplex);
  	glutMainLoop();  
  
  return EXIT_SUCCESS;  
}  

// *invocation
