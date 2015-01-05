#ifndef _SIMPLEX_H_
#define _SIMPLEX_H_

#include <GL/glut.h> 
#include "Vertex.h"
#include "List.h"
#include "EquationPlan.h"
#include <math.h>

#define IN 1
#define OUT 0
#define CONF -1

#define EGAL_HAUTEUR 0
#define SUP_HAUTEUR 1
#define INF_HAUTEUR -1

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

/** @defgroup s_simplex Simplex
 *
 * Un triangle dans l'espace
 * @{
**/

/**
 * \struct s_simplex
 *
 * \brief Objet Simplex
 *
 * Simplex est un objet comprenant 3 points pour représenter un Triangle 
 */
typedef struct s_simplex
{
	Vertex* m_tab_points[3]; /*!< tableau de points */
	struct s_simplex* m_tab_voisins[3]; /*!< tableau de voisins */
	List* m_list_candidats; /*!< pointeur sur un point */
	double m_hauteur; /*!< hauteur maximum des ses points par rapport à ce simplex */
	int m_afficher; /*!< booléen de prise en compte */
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

/**
 * \fn void ajoutPointsSimplex(Simplex* t, Vertex* p1, Vertex* p2, Vertex* p3)
 *
 * \brief Ajoute les nouveaux points au simplex
 *
 * \param t Pointeur sur un simplex
 * \param p1 Pointeur sur un nouveau point
 * \param p2 Pointeur sur un nouveau point
 * \param p3 Pointeur sur un nouveau point
 */
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
 * \fn void reattributionPoints2Simplex(Simplex *t1, Simplex* t2, List *l)
 *
 * \brief Redistribue les points de la liste au deux nouveaux simplex
 *
 * \param t1 pointeur sur un Simplex 
 * \param t2 Pointeur sur un simplex
 * \param l Liste de points
 */
void reattributionPoints2Simplex(Simplex *t1, Simplex* t2, List *l);

/**
 * \fn void reattributionPoints3Simplex(Simplex *t1, Simplex* t2, Simplex* t3, List *l)
 *
 * \brief Redistribue les points de la liste au deux nouveaux simplex
 *
 * \param t1 pointeur sur un Simplex 
 * \param t2 Pointeur sur un simplex
 * \param t3 Pointeur sur un simplex
 * \param l Liste de points
 */
void reattributionPoints3Simplex(Simplex *t1, Simplex* t2, Simplex* t3, List *l);

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

/**
 * \fn void affichageSimplex2D(Simplex * s)
 *
 * \brief Affichage du simplex dans un espace en deux dimmensions
 *
 * \param s pointeur sur un simplex
 */
void affichageSimplex2D(Simplex * s);

/**
 * \fn void affichageSimplex3DInterieur(Simplex * s)
 *
 * \brief Affichage du simplex dans un espace en trois dimmensions
 *
 * \param s pointeur sur un simplex
 */
void affichageSimplex3DInterieur(Simplex * s);

/**
 * \fn void affichageSimplex3DLigne(Simplex * s)
 *
 * \brief Affichage du simplex dans un espace en trois dimmensions
 *
 * \param s pointeur sur un simplex
 */
void affichageSimplex3DLigne(Simplex * s);

/**
 * \fn void getHauteur(Simplex * s)
 *
 * \brief Retourne la hauteur du simplex
 *
 * \param s pointeur sur un simplex
 */
float getHauteur(Simplex * s);

/**
 * \fn void copy(Simplex * s1, Simplex * s2)
 *
 * \brief S1 devient la copie du simplex s2
 *
 * \param s1 pointeur sur simplex que l'on souhaite modifier
 * \param s2 pointeur sur simplex que l'on souhaite copier
 */
void copy(Simplex * s1, Simplex * s2);

/**
 * \fn void ajouteVoisin(Simplex * s, Simplex * s_voisin)
 *
 * \brief Ajoute le simplex s_voisin au tableau de voisin de s
 * Il est ajouté à la place qui lui est destiné, à la position du point de s n'appartenant pas a s_voisin
 */
void ajouteVoisin(Simplex * s, Simplex * s_voisin);

/**
 * \fn int egaliteSimplex(Simplex * s1, Simplex * s2)
 *
 * \brief Test l'egalité de deux simplex
 *
 * \param s1 pointeur sur simplex
 * \param s2 pointeur sur simplex
 *
 * \return 1 si les simplex sont egaux, 0 sinon
 */
int egaliteSimplex(Simplex * s1, Simplex * s2);

/**
 * \fn void affichageSimplex(Simplex * s)
 *
 * \brief Affiche les points du simplex en coordonnées
 *
 * \param s pointeur sur simplex
 */
void affichageSimplex(Simplex * s);

/**
 * \fn int indiceDiff(Simplex * s1, Simplex * s2)
 *
 * \brief Retourne la position dans le tableau de point du point appartenant à s1 et pas à s2
 * s1 et s2 doivent être voisin pour que la fontion est un interet
 *
 * \param s1 pointeur sur simplex
 * \param s2 pointeur sur simplex voisin de s1
 *
 * \return retourne la position du point ou -1 si rien à été trouvé
 */
int indiceDiff(Simplex * s1, Simplex * s2);

/**
 * \fn int indicePosition(Simplex * s, Vertex * v)
 *
 * \brief retourne la position du point dans le tableau de point du simplex
 *
 * \param s pointeur sur simplex
 * \param v pointeur sur vertex
 *
 * \return position du point ou -1 si il n'appartient pas au simplex
 */
int indicePosition(Simplex * s, Vertex * v);

/**
 * \fn int ordreHauteur(Simplex *a, Simplex *b)
 *
 * \brief test la hauteur d'un simplex par rapport à un autre simplex
 *
 * \param a pointeur sur simplex que l'on test
 * \param b pointeur sur simplex
 *
 * \return EGAL_HAUTEUR 0, SUP_HAUTEUR 1, INF_HAUTEUR -1
 */
int ordreHauteur(Simplex *a, Simplex *b);

#endif
