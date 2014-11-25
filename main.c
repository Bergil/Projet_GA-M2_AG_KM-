#include <assert.h>  
#include <unistd.h>  
#include <GL/glut.h> 
#include <time.h>

#include "FilePrio.h"
#include "Triangle.h"

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
             maxX = 500,
			 minY = 0,
			 maxY = 500,
			 minZ = 0,
			 maxZ = 500,
			 margin = 10;

/*! Nombre de "sites" dans le programme, modifiable par la ligne
 * de commande de la fonction main().
 */
int nbPoints = 2000;

GLint GlobalePrimitiveDessin;

/*! Tableau gobal des sommets */
vertex *T = NULL;


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
  int n = nbPoints;

  while (--n >= 0)
   {
     T[n].coords[0] = myRandom(minX+10, maxX-10);
     T[n].coords[1] = myRandom(minY+10, maxY-10);
     T[n].coords[2] = myRandom(minZ+10, maxZ-10);
   }
}


/*vertex* findFirstLexico(vertex * T){
	int i;
	vertex* temp = T;
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
void displayTriangle(void)
{
	int n = nbPoints;

	glColor3f(0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBegin(GL_POINTS);
	
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

	vertex * pt1;
	ALLOUER(pt1,1);
	vertex * pt2;
	ALLOUER(pt2,1);
	vertex * pt3;
	ALLOUER(pt3,1);
	pt1->coords[0] = 1; 
	pt1->coords[1] = 1;
	pt1->coords[2] = 0;
	pt2->coords[0] = 4; 
	pt2->coords[1] = 1;
	pt2->coords[2] = 0;
	pt3->coords[0] = 2; 
	pt3->coords[1] = 2;
	pt3->coords[2] = 0;
	/*fprintf(stderr, "orientation: %d\n", orientationPolaire(pt3,pt2,pt1));
	vertex * res = minLexico(pt1,pt2,pt3);
	fprintf(stderr, "%f %f %f \n", res->coords[0], res->coords[1], res->coords[2]);*/
	Triangle *t = newTriangleWithPoint(pt2,pt1,pt3);
	fprintf(stderr, "%f %f %f \n", t->m_tab_points[0]->coords[0], t->m_tab_points[0]->coords[1], t->m_tab_points[0]->coords[2]);
	fprintf(stderr, "%f %f %f \n", t->m_tab_points[1]->coords[0], t->m_tab_points[1]->coords[1], t->m_tab_points[1]->coords[2]);
	fprintf(stderr, "%f %f %f \n", t->m_tab_points[2]->coords[0], t->m_tab_points[2]->coords[1], t->m_tab_points[2]->coords[2]);

	//int option = 0;
	assert(nbPoints > 0);
	T = (vertex *) malloc(sizeof(vertex)*nbPoints+1);
	assert(T != NULL);
	selectPoints(nbPoints);
	printf("Veuillez choisir une option : Rien encore\n"); 
	//clock_t temps;
	
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);  
	glutInitWindowPosition(5,5);  
	glutInitWindowSize(700, 700);  
	glutCreateWindow("My first OpenGL window...");  // Là, c'est une incantation (sic)* de fenêtre !
	winInit(); 
	
	glutDisplayFunc(displayTriangle);
  	glutMainLoop();  
  
  return EXIT_SUCCESS;  
}  

// *invocation
