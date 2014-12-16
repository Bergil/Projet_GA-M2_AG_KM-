#ifndef _SIMPLEX_H_
#define _SIMPLEX_H_

#include <GL/glut.h> 
#include "Vertex.h"
#include "List.h"
#include "EquationPlan.h"


/**
 * \file Simplex.h
 * \brief Header de la struct Simplex
 * \author GRIVOLLAT-MALVEZIN
 * \version 0.1
 * \date 7 novembre 2014
 *
 * Déclaration de la structure Simplex et ces méthodes associées
 *
 */


/**
 * \struct s_Simplex
 * \brief Objet Simplex
 *
 * Simplex est un objet comprenant 3 points pour représenter un Simplex 
 */
typedef struct s_simplex
{
	Vertex* m_tab_points[3]; /*!< tableau de points */
	struct Simplex* m_tab_voisins[3]; /*!< tableau de voisins */
	List* m_list_candidats; /*!< pointeur sur un point */
	double m_hauteur; /*!< hauteur maximum des ses points par rapport à ce simplex */
	EquationPlan m_equation_plan; /*!< equation du plan formé par les pints du simplex */
} Simplex;

/**
 * \fn Simplex* newSimplex();
 *
 * \brief Fonction de création d'une nouvelle instance d'un objet Simplex avec ses points mis à NULL.
 *
 * \return Instance nouvellement allouée d'un objet de type Simplex avec ses points mis à NULL.
 */
Simplex* newSimplex();

/**
 * \fn Simplex* newSimplex(vertex* p1, vertex* p2, vertex* p3)
 *
 * \brief Fonction de création d'une nouvelle instance d'un objet Simplex à partir de ses 3 points
 *
 * \param pt1 Pointeur sur un point du Simplex
 * \param pt2 Pointeur sur un point du Simplex
 * \param pt3 Pointeur sur un point du Simplex
 *
 * \return Instance nouvellement allouée d'un objet de type Simplex avec ses points.
 */
Simplex* newSimplexWithPoint(Vertex* p1, Vertex* p2, Vertex* p3);

void ajoutPointsSimplex(Simplex* t, Vertex* p1, Vertex* p2, Vertex* p3);

/**
 * \fn void deleteAllSimplex(Simplex* t)
 *
 * \brief Destruction d'un objet Simplex ainsi que tous ses points
 *
 * \param t Pointeur sur un point du Simplex
 */
void deleteAllSimplex(Simplex* t);

/**
 * \fn void deleteSimplex(Simplex* t)
 *
 * \brief Destruction d'un objet Simplex sans detruire ses points
 *
 * \param t Pointeur sur un point du Simplex
 */
void deleteSimplex(Simplex* t);

/**
 * \fn int estDansSimplex(Simplex* t, vertex *pt)
 *
 * \brief Test l'appartenance d'un point à un Simplex
 *
 * \param t Pointeur sur le Simplex
 * \param pt Pointeur sur le point tester
 *
 * \return -1 si les points sont alignés, 1 si le point est dans le Simplex, 0 sinon	
 */
int estDansSimplex(Simplex* t, Vertex *pt);

/**
 * \fn void ajoutPointCandidatQueue(Simplex* t, vertex *pt)
 *
 * \brief Ajout d'un point à la fin de la liste de candidat d'un Simplex
 *
 * \param t pointeur sur un Simplex
 * \param pt Pointeur sur le point nouveau candidat
 */
void ajoutPointCandidatQueue(Simplex* t, Vertex *pt);

/**
 * \fn void ajoutPointCandidatTete(Simplex* t, vertex *pt)
 *
 * \brief Ajout d'un point au début de la liste de candidat d'un Simplex
 *
 * \param t pointeur sur un Simplex 
 * \param pt Pointeur sur le point nouveau candidat
 */
void ajoutPointCandidatTete(Simplex* t, Vertex *pt);

/**
 * \fn void reatributionPoints(Simplex *t1, Simplex* t2, List *l)
 *
 * \brief Redistribue les points de la liste au deux nouveaux simplex
 *
 * \param t1 pointeur sur un Simplex 
 * \param t2 Pointeur sur un simplex
 * \param l Liste de points
 */
void reattributionPoints(Simplex *t1, Simplex* t2, List *l);

/**
 * \fn double calculHauteur(Simplex *s, Vertex *pt)
 *
 * \brief Calcul la hauteur d'un point par rapport au triangle
 *
 * \param s pointeur sur un Simplex 
 * \param pt Pointeur sur un simplex
 *
 * \return hauteur du triangle
 */
double calculHauteur(Simplex *s, Vertex *pt);



void affichageSimplex2D(Simplex * s);


#endif