 
#pragma once
#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h> 
#include "Tool.h"

#define DIM 3
#define ADROITE 1
#define ALIGNE 0
#define AGAUCHE -1

/**
 * \file Vertex.h
 * \brief Header de la struct Vertex
 * \author GRIVOLLAT-MALVEZIN
 * \version 0.1
 * \date 7 novembre 2014
 *
 * Déclaration de la structure Vertex et ces méthodes associées
 *
 */

 /** @defgroup s_vertex Vertex
 *
 * Un point dans l'espace
 * @{
**/


 /**
  * \struct s_vertex
  *
  * \brief Objet Vertex
  * 
  * Vertex est un point représenté par un ensemble de corrdonnées
  */
typedef struct s_vertex
{
	double coords[DIM]; /*!< tableau de coordonnées*/
} Vertex;

/**
 * \fn Vertex * newVertex()
 *
 * \brief Fonction de création d'un nouvelle instance d'un objet Vertex avec ses coordonnées mises à 0
 *
 * \return Vertex fraichement alloué
 */
Vertex * newVertex();

/**
 * \fn Vertex * newVertexWithCoords(double x, double y, double z)
 *
 * \brief Fonction de création d'une nouvelle instance d'un objet vertex avec passage de ses 3 premières dimensions
 *
 * \param x coordonnée x
 * \param y coordonnée y
 * \param z coordonnée z
 *
 * \return Vertex fraichement alloué
 */
Vertex * newVertexWithCoords(double x, double y, double z);

/**
 * \fn double determinant(const Vertex *A, const Vertex *B, const Vertex *C)
 *
 * \brief Retourne determinant
 *
 * \param A pointeur sur vertex
 * \param B pointeur sur vertex
 * \param C pointeur sur vertex 
 *
 * \return determinant
 */
double determinant(const Vertex *A, const Vertex *B, const Vertex *C);

/**
 * \fn int orientationPolaire(const Vertex *A, const Vertex *B, const Vertex *C)
 *
 * \brief Retourne l'orientation polaire des trois points
 *
 * \param A pointeur sur vertex 
 * \param B pointeur sur vertex
 * \param C pointeur sur vertex
 *
 * \return ALIGNE 0, ADROITE 1, ou AGAUCHE -1
 */
int orientationPolaire(const Vertex *A, const Vertex *B, const Vertex *C);

/**
 * \fn int egalite(Vertex* a, Vertex* b)
 *
 * \brief Test l'égalité entre deux Vertex
 *
 * \param a pointeur sur vertex
 * \param b pointeur sur vertex
 */
int egalite(Vertex* a, Vertex* b);

/**
 * \fn Vertex * minLexico(Vertex *a, Vertex *b, Vertex *c)
 *
 * \brief retourne le min lexicographique des trois points
 *
 * \param a pointeur sur vertex
 * \param b pointeur sur vertex
 * \param c pointeur sur vertex
 *
 * \return Vertex qui est le min lexicographique
 */
Vertex * minLexico(Vertex *a, Vertex *b, Vertex *c);

/**
 * \fn void affichageVertex(Vertex * v)
 *
 * \brief Affiche les coordonnées du vecteur
 *
 * \param v pointeur sur vertex
 */
void affichageVertex(Vertex * v);

/**
 * \fn void affichageVertex2D(Vertex * v)
 *
 * \brief Affiche le vertex en GL_POINTS dans une fenetre openGl deja existante
 *
 * \param v pointeur sur vertex à afficher
 */
void affichageVertex2D(Vertex * v);