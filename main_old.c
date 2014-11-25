#include <assert.h>  
#include "FilePrio.h"
#include <unistd.h>  
#include <GL/glut.h> 
#include <time.h>
#include "Triangle.h"

#define ALLOUER(X,NB) do if ( (X = malloc(sizeof(*(X)) * (NB))) == 0 )\
                             { fprintf(stderr, "Plus de memoire\n") ; \
                                exit(1); } \
                      while(0)

/*! variable externe permettant de lire les parametres sur le ligne de commande.*/
extern char *optarg;

/*! bascule pour autoriser ou interdire (0) les messages d'erreurs envoyes par getopt. */
extern int opterr;

typedef struct
{
	double xMax;
	double yMax;
	double xMin;
	double yMin;
} RectangleEnglobant;

/*! Tailles diverses du systemes de fenetrage .*/
const double minX = 0,
             maxX = 510,
			 minY = 0,
			 maxY = 510,
			 minZ = 0,
			 maxZ = 510,
			 margin = 10;

/*! Nombre de "sites" dans le programme, modifiable par la ligne
 * de commande de la fonction main().
 */
int nbPoints;
int nbPointsPoly = 24;
GLint GlobalePrimitiveDessin;

/*! Tableau gobal des sommets */
vertex *T = NULL;
//Rectangle englobant
RectangleEnglobant recEnglo;

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
	 T[i].coords[0] = (i%2)*maxX;
     T[i].coords[1] = (i/2)*maxY;
     T[i].coords[2] = myRandom(minZ, maxZ-10);
  }
  int n = nbPoints;
  while (--n >= 4)
   {
     T[n].coords[0] = myRandom(minX+10, maxX-10);
     T[n].coords[1] = myRandom(minY+10, maxY-10);
     T[n].coords[2] = myRandom(minZ, maxZ-10);
   }
}

vertex* findFirstLexico(vertex * T){
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
}


/////////////////////////////////////	FIGURE	/////////////////////////////////

void initialisationRectangleEnglobant()
{
	int i;
	recEnglo.xMax = T[0].coords[0];
	recEnglo.xMin = T[0].coords[0];
	recEnglo.yMax = T[0].coords[1];
	recEnglo.yMin = T[0].coords[1];

	for(i = 1; i<nbPointsPoly; i++)
	{
		if(T[i].coords[0] >= recEnglo.xMax)
			recEnglo.xMax = T[i].coords[0];
		else if(T[i].coords[0] < recEnglo.xMin)
			recEnglo.xMin = T[i].coords[0];	
		
		if(T[i].coords[1] >= recEnglo.yMax)
			recEnglo.yMax = T[i].coords[1];
		else if(T[i].coords[1] < recEnglo.yMin)
			recEnglo.yMin = T[i].coords[1];	
	}
}


////////////////////////////////////////////	TESTS INCLUSION		///////////////////////////////////////////
//Test si on est dans le rectangle englobant
int estDansRecEnglobant(vertex a)
{
	if(a.coords[0] >= recEnglo.xMax || a.coords[0] <= recEnglo.xMin)
		return 0;
	if(a.coords[1] >= recEnglo.yMax || a.coords[1] <= recEnglo.yMin)
		return 0;
	
	return 1;
}


//////////////////////////////////////// Affichages ///////////////////////////////////////
 /*warning Particularite: "callback function", ie pas d'argument transmis... Tout en global, yeurk!
 */
void displayTriangle(void)
{
	int n = nbPoints;

	glColor3f(0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBegin(GL_LINES);
	
	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(T[0].coords[0], T[0].coords[1]);glVertex2f(T[1].coords[0], T[1].coords[1]);
	glVertex2f(T[1].coords[0], T[1].coords[1]);glVertex2f(T[2].coords[0], T[2].coords[1]);
	glVertex2f(T[2].coords[0], T[2].coords[1]);glVertex2f(T[0].coords[0], T[0].coords[1]);

	glEnd();

	glBegin(GL_POINTS);

	/*while (--n >= 3) // Affichage des points du triangle
	{
		if(estDansTriangle(&T[0],&T[1],&T[2],&T[n]) == 1) //Si le point est dans le triangle
		{	
			glColor3f(0, 1.0, 0);
			glVertex2f(T[n].coords[0], T[n].coords[1]);
		}
		else if (estDansTriangle(&T[0],&T[1],&T[2],&T[n]) == -1)
		{	
			glColor3f(0, 0, 1.0);
			glVertex2f(T[n].coords[0], T[n].coords[1]);
		}
		else // Si le point est à l'exterieur du triangle
		{	
			glColor3f(1.0, 0, 0);
			glVertex2f(T[n].coords[0], T[n].coords[1]);
		}
	}*/
	while (--n >= 0){
		glColor3f(1.0, 0, 0);
		glVertex2f(T[n].coords[0], T[n].coords[1]);
	}
	glEnd();

	glFlush();
}

/*
void displayPolygone(void)
{
	int n = 23;

	glColor3f(0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBegin(GL_LINES);
	//Affichage polygone
	glColor3f(1.0, 1.0, 1.0);
	while(--n >= 0)
	{
		glVertex2f(T[n].coords[0], T[n].coords[1]); 
		glVertex2f(T[n+1].coords[0], T[n+1].coords[1]);
	}
	glVertex2f(T[0].coords[0], T[0].coords[1]); 
	glVertex2f(T[23].coords[0], T[23].coords[1]);

	glEnd();

	n = nbPoints;
	initialisationRectangleEnglobant();
	glBegin(GL_POINTS);

	//Affichage du reste des points
	while(--n >= nbPointsPoly)
	{	
		//fprintf(stderr, " n:%d \n", n);
		if(estDansRecEnglobant(T[n]) == 1)
		{
			
			//fprintf(stderr, " est dans poly x:%f y:%f \n", T[n].coords[0] , T[n].coords[1]);
			if(estDansPolygone(T[n],T, nbPointsPoly) == 1)
				glColor3f(0.0, 1.0, 0);
			else
				glColor3f(1.0, 0.0, 0);
		}
		else
			glColor3f(1.0, 0.0, 0);
		glVertex2f(T[n].coords[0], T[n].coords[1]);
	}

	glEnd();

	glFlush();
}*/

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
	fprintf(stderr,"affichage argc %d\n", argc);
	if(argc == 1){
		printf("Number of Points undefined, 50 points used\n");
		nbPoints = 50;
	}
	else{
		while ((c = getopt(argc, argv, "n:")) != EOF)
		{
			switch (c)
			{
				case 'n': if ((sscanf(optarg, "%d", &nbPoints) != 1) || nbPoints <= 0)
							printf("%d points used\n", nbPoints);
						  break;
				case '?': printf("use option -nX (no space), with 0 < X.\n");
						  break;
				default : printf("Shouldn't be here, really...\n");
						  break;
			}
		}
	}
	//int option = 0;
	assert(nbPoints > 0);
	T = (vertex *) malloc(sizeof(vertex)*nbPoints+1);
	assert(T != NULL);
		
	selectPoints(nbPoints);
	fdp file = *allouerFDP();
	initFDP(&file);
	//printf("Veuillez choisir une option : Rien encore\n"); 
	//clock_t temps;
	remplissageListeParTab(&file, T, nbPoints);
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);  
	glutInitWindowPosition(5,5);  
	glutInitWindowSize(700, 700);  
	glutCreateWindow("My first OpenGL window...");  // Là, c'est une incantation (sic)* de fenêtre !
	winInit(); 
	
	glutDisplayFunc(displayTriangle);
  	glutMainLoop();  
	destructionFDP(&file);
  return EXIT_SUCCESS;  
}  

// *invocation
