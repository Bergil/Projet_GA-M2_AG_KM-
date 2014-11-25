#include "Vertex.h"
#include "List.h"
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
 * \struct s_triangle
 * \brief Objet Triangle
 *
 * triangle est un objet comprenant 3 points pour représenter un triangle 
 */
typedef struct s_triangle
{
	vertex* m_tab_points[3]; /*!< tableau de points */
	vertex* m_tab_voisins[3]; /*!< tableau de voisins*/
	List* m_list_candidats; /*!< pointeur sur un point */
} Triangle;

/**
 * \fn Triangle* newTriangle();
 *
 * \brief Fonction de création d'une nouvelle instance d'un objet triangle avec ses points mis à NULL.
 *
 * \return Instance nouvellement allouée d'un objet de type triangle avec ses points mis à NULL.
 */
Triangle* newTriangle();

/**
 * \fn Triangle* newTriangle(vertex* p1, vertex* p2, vertex* p3)
 *
 * \brief Fonction de création d'une nouvelle instance d'un objet triangle à partir de ses 3 points
 *
 * \param pt1 Pointeur sur un point du triangle
 * \param pt2 Pointeur sur un point du triangle
 * \param pt3 Pointeur sur un point du triangle
 *
 * \return Instance nouvellement allouée d'un objet de type triangle avec ses points.
 */
Triangle* newTriangleWithPoint(vertex* p1, vertex* p2, vertex* p3);

/**
 * \fn void deleteAllTriangle(Triangle* t)
 *
 * \brief Destruction d'un objet triangle ainsi que tous ses points
 *
 * \param t Pointeur sur un point du triangle
 */
void deleteAllTriangle(Triangle* t);

/**
 * \fn void deleteTriangle(Triangle* t)
 *
 * \brief Destruction d'un objet triangle sans detruire ses points
 *
 * \param t Pointeur sur un point du triangle
 */
void deleteTriangle(Triangle* t);


int estDansTriangle(Triangle* t, vertex *pt);
