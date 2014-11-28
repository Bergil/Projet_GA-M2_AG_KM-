#include <assert.h>  
#include <unistd.h>  
#include <GL/glut.h> 
#include <time.h>

#include "FilePrio.h"
#include "Simplex.h"

#define ALLOUER(X,NB) do if ( (X = malloc(sizeof(*(X)) * (NB))) == 0 )\
                             { fprintf(stderr, "Plus de memoire\n") ; \
                                exit(1); } \
                      while(0)

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
vertex *TVertex = NULL;
vertex *TSimplex = NULL;


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

	glColor3f(0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBegin(GL_LINES);
	
	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(TVertex[0].coords[0], TVertex[0].coords[1]);glVertex2f(TVertex[1].coords[0], TVertex[1].coords[1]);
	glVertex2f(TVertex[1].coords[0], TVertex[1].coords[1]);glVertex2f(TVertex[2].coords[0], TVertex[2].coords[1]);
	glVertex2f(TVertex[2].coords[0], TVertex[2].coords[1]);glVertex2f(TVertex[0].coords[0], TVertex[0].coords[1]);

	glEnd();

	glBegin(GL_POINTS);

	while (--n >= 0){
		glColor3f(1.0, 0, 0);
		affichageVertex(&TVertex[n]);
		glVertex2f(TVertex[n].coords[0], TVertex[n].coords[1]);
	}
	glEnd();

	glFlush();
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


	//int option = 0;
	assert(nbPoints > 0);
	fprintf(stderr,"nbPoints = %d\n", nbPoints);
	TVertex = (vertex *) malloc(sizeof(vertex)*nbPoints+5);
	TSimplex= (vertex *) malloc(sizeof(vertex)*nbPoints);
	assert(TVertex != NULL);
	selectPoints(nbPoints);
	printf("Veuillez choisir une option : Rien encore\n"); 
	
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
