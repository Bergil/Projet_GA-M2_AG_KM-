#include "vertex.h"

/**
 * \file triangle.h
 * \brief Header de la struct triangle
 * \author GRIVOLLAT-MALVEZIN
 * \version 0.1
 * \date 7 novembre 2014
 *
 * Déclaration de la structure triangle et ces méthodes associées
 *
 */


/**
 * \struct triangle
 * \brief Objet triangle
 *
 * triangle est un objet comprenant 3 points pour représenter un triangle 
 */
typedef struct s_triangle
{
	vertex* pt1; /*!< pointeur sur un point */
	vertex* pt2; /*!< pointeur sur un point */
	vertex* pt3; /*!< pointeur sur un point */
} triangle;

/**
 * \fn triangle* newTriangle();
 *
 * \brief Fonction de création d'une nouvelle instance d'un objet triangle avec ses points mis à NULL.
 *
 * \return Instance nouvellement allouée d'un objet de type triangle avec ses points mis à NULL.
 */
triangle* newTriangle();

/**
 * \fn triangle* newTriangle(vertex* p1, vertex* p2, vertex* p3)
 *
 * \brief Fonction de création d'une nouvelle instance d'un objet triangle à partir de ses 3 points
 *
 * \param pt1 Pointeur sur un point du triangle
 * \param pt2 Pointeur sur un point du triangle
 * \param pt3 Pointeur sur un point du triangle
 *
 * \return Instance nouvellement allouée d'un objet de type triangle avec ses points.
 */
triangle* newTriangle(vertex* p1, vertex* p2, vertex* p3);

/**
 * \fn void deleteAllTriangle(triangle* t)
 *
 * \brief Destruction d'un objet triangle ainsi que tous ses points
 *
 * \param t Pointeur sur un point du triangle
 */
void deleteAllTriangle(triangle* t);

/**
 * \fn void deleteTriangle(triangle* t)
 *
 * \brief Destruction d'un objet triangle sans detruire ses points
 *
 * \param t Pointeur sur un point du triangle
 */
void deleteTriangle(triangle* t);